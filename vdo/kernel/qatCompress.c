/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */


#include "qat.h"
#include "constants.h"
#include "dataKVIO.h"
#include "dataVIO.h"

/*
 * Max instances in a QAT device, each instance is a channel to submit
 * jobs to QAT hardware, this is only for pre-allocating instance and
 * session arrays; the actual number of instances are defined in the
 * QAT driver's configuration file.
 */
#define	QAT_DC_MAX_INSTANCES	48

/*
 * ZLIB head and foot size
 */
#define	ZLIB_HEAD_SZ		2
#define	ZLIB_FOOT_SZ		4

static CpaInstanceHandle dc_inst_handles[QAT_DC_MAX_INSTANCES];
static CpaDcSessionHandle session_handles[QAT_DC_MAX_INSTANCES];
static CpaBufferList **buffer_array[QAT_DC_MAX_INSTANCES];
static Cpa16U num_inst = 0;
static Cpa32U inst_num = 0;
static boolean_t qat_dc_init_done = B_FALSE;
static struct kmem_cache *qat_add_cache;

/**********************************************************************/
static void *qat_add_cache_alloc(void)
{
	return (kmem_cache_alloc(qat_add_cache, GFP_KERNEL));
}

/**********************************************************************/
static void qat_add_cache_free(void *add_cache)
{
	kmem_cache_free(qat_add_cache, add_cache);
}

/**********************************************************************/
boolean_t qat_dc_use_accel(size_t s_len)
{
	return (qat_dc_init_done &&
	    s_len >= QAT_MIN_BUF_SIZE &&
	    s_len <= QAT_MAX_BUF_SIZE);
}

/**********************************************************************/
/* 
 * Callback function using for QAT accelerator is registered in initialization step, 
 * and will be invoked when hardware accelerator completes the task. 
 * It has only two parameters, one is the pointer to callback parameter p_callback, 
 * another is status indicates whether compression during hardware processing is successful.
 */
static void qat_dc_callback(void *p_callback, CpaStatus status)
{
	DataKVIO *dataKVIO = (DataKVIO *)p_callback;
	QATCallbackTag *qat_p_callback = &dataKVIO->qatCallbackTag;
	Cpa16U i = qat_p_callback->i;
	CpaDcSessionHandle session_handle;
	session_handle = session_handles[i]; 
	Cpa32U compressed_sz;
	CpaBufferList *buf_list_src = qat_p_callback->buf_list_src;
	CpaBufferList *buf_list_dst = qat_p_callback->buf_list_dst;
	Cpa8U *buffer_meta_src = qat_p_callback->buffer_meta_src;
	Cpa8U *buffer_meta_dst = qat_p_callback->buffer_meta_dst;
	CpaFlatBuffer *flat_buf_dst = NULL;
	char *add = qat_p_callback->add;
	CpaDcRqResults *dc_results = &qat_p_callback->dc_results;

	DataVIO *dataVIO = &dataKVIO->dataVIO;
	ReadBlock *readBlock = &dataKVIO->readBlock;

	if (qat_p_callback->dir == QAT_COMPRESS) {

		if (status != CPA_STATUS_SUCCESS) {
			status = CPA_STATUS_FAIL;
			dataVIO->compression.size = VDO_BLOCK_SIZE + 1;
			goto fail;
		}

		/* In write workflow, the length of produced compressed result will be checked 
		 * if there is enough remaining space for footer size. If no enough footer size, 
		 * data will be marked as incompressible data similar to calling routine 
		 * and successor procedure will be jumped and go directly to the finial step. */
		compressed_sz = dc_results->produced;
		if (compressed_sz + ZLIB_HEAD_SZ + ZLIB_FOOT_SZ > VDO_BLOCK_SIZE) {
			status = CPA_STATUS_INCOMPRESSIBLE;
			dataVIO->compression.size = VDO_BLOCK_SIZE + 1;
			goto fail;
		}

		if (((compressed_sz + ZLIB_HEAD_SZ) % PAGE_SIZE)
		    + ZLIB_FOOT_SZ > PAGE_SIZE) {
			status = CPA_STATUS_INCOMPRESSIBLE;
			dataVIO->compression.size = VDO_BLOCK_SIZE + 1;
			goto fail;
		}


		flat_buf_dst = (CpaFlatBuffer *)(buf_list_dst + 1);
	
		flat_buf_dst->pData = (char*)((unsigned long)flat_buf_dst->pData + (compressed_sz + ZLIB_HEAD_SZ));
		flat_buf_dst->dataLenInBytes = ZLIB_FOOT_SZ;

		/* A footer API is called to generate zlib style footer, 
		 * using the same instance after retrieving dc_inst_num from QATCallbackTag. */
		dc_results->produced = 0;
		status = cpaDcGenerateFooter(session_handle,
		    flat_buf_dst, dc_results);
		if (status != CPA_STATUS_SUCCESS) {
			dataVIO->compression.size = VDO_BLOCK_SIZE + 1;
			goto fail;
		}
		Cpa32U destLen = compressed_sz + dc_results->produced + ZLIB_HEAD_SZ;

		if (status == CPA_STATUS_SUCCESS && destLen <= VDO_BLOCK_SIZE) {
    		/* The scratch block will be used to contain the compressed data. */
    		dataVIO->compression.data = dataKVIO->scratchBlock;
    		dataVIO->compression.size = destLen;
  		} else {
			/* Use block size plus one as an indicator for uncompressible data. */
    		dataVIO->compression.size = VDO_BLOCK_SIZE + 1;
  		} 
	} 

	else {
		if (status != CPA_STATUS_SUCCESS) {
			/* In read workflow, if status is not success, 
		 	 * the status field of readBlock will be marked as invalid fragment 
		 	 * that indicated a read failure has occurred. */
			readBlock->status = VDO_INVALID_FRAGMENT; 
			status = CPA_STATUS_FAIL;	
		} else {
			/* For decompression, input data is a compressed fragment, 
			 * a piece of data saved in dataBlocks,
			 * and result data are saved as scratchBlocks. */
			readBlock->data = dataKVIO->scratchBlock;
		}
	}

fail:

	QAT_PHYS_CONTIG_FREE(buffer_meta_src);
	QAT_PHYS_CONTIG_FREE(buffer_meta_dst);
	QAT_PHYS_CONTIG_FREE(buf_list_src);
	QAT_PHYS_CONTIG_FREE(buf_list_dst);

	if (qat_p_callback->dir == QAT_COMPRESS) {
		qat_add_cache_free(add);
		kvdoEnqueueDataVIOCallback(dataKVIO);
	} else {
		ReadBlock *readBlock = &dataKVIO->readBlock;
		readBlock->callback(dataKVIO);
	}
}

/**********************************************************************/
static void qat_dc_clean(void)
{
	Cpa16U buff_num = 0;
	Cpa16U num_inter_buff_lists = 0;

	for (Cpa16U i = 0; i < num_inst; i++) {
		cpaDcStopInstance(dc_inst_handles[i]);
		QAT_PHYS_CONTIG_FREE(session_handles[i]);

		/* free intermediate buffers  */
		if (buffer_array[i] != NULL) {
			cpaDcGetNumIntermediateBuffers(
			    dc_inst_handles[i], &num_inter_buff_lists);
		
			for (buff_num = 0; buff_num < num_inter_buff_lists; buff_num++) {
				CpaBufferList *buffer_inter = buffer_array[i][buff_num];
				
				if (buffer_inter->pBuffers) {
					QAT_PHYS_CONTIG_FREE(buffer_inter->pBuffers->pData);
					QAT_PHYS_CONTIG_FREE(buffer_inter->pBuffers);
				}
				
				QAT_PHYS_CONTIG_FREE(buffer_inter->pPrivateMetaData);
				QAT_PHYS_CONTIG_FREE(buffer_inter);
			}
		}
	}

	kmem_cache_destroy(qat_add_cache);
	qat_add_cache = NULL;

	num_inst = 0;
	qat_dc_init_done = B_FALSE;
}

/**********************************************************************/
int qat_dc_init(void)
{
	CpaStatus status = CPA_STATUS_SUCCESS;
	Cpa32U sess_size = 0;
	Cpa32U ctx_size = 0;
	Cpa16U num_inter_buff_lists = 0;
	Cpa16U buff_num = 0;
	Cpa32U buff_meta_size = 0;
	CpaDcSessionSetupData sd = {0};

	status = cpaDcGetNumInstances(&num_inst);
	if (status != CPA_STATUS_SUCCESS) {
		return (-1);
	}

	/* if the user has configured no QAT compression units just return */
	if (num_inst == 0) {
		return (0);
	}

	if (num_inst > QAT_DC_MAX_INSTANCES) {
		num_inst = QAT_DC_MAX_INSTANCES;
	}

	status = cpaDcGetInstances(num_inst, &dc_inst_handles[0]);
	if (status != CPA_STATUS_SUCCESS) {
		return (-1);
	}

	for (Cpa16U i = 0; i < num_inst; i++) {
		cpaDcSetAddressTranslation(dc_inst_handles[i], (void*)virt_to_phys);

		status = cpaDcBufferListGetMetaSize(dc_inst_handles[i], 1, &buff_meta_size);

		if (status == CPA_STATUS_SUCCESS) {
			status = cpaDcGetNumIntermediateBuffers(
				dc_inst_handles[i], 
				&num_inter_buff_lists);
		}

		if (status == CPA_STATUS_SUCCESS && num_inter_buff_lists != 0) {
			status = QAT_PHYS_CONTIG_ALLOC(
				&buffer_array[i],
			    num_inter_buff_lists * sizeof (CpaBufferList *));
		}

		for (buff_num = 0; buff_num < num_inter_buff_lists;
		    buff_num++) {
			if (status == CPA_STATUS_SUCCESS) {
				status = QAT_PHYS_CONTIG_ALLOC(
					&buffer_array[i][buff_num],
				    sizeof (CpaBufferList));
			}

			if (status == CPA_STATUS_SUCCESS) {
				status = QAT_PHYS_CONTIG_ALLOC(
					&buffer_array[i][buff_num]->pPrivateMetaData,
				    buff_meta_size);
			}

			if (status == CPA_STATUS_SUCCESS) {
				status = QAT_PHYS_CONTIG_ALLOC(
					&buffer_array[i][buff_num]->pBuffers,
				    sizeof (CpaFlatBuffer));
			}

			if (status == CPA_STATUS_SUCCESS) {
				/*
				 *  implementation requires an intermediate
				 *  buffer approximately twice the size of
				 *  output buffer, which is 2x max buffer
				 *  size here.
				 */
				status = QAT_PHYS_CONTIG_ALLOC(
					&buffer_array[i][buff_num]->pBuffers->pData,
					2 * QAT_MAX_BUF_SIZE);
				if (status != CPA_STATUS_SUCCESS) {
					goto fail;
				}

				buffer_array[i][buff_num]->numBuffers = 1;
				buffer_array[i][buff_num]->pBuffers-> dataLenInBytes = 2 * QAT_MAX_BUF_SIZE;
			}
		}

		status = cpaDcStartInstance(
			dc_inst_handles[i], 
			num_inter_buff_lists, buffer_array[i]);
		if (status != CPA_STATUS_SUCCESS) {
			goto fail;
		}

		sd.compLevel = CPA_DC_L1;
		sd.compType = CPA_DC_DEFLATE;
		sd.huffType = CPA_DC_HT_FULL_DYNAMIC;
		sd.sessDirection = CPA_DC_DIR_COMBINED;
		sd.sessState = CPA_DC_STATELESS;
		sd.deflateWindowSize = 7;
		sd.checksum = CPA_DC_ADLER32;
		status = cpaDcGetSessionSize(dc_inst_handles[i],
		    &sd, &sess_size, &ctx_size);
		if (status != CPA_STATUS_SUCCESS) {
			goto fail;
		}

		QAT_PHYS_CONTIG_ALLOC(&session_handles[i], sess_size);
		if (session_handles[i] == NULL) {
			goto fail;
		}

		status = cpaDcInitSession(dc_inst_handles[i],
		    session_handles[i],
		    &sd, NULL, qat_dc_callback);
		if (status != CPA_STATUS_SUCCESS) {
			goto fail;
		}
	}

	qat_add_cache = kmem_cache_create("qat_add_cache", (size_t)VDO_BLOCK_SIZE, 0, 0, NULL);
	if (!qat_add_cache) {
		return (-1);
	}

	qat_dc_init_done = B_TRUE;
	return (0);

fail:

	qat_dc_clean();
	return (-1);
}

/**********************************************************************/
/* 
 * Final step is responsible for freeing allocated memory 
 * and sending the item to compression processing module 
 * to redirect the successor module. 
 */
void qat_dc_fini(void)
{
	if (!qat_dc_init_done) {
		return;
	}

	qat_dc_clean();
}

/**********************************************************************/
/*
 * The "add" parameter is an additional buffer which is passed
 * to QAT as a scratch buffer alongside the destination buffer
 * in case the "compressed" data ends up being larger than the
 * original source data. This is necessary to prevent QAT from
 * generating buffer overflow warnings for incompressible data.
 */
static int qat_compress_impl(DataKVIO *dataKVIO, char *src, int src_len,
    char *dst, int dst_len, char *add, int add_len, size_t *c_len)
{
    QATCallbackTag *qat_dc_callback_tag = &dataKVIO->qatCallbackTag;
	
	CpaInstanceHandle dc_inst_handle;
	CpaDcSessionHandle session_handle;
	CpaBufferList *buf_list_src = qat_dc_callback_tag->buf_list_src;
	CpaBufferList *buf_list_dst = qat_dc_callback_tag->buf_list_dst;
	CpaFlatBuffer *flat_buf_src = NULL;
	CpaFlatBuffer *flat_buf_dst = NULL;
	Cpa8U *buffer_meta_src = qat_dc_callback_tag->buffer_meta_src;
	Cpa8U *buffer_meta_dst = qat_dc_callback_tag->buffer_meta_dst;
	Cpa32U buffer_meta_size = 0;

	CpaDcRqResults* dc_results = &qat_dc_callback_tag->dc_results;
	qat_compress_dir_t dir = qat_dc_callback_tag->dir;	

	CpaStatus status = CPA_STATUS_SUCCESS;
	Cpa32U hdr_sz = 0;
	Cpa32U num_src_buf = (src_len >> PAGE_SHIFT) + 2;
	Cpa32U num_dst_buf = (dst_len >> PAGE_SHIFT) + 2;
	Cpa32U num_add_buf = (add_len >> PAGE_SHIFT) + 2;
	
	Cpa16U i;

	/* We increment num_src_buf and num_dst_buf by 2 to allow
	 * us to handle non page-aligned buffer addresses and buffers
	 * whose sizes are not divisible by PAGE_SIZE. */
	Cpa32U src_buffer_list_mem_size = sizeof (CpaBufferList) +
	    (num_src_buf * sizeof (CpaFlatBuffer));
	Cpa32U dst_buffer_list_mem_size = sizeof (CpaBufferList) +
	    ((num_dst_buf + num_add_buf) * sizeof (CpaFlatBuffer));
	
	/* dc_inst_num is assigned in calling routine, in a way like round robin 
	 * with atomic operation, to load balance for hardware instances. */
	i = (Cpa32U)atomic_inc_return((atomic_t *)&inst_num) % num_inst;
	dc_inst_handle = dc_inst_handles[i];
	session_handle = session_handles[i];

	/* build source metadata buffer list */
	cpaDcBufferListGetMetaSize(dc_inst_handle, num_src_buf,
	    &buffer_meta_size);
	if (QAT_PHYS_CONTIG_ALLOC(&buffer_meta_src, buffer_meta_size) !=
	    CPA_STATUS_SUCCESS) {
		goto fail;
	}

	/* build destination metadata buffer list */
	cpaDcBufferListGetMetaSize(dc_inst_handle, num_dst_buf + num_add_buf,
	    &buffer_meta_size);
	if (QAT_PHYS_CONTIG_ALLOC(&buffer_meta_dst, buffer_meta_size) !=
	    CPA_STATUS_SUCCESS) {
		goto fail;
	}

	/* build source buffer list */
	if (QAT_PHYS_CONTIG_ALLOC(&buf_list_src, src_buffer_list_mem_size) !=
	    CPA_STATUS_SUCCESS) {
		goto fail;
	}
	
	flat_buf_src = (CpaFlatBuffer *)(buf_list_src + 1);

	buf_list_src->pBuffers = flat_buf_src; /* always point to first one */

	/* build destination buffer list */
	if (QAT_PHYS_CONTIG_ALLOC(&buf_list_dst, dst_buffer_list_mem_size) !=
	    CPA_STATUS_SUCCESS) {
		goto fail;
	}

	flat_buf_dst = (CpaFlatBuffer *)(buf_list_dst + 1);

	buf_list_dst->pBuffers = flat_buf_dst; /* always point to first one */

	buf_list_src->numBuffers = 1;
	buf_list_src->pPrivateMetaData = buffer_meta_src;
	flat_buf_src->pData = src;
	flat_buf_src->dataLenInBytes = src_len;

	buf_list_dst->numBuffers = 1;
	buf_list_dst->pPrivateMetaData = buffer_meta_dst;
	flat_buf_dst->pData = dst;
	flat_buf_dst->dataLenInBytes = dst_len;
	
	qat_dc_callback_tag->buffer_meta_src = buffer_meta_src;
	qat_dc_callback_tag->buffer_meta_dst = buffer_meta_dst;
	qat_dc_callback_tag->buf_list_src = buf_list_src;
	qat_dc_callback_tag->buf_list_dst = buf_list_dst;

	if (dir == QAT_COMPRESS) {
		flat_buf_dst++;
		buf_list_dst->numBuffers++;
		flat_buf_dst->pData = add;
		flat_buf_dst->dataLenInBytes = add_len;

		/* As for compression, a header API is called to generate zlib style header.
		 * The result will be put in the head of output buffer 
		 * with length of zlib header, which is fixed at 2 byte. */
		cpaDcGenerateHeader(session_handle,
		    buf_list_dst->pBuffers, &hdr_sz);
		buf_list_dst->pBuffers->pData += hdr_sz;
		buf_list_dst->pBuffers->dataLenInBytes -= hdr_sz;

		/* After data and context preparation, QAT kernel API for compression is called, 
		 * with a parameter as DataKVIO that preserving the QATCallbackTag in it. */
		status = cpaDcCompressData(
		    dc_inst_handle, session_handle,
		    buf_list_src, buf_list_dst,
		    dc_results, CPA_DC_FLUSH_FINAL,
		    dataKVIO);
		
		/* Once data is successfully sent to QAT accelerator, 
		 * the calling routine will finish this sending work and return the control back 
		 * to compression processing module, which will be ready to access next work item 
		 * from CPU queue and start a new sending work. */
		if (status != CPA_STATUS_SUCCESS) {
			goto fail;
		}
	} 
	
	else {
		if (dir != QAT_DECOMPRESS) {
			goto fail;
		}

		/* In decompression scenario, this header will be jumped, 
		 * leading hardware accelerator to start from content data. */
		buf_list_src->pBuffers->pData += ZLIB_HEAD_SZ;
		buf_list_src->pBuffers->dataLenInBytes -= ZLIB_HEAD_SZ;
		status = cpaDcDecompressData(dc_inst_handle, session_handle,
		    buf_list_src, buf_list_dst, dc_results, CPA_DC_FLUSH_FINAL,
		    dataKVIO);

		if (status != CPA_STATUS_SUCCESS) {
			status = CPA_STATUS_FAIL;
			goto fail;
		}
	}

	return status;

fail:
	
	QAT_PHYS_CONTIG_FREE(buffer_meta_src);
	QAT_PHYS_CONTIG_FREE(buffer_meta_dst);
	QAT_PHYS_CONTIG_FREE(buf_list_src);
	QAT_PHYS_CONTIG_FREE(buf_list_dst);

	if (dir == QAT_COMPRESS) {
		/* If errors are occurred when sending request to hardware accelerator, 
		 * this request will be interrupted and marked as incompressible data, 
		 * and allocated memory will be freed before leaving the calling routine. */
		qat_add_cache_free(add);
		DataVIO *dataVIO = &dataKVIO->dataVIO;
		dataVIO->compression.size = VDO_BLOCK_SIZE + 1;
		kvdoEnqueueDataVIOCallback(dataKVIO);
	}
	else {
		/* In read workflow, if status is not success, 
		 * the status field of readBlock will be marked as invalid fragment 
		 * that indicated a read failure has occurred. */
		ReadBlock *readBlock = &dataKVIO->readBlock;
		readBlock->status = VDO_INVALID_FRAGMENT;
		readBlock->callback(dataKVIO);
	}

	return (status);
}

/**********************************************************************/
/*
 * Entry point for QAT accelerated compression / decompression.
 */
int qat_compress(DataKVIO* dataKVIO, qat_compress_dir_t dir, char *src, int src_len,
    char *dst, int dst_len, size_t *c_len)
{
	int ret;
    size_t add_len = 0;
	
	QATCallbackTag *tag = &dataKVIO->qatCallbackTag;
	
	tag->dir = dir;
    if (dir == QAT_COMPRESS) {
		tag->add = qat_add_cache_alloc();
		add_len = VDO_BLOCK_SIZE;
    }

	ret = qat_compress_impl(dataKVIO, src, src_len, dst,
	    dst_len, tag->add, add_len, c_len);

	return (ret);
}


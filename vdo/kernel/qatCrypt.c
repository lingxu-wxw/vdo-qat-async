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

/*
 * This file represents the QAT implementation of checksums and encryption.
 * Internally, QAT shares the same cryptographic instances for both of these
 * operations, so the code has been combined here. QAT data compression uses
 * compression instances, so that code is separated into qat_compress.c
 */

#include "qat.h"
#include "constants.h"
#include "dataKVIO.h"
#include "dataVIO.h"

/*
 * Max instances in a QAT device, each instance is a channel to submit
 * jobs to QAT hardware, this is only for pre-allocating instances
 * and session arrays; the actual number of instances are defined in
 * the QAT driver's configure file.
 */
#define	QAT_CRYPT_MAX_INSTANCES		48

#define	MAX_PAGE_NUM			1024

static Cpa32U inst_num = 0;
static Cpa16U num_inst = 0;
static CpaInstanceHandle cy_inst_handles[QAT_CRYPT_MAX_INSTANCES];
static boolean_t qat_cy_init_done = B_FALSE;
int zfs_qat_encrypt_disable = 0;
int zfs_qat_checksum_disable = 0;

typedef struct cy_callback {
	CpaBoolean verify_result;
	struct completion complete;
} cy_callback_t;

/**********************************************************************/
static void symcallback(void *p_callback, CpaStatus status, const CpaCySymOp operation,
    void *op_data, CpaBufferList *buf_list_dst, CpaBoolean verify)
{
    // TODO
}

/**********************************************************************/
boolean_t qat_crypt_use_accel(size_t s_len)
{
    return (qat_cy_init_done &&
	    s_len >= QAT_MIN_BUF_SIZE &&
	    s_len <= QAT_MAX_BUF_SIZE);
}

/**********************************************************************/
boolean_t qat_checksum_use_accel(size_t s_len)
{
	return (qat_cy_init_done &&
	    s_len >= QAT_MIN_BUF_SIZE &&
	    s_len <= QAT_MAX_BUF_SIZE);
}

/**********************************************************************/
void qat_cy_clean(void)
{
    for (Cpa16U i = 0; i < num_inst; i++) {
		cpaCyStopInstance(cy_inst_handles[i]);
    }

	num_inst = 0;
	qat_cy_init_done = B_FALSE;
}

/**********************************************************************/
int qat_cy_init(void)
{
    CpaStatus status = CPA_STATUS_FAIL;

	// if (qat_cy_init_done) {
	// 	return (0);
    // }

	status = cpaCyGetNumInstances(&num_inst);
	if (status != CPA_STATUS_SUCCESS) {
		return (-1);
    }

	/* if the user has configured no QAT encryption units just return */
	if (num_inst == 0) {
		return (0);
    }

	if (num_inst > QAT_CRYPT_MAX_INSTANCES) {
		num_inst = QAT_CRYPT_MAX_INSTANCES;
    }

	status = cpaCyGetInstances(num_inst, &cy_inst_handles[0]);
	if (status != CPA_STATUS_SUCCESS) {
		return (-1);
    }

	for (Cpa16U i = 0; i < num_inst; i++) {
		status = cpaCySetAddressTranslation(cy_inst_handles[i],
		    (void *)virt_to_phys);
		if (status != CPA_STATUS_SUCCESS) {
			goto fail;
        }

		status = cpaCyStartInstance(cy_inst_handles[i]);
		if (status != CPA_STATUS_SUCCESS) {
			goto error;
        }
	}

	qat_cy_init_done = B_TRUE;
	return (0);

fail:

	qat_cy_clean();
	return (-1);
}

/**********************************************************************/
void qat_cy_fini(void)
{
    if (!qat_cy_init_done)
		return;

	qat_cy_clean();
}

/**********************************************************************/
static CpaStatus qat_init_crypt_session_ctx(qat_encrypt_dir_t dir, CpaInstanceHandle inst_handle,
    CpaCySymSessionCtx **cy_session_ctx, crypto_key_t *key,
    Cpa64U crypt, Cpa32U aad_len)
{
    CpaStatus status = CPA_STATUS_SUCCESS;
	Cpa32U ctx_size;
	Cpa32U ciper_algorithm;
	Cpa32U hash_algorithm;
	CpaCySymSessionSetupData sd = { 0 };

	// CCM是什么
	/* if (zio_crypt_table[crypt].ci_crypt_type == ZC_TYPE_CCM) {
		return (CPA_STATUS_FAIL);
	} else {
		ciper_algorithm = CPA_CY_SYM_CIPHER_AES_GCM;
		hash_algorithm = CPA_CY_SYM_HASH_AES_GCM;
	} */

	sd.cipherSetupData.cipherAlgorithm = ciper_algorithm;
	sd.cipherSetupData.pCipherKey = key->ck_data;
	sd.cipherSetupData.cipherKeyLenInBytes = key->ck_length / 8;
	sd.hashSetupData.hashAlgorithm = hash_algorithm;
	sd.hashSetupData.hashMode = CPA_CY_SYM_HASH_MODE_AUTH;
	sd.hashSetupData.digestResultLenInBytes = ZIO_DATA_MAC_LEN;
	sd.hashSetupData.authModeSetupData.aadLenInBytes = aad_len;
	sd.sessionPriority = CPA_CY_PRIORITY_NORMAL;
	sd.symOperation = CPA_CY_SYM_OP_ALGORITHM_CHAINING;
	sd.digestIsAppended = CPA_FALSE;
	sd.verifyDigest = CPA_FALSE;

	if (dir == QAT_ENCRYPT) {
		sd.cipherSetupData.cipherDirection =
		    CPA_CY_SYM_CIPHER_DIRECTION_ENCRYPT;
		sd.algChainOrder =
		    CPA_CY_SYM_ALG_CHAIN_ORDER_HASH_THEN_CIPHER;
	} else {
		// ASSERT3U(dir, ==, QAT_DECRYPT);
		sd.cipherSetupData.cipherDirection =
		    CPA_CY_SYM_CIPHER_DIRECTION_DECRYPT;
		sd.algChainOrder =
		    CPA_CY_SYM_ALG_CHAIN_ORDER_CIPHER_THEN_HASH;
	}

	status = cpaCySymSessionCtxGetSize(inst_handle, &sd, &ctx_size);
	if (status != CPA_STATUS_SUCCESS) {
		return (status);
	}

	status = QAT_PHYS_CONTIG_ALLOC(cy_session_ctx, ctx_size);
	if (status != CPA_STATUS_SUCCESS) {
		return (status);
	}

	// symcallback
	status = cpaCySymInitSession(inst_handle, symcallback, &sd,
	    *cy_session_ctx);
	if (status != CPA_STATUS_SUCCESS) {
		QAT_PHYS_CONTIG_FREE(*cy_session_ctx);
		return (status);
	}

	return (CPA_STATUS_SUCCESS);
}

/**********************************************************************/
static CpaStatus qat_init_checksum_session_ctx(CpaInstanceHandle inst_handle,
    CpaCySymSessionCtx **cy_session_ctx, Cpa64U cksum)
{
    CpaStatus status = CPA_STATUS_SUCCESS;
	Cpa32U ctx_size;
	Cpa32U hash_algorithm;
	CpaCySymSessionSetupData sd = { 0 };

	/*
	 * ZFS's SHA512 checksum is actually SHA512/256, which uses
	 * a different IV from standard SHA512. QAT does not support
	 * SHA512/256, so we can only support SHA256.
	 */
	// zio_checksum
	if (cksum == ZIO_CHECKSUM_SHA256) {
		hash_algorithm = CPA_CY_SYM_HASH_SHA256;
	} else {
		return (CPA_STATUS_FAIL);
	}

	sd.sessionPriority = CPA_CY_PRIORITY_NORMAL;
	sd.symOperation = CPA_CY_SYM_OP_HASH;
	sd.hashSetupData.hashAlgorithm = hash_algorithm;
	sd.hashSetupData.hashMode = CPA_CY_SYM_HASH_MODE_PLAIN;
	sd.hashSetupData.digestResultLenInBytes = sizeof (zio_cksum_t);
	sd.digestIsAppended = CPA_FALSE;
	sd.verifyDigest = CPA_FALSE;

	status = cpaCySymSessionCtxGetSize(inst_handle, &sd, &ctx_size);
	if (status != CPA_STATUS_SUCCESS) {
		return (status);
	}

	status = QAT_PHYS_CONTIG_ALLOC(cy_session_ctx, ctx_size);
	if (status != CPA_STATUS_SUCCESS) {
		return (status);
	}

	status = cpaCySymInitSession(inst_handle, symcallback, &sd,
	    *cy_session_ctx);
	if (status != CPA_STATUS_SUCCESS) {
		QAT_PHYS_CONTIG_FREE(*cy_session_ctx);
		return (status);
	}

	return (CPA_STATUS_SUCCESS);
}

/**********************************************************************/
static CpaStatus qat_init_cy_buffer_lists(CpaInstanceHandle inst_handle, uint32_t nr_bufs,
    CpaBufferList *src, CpaBufferList *dst)
{
    CpaStatus status = CPA_STATUS_SUCCESS;
	Cpa32U meta_size = 0;

	status = cpaCyBufferListGetMetaSize(inst_handle, nr_bufs, &meta_size);
	if (status != CPA_STATUS_SUCCESS) {
		return (status);
	}

	status = QAT_PHYS_CONTIG_ALLOC(&src->pPrivateMetaData, meta_size);
	if (status != CPA_STATUS_SUCCESS) {
		goto fail;
	}

	if (src != dst) {
		status = QAT_PHYS_CONTIG_ALLOC(&dst->pPrivateMetaData,
		    meta_size);
		if (status != CPA_STATUS_SUCCESS) {
			goto fail;
		}
	}

	return (CPA_STATUS_SUCCESS);

fail:
	QAT_PHYS_CONTIG_FREE(src->pPrivateMetaData);
	if (src != dst) {
		QAT_PHYS_CONTIG_FREE(dst->pPrivateMetaData);
	}

	return (status);
}

/**********************************************************************/
int qat_crypt(DataKVIO* dataKVIO, qat_encrypt_dir_t dir, uint8_t *src_buf, uint8_t *dst_buf,
    uint8_t *aad_buf, uint32_t aad_len, uint8_t *iv_buf, uint8_t *digest_buf,
    crypto_key_t *key, uint64_t crypt, uint32_t enc_len)
{
    CpaStatus status = CPA_STATUS_SUCCESS;
	Cpa16U i;
	CpaInstanceHandle cy_inst_handle;
	Cpa16U nr_bufs = (enc_len >> PAGE_SHIFT) + 2;
	Cpa32U bytes_left = 0;
	Cpa8S *data = NULL;
	CpaCySymSessionCtx *cy_session_ctx = NULL;
	cy_callback_t cb;
	CpaCySymOpData op_data = { 0 };
	CpaBufferList src_buffer_list = { 0 };
	CpaBufferList dst_buffer_list = { 0 };
	CpaFlatBuffer *flat_src_buf_array = NULL;
	CpaFlatBuffer *flat_src_buf = NULL;
	CpaFlatBuffer *flat_dst_buf_array = NULL;
	CpaFlatBuffer *flat_dst_buf = NULL;
	struct page *in_pages[MAX_PAGE_NUM];
	struct page *out_pages[MAX_PAGE_NUM];
	Cpa32U in_page_num = 0;
	Cpa32U out_page_num = 0;
	Cpa32U in_page_off = 0;
	Cpa32U out_page_off = 0;


	return 0;
}

/**********************************************************************/
int qat_checksum(uint64_t cksum, uint8_t *buf, uint64_t size, zio_cksum_t *zcp)
{
	CpaStatus status;
	Cpa16U i;
	CpaInstanceHandle cy_inst_handle;
	Cpa16U nr_bufs = (size >> PAGE_SHIFT) + 2;
	Cpa32U bytes_left = 0;
	Cpa8S *data = NULL;
	CpaCySymSessionCtx *cy_session_ctx = NULL;
	cy_callback_t cb;
	Cpa8U *digest_buffer = NULL;
	CpaCySymOpData op_data = { 0 };
	CpaBufferList src_buffer_list = { 0 };
	CpaFlatBuffer *flat_src_buf_array = NULL;
	CpaFlatBuffer *flat_src_buf = NULL;
	struct page *in_pages[MAX_PAGE_NUM];
	Cpa32U page_num = 0;
	Cpa32U page_off = 0;


    return 0;
}



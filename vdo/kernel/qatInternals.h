#ifndef _QAT_INTERNALS_H
#define _QAT_INTERNALS_H

#include "cpa.h"
#include "dc/cpa_dc.h"

typedef enum qat_compress_dir {
	QAT_DECOMPRESS = 0,
	QAT_COMPRESS = 1,
} qat_compress_dir_t;

typedef enum qat_encrypt_dir {
	QAT_DECRYPT = 0,
	QAT_ENCRYPT = 1,
} qat_encrypt_dir_t;

typedef enum {
	B_FALSE = 0,
	B_TRUE = 1,
} boolean_t;

typedef enum qat_crypt_type {
	QC_TYPE_NONE = 0,
	QC_TYPE_CCM,
	QC_TYPE_GCM
} qat_crypt_type_t;

typedef struct qat_cksum {
	uint64_t	qc_word[4];
} qat_cksum_t;

typedef struct
{
	CpaDcRqResults dc_results;
	qat_compress_dir_t dir;
	Cpa16U i;
	Cpa8U *buffer_meta_src;
	Cpa8U *buffer_meta_dst;
	CpaBufferList *buf_list_src;
	CpaBufferList *buf_list_dst;
	char *add;
} QATCallbackTag;

#endif /* _QAT_INTERNALS_H  */

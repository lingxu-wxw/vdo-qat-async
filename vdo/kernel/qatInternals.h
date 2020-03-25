#ifndef _QAT_INTERNALS_H
#define _QAT_INTERNALS_H

#include "cpa.h"
#include "dc/cpa_dc.h"

typedef enum qat_compress_dir {
	QAT_DECOMPRESS = 0,
	QAT_COMPRESS = 1,
} qat_compress_dir_t;

typedef enum {
	B_FALSE = 0,
	B_TRUE = 1,
} boolean_t;


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

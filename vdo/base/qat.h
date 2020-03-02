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

#ifndef	_SYS_QAT_H
#define	_SYS_QAT_H

#include "cpa.h"
#include "dc/cpa_dc.h"
#include "dataKVIO.h"
#include "dataVIO.h"
#include "qatInternals.h"

#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
#include <linux/completion.h>
#include <linux/atomic.h>

/*
 * Timeout - no response from hardware after 0.5 seconds
 */
#define	QAT_TIMEOUT_MS		500

/*
 * The minimal and maximal buffer size which are not restricted
 * in the QAT hardware, but with the input buffer size between 4KB
 * and 128KB the hardware can provide the optimal performance.
 */
#define	QAT_MIN_BUF_SIZE	(4*1024)
#define	QAT_MAX_BUF_SIZE	(128*1024)

/* inlined for performance */
static inline struct page *
qat_mem_to_page(void *addr)
{
	if (!is_vmalloc_addr(addr))
		return (virt_to_page(addr));

	return (vmalloc_to_page(addr));
}


CpaStatus qat_mem_alloc_contig(void **pp_mem_addr, Cpa32U size_bytes);
void qat_mem_free_contig(void **pp_mem_addr);
#define	QAT_PHYS_CONTIG_ALLOC(pp_mem_addr, size_bytes)	\
	qat_mem_alloc_contig((void *)(pp_mem_addr), (size_bytes))
#define	QAT_PHYS_CONTIG_FREE(p_mem_addr)	\
	qat_mem_free_contig((void *)&(p_mem_addr))

extern int qat_dc_init(void);
extern void qat_dc_fini(void);

extern int qat_init(void);
extern void qat_fini(void);

/* fake CpaStatus used to indicate data was not compressible */
#define	CPA_STATUS_INCOMPRESSIBLE				(-127)

extern boolean_t qat_dc_use_accel(size_t s_len);

extern int qat_compress(DataKVIO *dataKVIO, qat_compress_dir_t dir, char *src, int src_len,
    char *dst, int dst_len, size_t *c_len);

#endif /* _SYS_QAT_H */

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
#include "statusCodes.h"
#include <linux/slab.h>

/**********************************************************************/
CpaStatus qat_mem_alloc_contig(void **pp_mem_addr, Cpa32U size_bytes)
{
	*pp_mem_addr = kmalloc(size_bytes, GFP_KERNEL);
	if (*pp_mem_addr == NULL)
		return (CPA_STATUS_RESOURCE);
	return (CPA_STATUS_SUCCESS);
}

/**********************************************************************/
void qat_mem_free_contig(void **pp_mem_addr)
{
	if (*pp_mem_addr != NULL) {
		kfree(*pp_mem_addr);
		*pp_mem_addr = NULL;
	}
}

/**********************************************************************/
int qat_init(void)
{
	int ret;

	ret = qat_dc_init();
	if (ret != 0)
		return (ret);

	return VDO_SUCCESS;
}

/**********************************************************************/
void qat_fini(void)
{
	qat_dc_fini();
}

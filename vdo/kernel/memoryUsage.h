/*
 * Copyright (c) 2018 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA. 
 *
 * $Id: //eng/vdo-releases/aluminum/src/c++/vdo/kernel/memoryUsage.h#1 $
 */

#ifndef MEMORY_USAGE_H
#define MEMORY_USAGE_H 1

#include "memoryAlloc.h"

#include "kernelStatistics.h"

/**
 * Get the memory usage for statistics reporting.
 *
 * @return The memory usage
 **/
MemoryUsage getMemoryUsage(void)
  __attribute__((warn_unused_result));

#endif /* MEMORY_USAGE_H */

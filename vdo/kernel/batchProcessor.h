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
 * $Id: //eng/vdo-releases/aluminum/src/c++/vdo/kernel/batchProcessor.h#2 $
 */

#ifndef BATCHPROCESSOR_H
#define BATCHPROCESSOR_H

#include "kernelTypes.h"
#include "util/funnelQueue.h"

/**
 * Control data for managing collections of objects to be operated on
 * by a specified function. May be used when the work function is
 * lightweight enough or cache-contentious enough that it makes sense
 * to try to accumulate multiple objects and operate on them all at
 * once in one thread.
 *
 * The work function is run in one of the kernel layer's "CPU queues",
 * and care is taken to ensure that only one invocation can be running
 * or scheduled at any given time. It can loop calling nextBatchItem
 * repeatedly until there are no more objects to operate on. It should
 * also call condReschedBatchProcessor now and then, to play nicely
 * with the OS scheduler.
 *
 * Objects to operate on are manipulated through a FunnelQueueEntry
 * object which must be contained within them.
 **/
typedef struct batchProcessor BatchProcessor;

typedef void (*BatchProcessorCallback)(BatchProcessor *batch, void *closure);

/**
 * Creates a batch-processor control structure.
 *
 * @param [in]  layer     The kernel layer data, used to enqueue work items
 * @param [in]  callback  A function to process the accumulated objects
 * @param [in]  closure   A private data pointer for use by the callback
 * @param [out] batchPtr  Where to store the pointer to the new object
 *
 * @return   UDS_SUCCESS or an error code
 **/
int makeBatchProcessor(KernelLayer             *layer,
                       BatchProcessorCallback   callback,
                       void                    *closure,
                       BatchProcessor         **batchPtr);

/**
 * Adds an object to the processing queue.
 *
 * <p>If the callback function is not currently running or scheduled to be run,
 * it gets queued up to run.
 *
 * @param [in] batch  The batch-processor data
 * @param [in] item   The handle on the new object to add
 **/
void addToBatchProcessor(BatchProcessor *batch, KvdoWorkItem *item);

/**
 * Fetches the next object in the processing queue.
 *
 * @param [in]  batch  The batch-processor data
 *
 * @return   An object pointer or NULL
 **/
KvdoWorkItem *nextBatchItem(BatchProcessor *batch)
  __attribute__((warn_unused_result));

/**
 * Free the batch-processor data and null out the pointer.
 *
 * @param [in,out] batchPtr  Where the BatchProcessor pointer is stored
 **/
void freeBatchProcessor(BatchProcessor **batchPtr);

/**
 * Yield control to the scheduler if the kernel has indicated that
 * other work needs to run on the current processor.
 *
 * The data structure is needed so that the spin lock can be
 * (conditionally) released and re-acquired.
 *
 * @param [in]  batch  The batch-processor data
 **/
void condReschedBatchProcessor(BatchProcessor *batch);

#endif // BATCHPROCESSOR_H

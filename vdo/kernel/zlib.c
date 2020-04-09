/*
 *  Copyright (C) 2007-2010 Lawrence Livermore National Security, LLC.
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Brian Behlendorf <behlendorf1@llnl.gov>.
 *  UCRL-CODE-235197
 *
 *  This file is part of the SPL, Solaris Porting Layer.
 *  For details, see <http://zfsonlinux.org/>.
 *
 *  The SPL is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  The SPL is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the SPL.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  z_compress_level/z_uncompress are nearly identical copies of the
 *  compress2/uncompress functions provided by the official zlib package
 *  available at http://zlib.net/.  The only changes made we to slightly
 *  adapt the functions called to match the linux kernel implementation
 *  of zlib.  The full zlib license follows:
 *
 *  zlib.h -- interface of the 'zlib' general purpose compression library
 *  version 1.2.5, April 19th, 2010
 *
 *  Copyright (C) 1995-2010 Jean-loup Gailly and Mark Adler
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *  Jean-loup Gailly
 *  Mark Adler
 */


#include "zlib.h"

#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/sched.h>

static struct kmem_cache *zlib_workspace_cache;

static void *
zlib_workspace_alloc(void)
{
	return (kmem_cache_alloc(zlib_workspace_cache, GFP_KERNEL));
}

static void
zlib_workspace_free(void *workspace)
{
	kmem_cache_free(zlib_workspace_cache, workspace);
}

int
zlib_compress_level(void *dest, size_t *destLen, const void *source,
    size_t sourceLen, int level)
{
	z_stream stream;
	int err;

	stream.next_in = (Byte *)source;
	stream.avail_in = (uInt)sourceLen;
	stream.next_out = dest;
	stream.avail_out = (uInt)*destLen;

	if ((size_t)stream.avail_out != *destLen)
		return (Z_BUF_ERROR);

	stream.workspace = zlib_workspace_alloc();
	if (!stream.workspace)
		return (Z_MEM_ERROR);

	err = zlib_deflateInit(&stream, level);
	if (err != Z_OK) {
		zlib_workspace_free(stream.workspace);
		return (err);
	}

	err = zlib_deflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		zlib_deflateEnd(&stream);
		zlib_workspace_free(stream.workspace);
		return (err == Z_OK ? Z_BUF_ERROR : err);
	}
	*destLen = stream.total_out;

	err = zlib_deflateEnd(&stream);
	zlib_workspace_free(stream.workspace);

	return (err);
}

int
zlib_uncompress(void *dest, size_t *destLen, const void *source, size_t sourceLen)
{
	z_stream stream;
	int err;

	stream.next_in = (Byte *)source;
	stream.avail_in = (uInt)sourceLen;
	stream.next_out = dest;
	stream.avail_out = (uInt)*destLen;

	if ((size_t)stream.avail_out != *destLen)
		return (Z_BUF_ERROR);

	stream.workspace = zlib_workspace_alloc();
	if (!stream.workspace)
		return (Z_MEM_ERROR);

	err = zlib_inflateInit(&stream);
	if (err != Z_OK) {
		zlib_workspace_free(stream.workspace);
		return (err);
	}

	err = zlib_inflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		zlib_inflateEnd(&stream);
		zlib_workspace_free(stream.workspace);

		if (err == Z_NEED_DICT ||
		    (err == Z_BUF_ERROR && stream.avail_in == 0))
			return (Z_DATA_ERROR);

		return (err);
	}
	*destLen = stream.total_out;

	err = zlib_inflateEnd(&stream);
	zlib_workspace_free(stream.workspace);

	return (err);
}

int
zlib_init(void)
{
	int size;

	size = MAX(zlib_deflate_workspacesize(MAX_WBITS, MAX_MEM_LEVEL),
	    zlib_inflate_workspacesize());

	zlib_workspace_cache = kmem_cache_create(
	    "zlib_workspace_cache",
	    size, 0, 0, NULL);
	if (!zlib_workspace_cache)
		return (1);

	return (0);
}

void
zlib_fini(void)
{
	kmem_cache_destroy(zlib_workspace_cache);
	zlib_workspace_cache = NULL;
}

/* testmini.c -- very simple test program for the miniLZO library

   This file is part of the LZO real-time data compression library.

   Copyright (C) 2011 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2010 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2009 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2008 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2007 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2006 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2005 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2004 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2003 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2002 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2001 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2000 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1999 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/opensource/lzo/
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


/*************************************************************************
// This program shows the basic usage of the LZO library.
// We will compress a block of data and decompress again.
//
// For more information, documentation, example programs and other support
// files (like Makefiles and build scripts) please download the full LZO
// package from
//    http://www.oberhumer.com/opensource/lzo/
**************************************************************************/

/* First let's include "minizo.h". */

#include "minilzo.h"


/* We want to compress the data block at 'in' with length 'IN_LEN' to
 * the block at 'out'. Because the input block may be incompressible,
 * we must provide a little more output space in case that compression
 * is not possible.
 */

#define IN_LEN      (32*1024*1024u)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

static unsigned char __LZO_MMODEL in  [ IN_LEN ];
static unsigned char __LZO_MMODEL out [ OUT_LEN ];


/* Work-memory needed for compression. Allocate memory in units
 * of 'lzo_align_t' (instead of 'char') to make sure it is properly aligned.
 */

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);


/*************************************************************************
//
**************************************************************************/

int main(int argc, char *argv[])
{
    int r;
    lzo_uint in_len;
    lzo_uint out_len;
    lzo_uint new_len;
	lzo_uint org_len;
	struct timeval stime, etime;

    if (argc != 2 && argv == NULL)   /* avoid warning about unused args */
        return 0;

	org_len = atol(argv[1]);

    fprintf(stderr, "\nLZO real-time data compression library (v%s, %s).\n",
           lzo_version_string(), lzo_version_date());
    fprintf(stderr, "Copyright (C) 1996-2011 Markus Franz Xaver Johannes Oberhumer\nAll Rights Reserved.\n\n");


/*
 * Step 1: initialize the LZO library
 */
    if (lzo_init() != LZO_E_OK)
    {
        fprintf(stderr, "internal error - lzo_init() failed !!!\n");
        fprintf(stderr, "(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
        return 3;
    }

/*
 * Step 2: prepare the input block that will get compressed.
 *         We just fill it with zeros in this example program,
 *         but you would use your real-world data here.
 */

	if ((in_len = fread(in, 1, IN_LEN, stdin))){
		fprintf(stderr, "In file size is %ld\n", in_len);
	}else{
		fprintf(stderr, "Read in file error!\n");
	}
		

/*
 * Step 3: compress from 'in' to 'out' with LZO1X-1
 */
#if 0
    r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
    if (r == LZO_E_OK)
        fprintf(stderr, "compressed %lu bytes into %lu bytes\n",
            (unsigned long) in_len, (unsigned long) out_len);
    else
    {
        /* this should NEVER happen */
        fprintf(stderr, "internal error - compression failed: %d\n", r);
        return 2;
    }
    /* check for an incompressible block */
    if (out_len >= in_len)
    {
        fprintf(stderr, "This block contains incompressible data.\n");
        return 0;
    }

	if (fwrite(out, 1, out_len, stdout) != out_len){
		fprintf(stderr, "Write after compressed data to out file error!\n");
	}else{
		fprintf(stderr, "Done write %d bytes data to out data\n", out_len);
	}
#endif
/*
 * Step 4: decompress again, now going from 'out' to 'in'
 */
    new_len = org_len;

	gettimeofday(&stime, NULL);	

    r = lzo1x_decompress_safe(in, in_len, out, &new_len,NULL);

	gettimeofday(&etime, NULL);

    if (r == LZO_E_OK && new_len == org_len)

        fprintf(stderr, "decompressed %lu bytes back into %lu bytes, spent time is %lfms\n",
            (unsigned long) in_len, (unsigned long) new_len, (etime.tv_sec - stime.tv_sec) * 1000.0 + (etime.tv_usec - stime.tv_usec)/1000.0);
    else
    {
        /* this should NEVER happen */
        fprintf(stderr, "internal error - decompression failed: %d\n", r);
        return 1;
    }

	if (fwrite(out, 1, new_len, stdout) != new_len){
		fprintf(stderr, "Write after compressed data to out file error!\n");
	}else{
		fprintf(stderr, "Done write %ld bytes data to out data\n", new_len);
	}

    return 0;
}

/*
vi:ts=4:et
*/


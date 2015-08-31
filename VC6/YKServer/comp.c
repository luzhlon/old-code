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
	struct timeval stime, etime;

    if (argc < 0 && argv == NULL)   /* avoid warning about unused args */
        return 0;

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

	/*XXX:Compute spent times */
	gettimeofday(&stime, NULL);

    r = lzo1x_1_compress(in,in_len,out,&out_len, wrkmem);

	gettimeofday(&etime, NULL);

    if (r == LZO_E_OK)
        fprintf(stderr, "compressed %lu bytes into %lu bytes, spent %lfms ratio %f\n",
            (unsigned long) in_len, (unsigned long) out_len, (etime.tv_sec - stime.tv_sec) * 1000.0 + (etime.tv_usec - stime.tv_usec)/1000.0, \
		    out_len*1.0/in_len*100);
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
		fprintf(stderr, "Done write %ld bytes data to out data\n", out_len);
	}

#if 0
/*
 * Step 4: decompress again, now going from 'out' to 'in'
 */
    new_len = in_len;
    r = lzo1x_decompress(out,out_len,in,&new_len,NULL);
    if (r == LZO_E_OK && new_len == in_len)
        printf("decompressed %lu bytes back into %lu bytes\n",
            (unsigned long) out_len, (unsigned long) in_len);
    else
    {
        /* this should NEVER happen */
        printf("internal error - decompression failed: %d\n", r);
        return 1;
    }
#endif


    return 0;
}

/*
vi:ts=4:et
*/


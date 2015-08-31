#include "minilzo.h"
#include "minilzo.c"

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);

#define Compress lzo1x_1_compress
#define deCompress lzo1x_decompress
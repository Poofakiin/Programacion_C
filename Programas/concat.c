#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "concat.h"

uint concat_bits(uint x, int n, uint y, int m) {
    x<<=sizeof(x)*8-n;
    x>>=sizeof(x)*8-n;
    x<<=m;
    y<<=sizeof(x)*8-m;
    y>>=sizeof(x)*8-m;
    return x|y;
}



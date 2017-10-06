#include "stdafx.h"
#include "mmemory.h"
#include <stdlib.h>

int main( )
{
    printf("Hello, World!\n");
    //_init(10,2);
    int * p ;
    int * i ;
    VA addr;
    addr = calloc(16, sizeof(char));

    addr[0] = '1';
    addr[1] = '1';
    addr[2] = '1';
    addr[3] = '1';
    addr[4] = '1';
    addr[5] = '1';
    addr[6] = '1';
    addr[7] = '1';
    addr[8] = '1';
    addr[9] = '1';
    addr[10] = '1';
    addr[11] = '1';
    addr[12] = '1';
    addr[13] = '1';
    addr[14] = '1';
    addr[15] = '1';

    //toDecimalNumber(addr,p,i);
    printf(p);
    return 0;
}

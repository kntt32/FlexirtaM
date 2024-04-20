#include "FlexirtaM_Build.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "FlexirtaM.h"

#if MATRIX_ENABLE_RDRAND
static unsigned long long Matrix_Method(RdRand)() {
    unsigned long long result = 0;
    asm("rdrand %0" : "=r"(result));
    return result;
}
#endif

Matrix_BaseNumber Matrix_Method(Math_Random)() {
#if MATRIX_ENABLE_RDRAND
    return ((Matrix_BaseNumber)(Matrix_Method(RdRand)()%0x100000))/0x100000;
#else
    static int seedinited = 0;
    if(!seedinited) {
        srand((unsigned int)time(NULL));
        seedinited = 1;
    }
    return (Matrix_BaseNumber)(rand()%10000)/10000;
#endif
}

unsigned long long Matrix_Method(Math_Factorial)(unsigned int number) {
    if(number == 0) return 1;
    unsigned long long result = number;
    for(int i=1; i<number; i++) {
        result *= i;
    }
    return result;
}

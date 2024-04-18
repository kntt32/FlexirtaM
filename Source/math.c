#include "buildoption.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef BASE_NUMBER_TYPE Matrix_BaseNumber;
#include "FlexirtaM.h"

#if ENABLE_RDRAND
unsigned long long Matrix_RdRand() {
    unsigned long long result = 0;
    asm("rdrand %0" : "=r"(result));
    return result;
}
#endif

Matrix_BaseNumber Matrix_Math_Random() {
#if ENABLE_RDRAND
    return ((Matrix_BaseNumber)(Matrix_RdRand()%0x100000))/0x100000;
#else
    static int seedinited = 0;
    if(!seedinited) {
        srand((unsigned int)time(NULL));
        seedinited = 1;
    }
    return (Matrix_BaseNumber)(rand()%10000)/10000;
#endif
}

unsigned long long Matrix_Math_Factorial(unsigned int number) {
    if(number == 0) return 1;
    unsigned long long result = number;
    for(int i=1; i<number; i++) {
        result *= i;
    }
    return result;
}


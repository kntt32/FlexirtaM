#include "buildoption.h"

#include <stdio.h>
#include <stdlib.h>


#define Matrix_BaseNumber BASE_NUMBER_TYPE
#include "FlexirtaM.h"

int main() {
    Matrix_double* mat1 = Matrix_double_New(2,2);
    Matrix_double* mat2 = Matrix_double_New(2,2);
    {
        Matrix_BaseNumber temp[] = {1,2,3,4};
        Matrix_double_Array(mat1, 2, 2, temp);
        Matrix_BaseNumber temp2[] = {5,6,7,8};
        Matrix_double_Array(mat2, 2,2,temp2);
    }
    printf("%lf\n", Matrix_double_Det(mat1));
    Matrix_double_Print(mat1);
    printf("\n");
    
    Matrix_double_Print(mat2);
    printf("\n");

    Matrix_double* resultmat = Matrix_double_New(2,2);

    Matrix_double_DotFast(resultmat, mat1, mat2);
    Matrix_double_Print(resultmat);
    return 0;


    return 0;
}

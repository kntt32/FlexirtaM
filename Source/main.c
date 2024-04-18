#include "buildoption.h"

#include <stdio.h>
#include <stdlib.h>

typedef BASE_NUMBER_TYPE Matrix_BaseNumber;
#include "FlexirtaM.h"

int main() {
    Matrix* mat1 = Matrix_New(2,2);
    Matrix* mat2 = Matrix_New(2,2);
    {
        Matrix_BaseNumber temp[] = {1,2,3,4};
        Matrix_Array(mat1, 2, 2, temp);
        Matrix_BaseNumber temp2[] = {5,6,7,8};
        Matrix_Array(mat2, 2,2,temp2);
    }
    printf("%lf\n", Matrix_Det(mat1));
    Matrix_Print(mat1);
    printf("\n");
    
    Matrix_Print(mat2);
    printf("\n");

    Matrix* resultmat = Matrix_New(2,2);

    Matrix_DotFast(resultmat, mat1, mat2);
    Matrix_Print(resultmat);
    return 0;


    return 0;
}

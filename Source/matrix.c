#include "buildoption.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef BASE_NUMBER_TYPE Matrix_BaseNumber;
#include "FlexirtaM.h"

Matrix* Matrix_Constructer(Matrix* this, const unsigned int width, const unsigned int height) {
    if(this == NULL) {
        return NULL;
    }
    if(width == 0 || height == 0) {
        this->width = 0;
        this->height = 0;
        this->data = NULL;
        this->index = NULL;
        return this;
    }
    this->width = width;
    this->height = height;
    this->data = malloc(sizeof(Matrix_BaseNumber)*width*height);
    this->index = malloc(sizeof(Matrix_BaseNumber*)*height);
    if(this->data == NULL || this->index == NULL) {
        return NULL;
    }
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = 0;
    }
    for(unsigned int i=0; i<height; i++) {
        this->index[i] = this->data+i*this->width;
    }
    return this;
}

Matrix* Matrix_Destructer(Matrix* this) {
    if(this == NULL) return NULL;
    free(this->data);
    free(this->index);
    this->data = NULL;
    this->index = NULL;
    this->width = 0;
    this->height = 0;
    return this;
}

Matrix* Matrix_New(const unsigned int width, const unsigned int height) {
    Matrix* result = malloc(sizeof(Matrix));
    return Matrix_Constructer(result, width, height);
}

void Matrix_Delete(Matrix* this) {
    if(this == NULL) return;
    Matrix_Destructer(this);
    free(this);
}

Matrix* Matrix_Reset(Matrix* this, const unsigned int width, const unsigned int height) {
    if(!(this->width == width && this->height == height)) {
        Matrix_Destructer(this);
        Matrix_Constructer(this, width, height);
    }else {
        for(unsigned int i=0; i<width*height; i++) {
            this->data[i] = 0;
        }
    }
    return this;
}

Matrix* Matrix_Array(Matrix* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber* array) {
    Matrix_Reset(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = array[i];
    }
    return this;
}

Matrix* Matrix_Copy(Matrix* this, const Matrix* from) {
    if(this == NULL || from == NULL) return NULL;
    Matrix_Destructer(this);
    Matrix_Constructer(this, from->width, from->height);
    for(unsigned int i=0; i<from->width*from->height; i++) {
        this->data[i] = from->data[i];
    }
    return this;
}

Matrix_BaseNumber* Matrix_Element(Matrix* this, const unsigned int x, const unsigned int y) {
    if(!(x < this->width && y < this->height) || (this == NULL)) {
        return NULL;
    }
    return (this->data + x+y*this->width);
}

Matrix_BaseNumber** Matrix_(Matrix* this) {
    if(this == NULL) return NULL;
    return this->index;
}

Matrix* Matrix_Ones(Matrix* this, const unsigned int width, const unsigned int height) {
    return Matrix_Fill(this, width, height, 1);
}

Matrix* Matrix_Fill(Matrix* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber number) {
    if(!(this->width == width && this->height == height)) {
        Matrix_Destructer(this);
        Matrix_Constructer(this, width, height);
    }
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = number;
    }
    return this;
}

Matrix* Matrix_Eye(Matrix* this, const unsigned int size) {
    if(this == NULL) return NULL;
    Matrix_Reset(this, size, size);
    for(unsigned int i=0; i<size; i++) {
        this->data[i*(1+this->width)] = 1;
    }
    return this;
}

Matrix* Matrix_Random(Matrix* this, const unsigned int width, const unsigned int height) {
    if(this == NULL) return NULL;
    Matrix_Reset(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = Matrix_Math_Random();
    }
    return this;
}

Matrix* Matrix_SignedRandom(Matrix* this, const unsigned int width, const unsigned int height) {
    if(this == NULL) return NULL;
    Matrix_Reset(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = Matrix_Math_Random()*2-1;
    }
    return this;
}

Matrix* Matrix_Arrange(Matrix* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber start, const Matrix_BaseNumber increase) {
    if(this == NULL) return NULL;
    Matrix_Reset(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = start + increase*i;
    }
    return this;
}

void Matrix_Print(const Matrix* this) {
    unsigned int buffsize = 0;
    Matrix_OutStr(this, NULL, &buffsize);
    char* buff = malloc(sizeof(char)*buffsize);

    if(Matrix_OutStr(this, buff, &buffsize) == NULL) return;
    printf("%s", buff);

    free(buff);
    return;
}

char* Matrix_OutStr(const Matrix* this, char* buff, unsigned int* buffsize) {
    if(this == NULL || buffsize == NULL) return NULL;
    
    char tempChar[16] = "";
    unsigned int neededsize = 1;
    if(this->width != 0 && this->height != 0) {
        neededsize += 2*this->height;
        for(unsigned int i=0; i<this->width*this->height; i++) {
            neededsize += 3;
            sprintf(tempChar, BASE_NUMBER_CONVERT_OPARATER, this->data[i]);
            for(int k=0; k<sizeof(tempChar); k++) {
                if(tempChar[k] == '\0') {
                    neededsize += k;
                    break;
                }
            }

        }
    }else {
        neededsize = 3;
    }
    if(*buffsize < neededsize) {
        *buffsize = neededsize;
        return NULL;
    }
    if(buff == NULL) return NULL;

    if(this->width == 0 || this->height == 0) {
        sprintf(buff, "{}");
        return buff;
    }

    buff[0] = '\0';
    for(unsigned int i=0; i<this->height; i++) {
        strcat(buff, "{ ");
        for(unsigned int k=0; k<this->width; k++) {
            if(k != 0) {
                strcat(buff, ", ");
            }
            tempChar[0] = '\0';
            sprintf(tempChar, BASE_NUMBER_CONVERT_OPARATER, this->data[k+i*this->width]);
            strcat(buff, tempChar);
        }
        strcat(buff, " }\n");
    }
    return buff;
}

Matrix* Matrix_Add(Matrix* this, const Matrix* matrix) {
    if(this == NULL || matrix == NULL) return this;

    if(this->width == matrix->width && this->height == matrix->height) {
        for(unsigned int i=0; i<matrix->width*matrix->height; i++) {
            this->data[i] += matrix->data[i];
        }
        return this;
    }

    Matrix thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Reset(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] + matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Destructer(&thiscopy);
    return this;
}

Matrix* Matrix_AddFast(Matrix* this, const Matrix* mat1, const Matrix* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Reset(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] + mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Reset(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] + mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix* Matrix_Sub(Matrix* this, const Matrix* matrix) {
    if(this == NULL || matrix == NULL) return this;
    Matrix thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Reset(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] - matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Destructer(&thiscopy);
    return this;
}

Matrix* Matrix_SubFast(Matrix* this, const Matrix* mat1, const Matrix* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Reset(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] - mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Reset(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] - mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix* Matrix_Mul(Matrix* this, const Matrix* matrix) {
    if(this == NULL || matrix == NULL) return this;
    Matrix thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Reset(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] * matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Destructer(&thiscopy);
    return this;
}

Matrix* Matrix_MulFast(Matrix* this, const Matrix* mat1, const Matrix* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Reset(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] * mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Reset(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] * mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix* Matrix_Div(Matrix* this, const Matrix* matrix) {
    if(this == NULL || matrix == NULL) return this;
    Matrix thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Reset(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] / matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Destructer(&thiscopy);
    return this;
}

Matrix* Matrix_DivFast(Matrix* this, const Matrix* mat1, const Matrix* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Reset(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] / mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Reset(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] / mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix* Matrix_Dot(Matrix* this, const Matrix* matrix) {
    if(this == NULL || matrix == NULL || this->width != matrix->height) {
        return NULL;
    }
    Matrix thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;
    
    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Reset(this, matrix->width, thiscopy.height);

    Matrix_BaseNumber* targetElement = NULL;
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            targetElement = &(this->index[i][k]);
            for(unsigned int j=0; j<matrix->height; j++) {
                *targetElement += thiscopy.index[i][j]*matrix->index[j][k];
            }
        }
    }

    Matrix_Destructer(&thiscopy);    
    return this;
}

Matrix* Matrix_DotFast(Matrix* this, const Matrix* mat1, const Matrix* mat2) {
    if(this == NULL || mat1 == NULL || mat2 == NULL || mat1->width != mat2->height) return NULL;

    Matrix_Reset(this, mat2->width, mat1->height);
    Matrix_BaseNumber* targetElement = NULL;
    for(unsigned int i=0; i<mat1->height; i++) {
        for(unsigned int k=0; k<mat2->width; k++) {
            targetElement = &(this->data[k+i*mat2->width]);
            for(unsigned int j=0; j<mat1->width; j++) {
                *targetElement += mat1->data[j+i*mat1->width] * mat2->data[k+j*mat2->width];
            }
        }
    }
    
    return this;
}

Matrix* Matrix_Transpose(Matrix* this) {
    if(this == NULL) return NULL;

    Matrix thiscopy;
    thiscopy = *this;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Reset(this, thiscopy.height, thiscopy.width);

    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->index[i][k] = thiscopy.index[k][i];
        }
    }

    Matrix_Destructer(&thiscopy);
    return this;
}

Matrix* Matrix_Inverse(Matrix* this) {
    //余因子行列を使って逆行列を計算する
    if(this->width != this->height || this == NULL) return NULL;

    Matrix_BaseNumber det_this = Matrix_Det(this);
    if(det_this == 0) return NULL;

    Matrix_CofacterMatrix(this);
Matrix_Print(this);
    for(unsigned int i=0; i<this->width*this->height; i++) {
        this->data[i] /= det_this;
    }

    return this;
}

Matrix* Matrix_MinorDeterminant(Matrix* this, const unsigned int x, const unsigned int y) {
    if(this == NULL || this->width<=1 || this->height<=1) return NULL;

    Matrix thiscopy;
    thiscopy = *this;
    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;

    Matrix_Reset(this, thiscopy.width-1, thiscopy.height-1);
    for(unsigned int i=0; i<y; i++) {
        for(unsigned int k=0; k<x; k++)
            this->index[i][k] = thiscopy.index[i][k];
        for(unsigned int k=x+1; k<thiscopy.width; k++)
            this->index[i][k-1] = thiscopy.index[i][k];
    }
    for(unsigned int i=y+1; i<thiscopy.height; i++) {
        for(unsigned int k=0; k<x; k++)
            this->index[i-1][k] = thiscopy.index[i][k];
        for(unsigned int k=x+1; k<thiscopy.width; k++)
            this->index[i-1][k-1] = thiscopy.index[i][k];
    }
    Matrix_Destructer(&thiscopy);
    return this;
}

Matrix_BaseNumber Matrix_Cofacter(const Matrix* this, const unsigned x, const unsigned y) {
    if(this->width <= x || this->height <= y) return 0;
    Matrix thiscopy;
    Matrix_Constructer(&thiscopy, 0, 0);
    Matrix_Copy(&thiscopy, this);

    Matrix_MinorDeterminant(&thiscopy, x, y);
    Matrix_BaseNumber result = pow(-1, x+y)*Matrix_Det(&thiscopy);

    Matrix_Destructer(&thiscopy);
    return result;
}

Matrix* Matrix_CofacterMatrix(Matrix* this) {
    Matrix thiscopy = *this;
    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;

    Matrix_Reset(this, thiscopy.height, thiscopy.width);
    for(unsigned int i=0; i<thiscopy.height; i++) {
        for(unsigned int k=0; k<thiscopy.width; k++) {
            this->index[k][i] = Matrix_Cofacter(&thiscopy, k, i);
        }
    }

    Matrix_Destructer(&thiscopy);
    return this;
}

Matrix_BaseNumber Matrix_Det(const Matrix* this) {
    if(this == NULL || this->width != this->height) return 0;
    Matrix tempMatrix;
    Matrix_Constructer(&tempMatrix, 0, 0);

    Matrix_BaseNumber result = 0;
    Matrix_BaseNumber tempMatrix_BaseNumber = 1;

    unsigned long long times = Matrix_Math_Factorial(this->width);
    for(unsigned long long i=0; i<times; i++) {
        Matrix_Sigma(&tempMatrix, this->width, i);
        tempMatrix_BaseNumber = Matrix_Sgn(&tempMatrix);
        for(unsigned int k=0; k<this->width; k++) {
            tempMatrix_BaseNumber *= (this->index)[k][(int)(tempMatrix.data[k]-1)];
        }
        result += tempMatrix_BaseNumber;
    }

    Matrix_Destructer(&tempMatrix);

    return result;
}

Matrix* Matrix_Sigma(Matrix* this, const unsigned int width, const unsigned int sigmaIndex) {
    if(this == NULL || width == 0) return NULL;
    Matrix_Reset(this, width, 1);

    unsigned int sigmaIndexCopy = sigmaIndex;
    int statusList[width];
    for(unsigned int i=0; i<width; i++) statusList[i]=1;
    for(int i=width-1; 0<=i; i--) {
        unsigned int indexOfStatusList = 0;
        unsigned int targetIndexOfStatusList = sigmaIndexCopy/Matrix_Math_Factorial(i);
        for(unsigned int k=0;; k++) {
            if(!statusList[k]) continue;
            if(indexOfStatusList == targetIndexOfStatusList) {
                statusList[k] = 0;
                this->data[width-1-i] = k+1;
                break;
            }
            if(statusList[k]) indexOfStatusList++;
        }
        sigmaIndexCopy %= Matrix_Math_Factorial(i);
    }
    
    return this;
}

Matrix_BaseNumber Matrix_Sgn(const Matrix* this) {
    if(this == NULL || this->height != 1) return 0;

    unsigned int count = 0;

    Matrix thiscopy;
    Matrix_Constructer(&thiscopy, 0, 0);
    Matrix_Copy(&thiscopy, this);

    for(unsigned int i=1; i<=thiscopy.width; i++) {
        int isTrueFlug = 0;
        for(unsigned int k=i-1; k<thiscopy.width; k++) {
            if(thiscopy.data[k] == i) {
                isTrueFlug = 1;
                if(thiscopy.data[i-1] != i) {
                    count++;
                    thiscopy.data[k] = thiscopy.data[i-1];
                    thiscopy.data[i-1] = i;
                }
                break;
            }
        }
        if(!isTrueFlug) {
            Matrix_Destructer(&thiscopy);
            return 0;
        }
    }

    Matrix_Destructer(&thiscopy);
    return (count%2)?(-1):(1);
}

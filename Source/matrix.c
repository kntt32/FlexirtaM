#include "buildoption.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "flexirtam_include.h"

Matrix_Struct* Matrix_Method(Constructer)(Matrix_Struct* this, const unsigned int width, const unsigned int height) {
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

Matrix_Struct* Matrix_Method(Destructer)(Matrix_Struct* this) {
    if(this == NULL) return NULL;
    free(this->data);
    free(this->index);
    this->data = NULL;
    this->index = NULL;
    this->width = 0;
    this->height = 0;
    return this;
}

Matrix_Struct* Matrix_Method(New)(const unsigned int width, const unsigned int height) {
    Matrix_Struct* result = malloc(sizeof(Matrix_Struct));
    return Matrix_Method(Constructer)(result, width, height);
}

void Matrix_Method(Delete)(Matrix_Struct* this) {
    if(this == NULL) return;
    Matrix_Method(Destructer)(this);
    free(this);
}

Matrix_Struct* Matrix_Method(Reset)(Matrix_Struct* this, const unsigned int width, const unsigned int height) {
    if(!(this->width == width && this->height == height)) {
        Matrix_Method(Destructer)(this);
        Matrix_Method(Constructer)(this, width, height);
    }else {
        for(unsigned int i=0; i<width*height; i++) {
            this->data[i] = 0;
        }
    }
    return this;
}

Matrix_Struct* Matrix_Method(Array)(Matrix_Struct* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber* array) {
    Matrix_Method(Reset)(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = array[i];
    }
    return this;
}

Matrix_Struct* Matrix_Method(Copy)(Matrix_Struct* this, const Matrix_Struct* from) {
    if(this == NULL || from == NULL) return NULL;
    Matrix_Method(Destructer)(this);
    Matrix_Method(Constructer)(this, from->width, from->height);
    for(unsigned int i=0; i<from->width*from->height; i++) {
        this->data[i] = from->data[i];
    }
    return this;
}

Matrix_BaseNumber* Matrix_Method(Element)(Matrix_Struct* this, const unsigned int x, const unsigned int y) {
    if(!(x < this->width && y < this->height) || (this == NULL)) {
        return NULL;
    }
    return (this->data + x+y*this->width);
}

Matrix_BaseNumber** Matrix_Method()(Matrix_Struct* this) {
    if(this == NULL) return NULL;
    return this->index;
}

Matrix_Struct* Matrix_Method(Ones)(Matrix_Struct* this, const unsigned int width, const unsigned int height) {
    return Matrix_Method(Fill)(this, width, height, 1);
}

Matrix_Struct* Matrix_Method(Fill)(Matrix_Struct* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber number) {
    if(!(this->width == width && this->height == height)) {
        Matrix_Method(Destructer)(this);
        Matrix_Method(Constructer)(this, width, height);
    }
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = number;
    }
    return this;
}

Matrix_Struct* Matrix_Method(Eye)(Matrix_Struct* this, const unsigned int size) {
    if(this == NULL) return NULL;
    Matrix_Method(Reset)(this, size, size);
    for(unsigned int i=0; i<size; i++) {
        this->data[i*(1+this->width)] = 1;
    }
    return this;
}

Matrix_Struct* Matrix_Method(Random)(Matrix_Struct* this, const unsigned int width, const unsigned int height) {
    if(this == NULL) return NULL;
    Matrix_Method(Reset)(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = Matrix_Method(Math_Random)();
    }
    return this;
}

Matrix_Struct* Matrix_Method(SignedRandom)(Matrix_Struct* this, const unsigned int width, const unsigned int height) {
    if(this == NULL) return NULL;
    Matrix_Method(Reset)(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = Matrix_Method(Math_Random)()*2-1;
    }
    return this;
}

Matrix_Struct* Matrix_Method(Arrange)(Matrix_Struct* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber start, const Matrix_BaseNumber increase) {
    if(this == NULL) return NULL;
    Matrix_Method(Reset)(this, width, height);
    for(unsigned int i=0; i<width*height; i++) {
        this->data[i] = start + increase*i;
    }
    return this;
}

void Matrix_Method(Print)(const Matrix_Struct* this) {
    unsigned int buffsize = 0;
    Matrix_Method(OutStr)(this, NULL, &buffsize);
    char* buff = malloc(sizeof(char)*buffsize);

    if(Matrix_Method(OutStr)(this, buff, &buffsize) == NULL) return;
    printf("%s", buff);

    free(buff);
    return;
}

char* Matrix_Method(OutStr)(const Matrix_Struct* this, char* buff, unsigned int* buffsize) {
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

Matrix_Struct* Matrix_Method(Add)(Matrix_Struct* this, const Matrix_Struct* matrix) {
    if(this == NULL || matrix == NULL) return this;

    if(this->width == matrix->width && this->height == matrix->height) {
        for(unsigned int i=0; i<matrix->width*matrix->height; i++) {
            this->data[i] += matrix->data[i];
        }
        return this;
    }

    Matrix_Struct thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Method(Reset)(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] + matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Method(Destructer)(&thiscopy);
    return this;
}

Matrix_Struct* Matrix_Method(AddFast)(Matrix_Struct* this, const Matrix_Struct* mat1, const Matrix_Struct* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Method(Reset)(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] + mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Method(Reset)(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] + mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix_Struct* Matrix_Method(Sub)(Matrix_Struct* this, const Matrix_Struct* matrix) {
    if(this == NULL || matrix == NULL) return this;
    Matrix_Struct thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Method(Reset)(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] - matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Method(Destructer)(&thiscopy);
    return this;
}

Matrix_Struct* Matrix_Method(SubFast)(Matrix_Struct* this, const Matrix_Struct* mat1, const Matrix_Struct* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Method(Reset)(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] - mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Method(Reset)(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] - mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix_Struct* Matrix_Method(Mul)(Matrix_Struct* this, const Matrix_Struct* matrix) {
    if(this == NULL || matrix == NULL) return this;
    Matrix_Struct thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Method(Reset)(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] * matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Method(Destructer)(&thiscopy);
    return this;
}

Matrix_Struct* Matrix_Method(MulFast)(Matrix_Struct* this, const Matrix_Struct* mat1, const Matrix_Struct* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Method(Reset)(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] * mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Method(Reset)(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] * mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix_Struct* Matrix_Method(Div)(Matrix_Struct* this, const Matrix_Struct* matrix) {
    if(this == NULL || matrix == NULL) return this;
    Matrix_Struct thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Method(Reset)(this, (thiscopy.width > matrix->width)?(thiscopy.width):(matrix->width), (thiscopy.height > matrix->height)?(thiscopy.height):(matrix->height));
    
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->data[k+i*this->width] = thiscopy.data[(k%thiscopy.width)+(i%thiscopy.height)*thiscopy.width] / matrix->data[(k%matrix->width)+(i%matrix->height)*matrix->width];
        }
    }

    Matrix_Method(Destructer)(&thiscopy);
    return this;
}

Matrix_Struct* Matrix_Method(DivFast)(Matrix_Struct* this, const Matrix_Struct* mat1, const Matrix_Struct* mat2) {
    if(mat1->width == mat2->width && mat1->height == mat2->height) {
        Matrix_Method(Reset)(this, mat1->width, mat1->height);
        for(unsigned int i=0; i<mat1->height; i++) {
            for(unsigned int k=0; k<mat1->width; k++) {
                this->data[k+i*mat1->width] = mat1->data[k+i*mat1->width] / mat2->data[k+i*mat1->width];
            }
        }
        return this;
    }else {
        Matrix_Method(Reset)(this, (mat1->width > mat2->width)?(mat1->width):(mat2->width), (mat1->height > mat2->height)?(mat1->height):(mat2->height));
        for(unsigned int i=0; i<this->height; i++) {
            for(unsigned int k=0; k<this->width; k++) {
                this->data[k+i*this->width] = mat1->data[(k%mat1->width)+(i%mat1->height)*mat1->width] / mat2->data[(k%mat2->width)+(i%mat2->height)*mat2->width];
            }
        }
    }
}

Matrix_Struct* Matrix_Method(Dot)(Matrix_Struct* this, const Matrix_Struct* matrix) {
    if(this == NULL || matrix == NULL || this->width != matrix->height) {
        return NULL;
    }
    Matrix_Struct thiscopy;
    thiscopy.width = this->width;
    thiscopy.height = this->height;
    thiscopy.data = this->data;
    thiscopy.index = this->index;
    
    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Method(Reset)(this, matrix->width, thiscopy.height);

    Matrix_BaseNumber* targetElement = NULL;
    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            targetElement = &(this->index[i][k]);
            for(unsigned int j=0; j<matrix->height; j++) {
                *targetElement += thiscopy.index[i][j]*matrix->index[j][k];
            }
        }
    }

    Matrix_Method(Destructer)(&thiscopy);    
    return this;
}

Matrix_Struct* Matrix_Method(DotFast)(Matrix_Struct* this, const Matrix_Struct* mat1, const Matrix_Struct* mat2) {
    if(this == NULL || mat1 == NULL || mat2 == NULL || mat1->width != mat2->height) return NULL;

    Matrix_Method(Reset)(this, mat2->width, mat1->height);
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

Matrix_Struct* Matrix_Method(Transpose)(Matrix_Struct* this) {
    if(this == NULL) return NULL;

    Matrix_Struct thiscopy;
    thiscopy = *this;

    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;
    Matrix_Method(Reset)(this, thiscopy.height, thiscopy.width);

    for(unsigned int i=0; i<this->height; i++) {
        for(unsigned int k=0; k<this->width; k++) {
            this->index[i][k] = thiscopy.index[k][i];
        }
    }

    Matrix_Method(Destructer)(&thiscopy);
    return this;
}

Matrix_Struct* Matrix_Method(Inverse)(Matrix_Struct* this) {
    //余因子行列を使って逆行列を計算する
    if(this->width != this->height || this == NULL) return NULL;

    Matrix_BaseNumber det_this = Matrix_Method(Det)(this);
    if(det_this == 0) return NULL;

    Matrix_Method(CofacterMatrix)(this);
    for(unsigned int i=0; i<this->width*this->height; i++) {
        this->data[i] /= det_this;
    }

    return this;
}

Matrix_Struct* Matrix_Method(MinorDeterminant)(Matrix_Struct* this, const unsigned int x, const unsigned int y) {
    if(this == NULL || this->width<=1 || this->height<=1) return NULL;

    Matrix_Struct thiscopy;
    thiscopy = *this;
    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;

    Matrix_Method(Reset)(this, thiscopy.width-1, thiscopy.height-1);
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
    Matrix_Method(Destructer)(&thiscopy);
    return this;
}

Matrix_BaseNumber Matrix_Method(Cofacter)(const Matrix_Struct* this, const unsigned x, const unsigned y) {
    if(this->width <= x || this->height <= y) return 0;
    Matrix_Struct thiscopy;
    Matrix_Method(Constructer)(&thiscopy, 0, 0);
    Matrix_Method(Copy)(&thiscopy, this);

    Matrix_Method(MinorDeterminant)(&thiscopy, x, y);
    Matrix_BaseNumber result = pow(-1, x+y)*Matrix_Method(Det)(&thiscopy);

    Matrix_Method(Destructer)(&thiscopy);
    return result;
}

Matrix_Struct* Matrix_Method(CofacterMatrix)(Matrix_Struct* this) {
    Matrix_Struct thiscopy = *this;
    this->width = 0;
    this->height = 0;
    this->data = NULL;
    this->index = NULL;

    Matrix_Method(Reset)(this, thiscopy.height, thiscopy.width);
    for(unsigned int i=0; i<thiscopy.height; i++) {
        for(unsigned int k=0; k<thiscopy.width; k++) {
            this->index[k][i] = Matrix_Method(Cofacter)(&thiscopy, k, i);
        }
    }

    Matrix_Method(Destructer)(&thiscopy);
    return this;
}

Matrix_BaseNumber Matrix_Method(Det)(const Matrix_Struct* this) {
    if(this == NULL || this->width != this->height) return 0;
    Matrix_Struct tempMatrix;
    Matrix_Method(Constructer)(&tempMatrix, 0, 0);

    Matrix_BaseNumber result = 0;
    Matrix_BaseNumber tempMatrix_BaseNumber = 1;

    unsigned long long times = Matrix_Method(Math_Factorial)(this->width);
    for(unsigned long long i=0; i<times; i++) {
        Matrix_Method(Sigma)(&tempMatrix, this->width, i);
        tempMatrix_BaseNumber = Matrix_Method(Sgn)(&tempMatrix);
        for(unsigned int k=0; k<this->width; k++) {
            tempMatrix_BaseNumber *= (this->index)[k][(int)(tempMatrix.data[k]-1)];
        }
        result += tempMatrix_BaseNumber;
    }

    Matrix_Method(Destructer)(&tempMatrix);

    return result;
}

Matrix_Struct* Matrix_Method(Sigma)(Matrix_Struct* this, const unsigned int width, const unsigned int sigmaIndex) {
    if(this == NULL || width == 0) return NULL;
    Matrix_Method(Reset)(this, width, 1);

    unsigned int sigmaIndexCopy = sigmaIndex;
    int statusList[width];
    for(unsigned int i=0; i<width; i++) statusList[i]=1;
    for(int i=width-1; 0<=i; i--) {
        unsigned int indexOfStatusList = 0;
        unsigned int targetIndexOfStatusList = sigmaIndexCopy/Matrix_Method(Math_Factorial)(i);
        for(unsigned int k=0;; k++) {
            if(!statusList[k]) continue;
            if(indexOfStatusList == targetIndexOfStatusList) {
                statusList[k] = 0;
                this->data[width-1-i] = k+1;
                break;
            }
            if(statusList[k]) indexOfStatusList++;
        }
        sigmaIndexCopy %= Matrix_Method(Math_Factorial)(i);
    }
    
    return this;
}

Matrix_BaseNumber Matrix_Method(Sgn)(const Matrix_Struct* this) {
    if(this == NULL || this->height != 1) return 0;

    unsigned int count = 0;

    Matrix_Struct thiscopy;
    Matrix_Method(Constructer)(&thiscopy, 0, 0);
    Matrix_Method(Copy)(&thiscopy, this);

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
            Matrix_Method(Destructer)(&thiscopy);
            return 0;
        }
    }

    Matrix_Method(Destructer)(&thiscopy);
    return (count%2)?(-1):(1);
}

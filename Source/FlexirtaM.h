#ifndef INCLUDED_MATLIB
#define INCLUDED_MATLIB

//matrix.c
typedef struct {
    unsigned int width;
    unsigned int height;
    Matrix_BaseNumber* data;
    Matrix_BaseNumber** index;
} Matrix;


//構造体の生成/破棄関連
Matrix* Matrix_Constructer(Matrix* this, const unsigned int width, const unsigned int height);//コンストラクタ　下記New以外で確保したものは利用前にこれに通す。しないと未定義動作
Matrix* Matrix_Destructer(Matrix* this);//デストラクタ　コンストラクタに通したものはこれに通す
Matrix* Matrix_New(const unsigned int width, const unsigned int height);//動的領域から構造体を新たに確保する
void Matrix_Delete(Matrix* this);//Matrix_Newで確保した構造体を解放する

//行列の操作関連
Matrix* Matrix_Reset(Matrix* this, const unsigned int width, const unsigned int height);//行列のサイズを変更して0だけにする
Matrix* Matrix_Array(Matrix* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber* array);//行列にMatrix_BaseNumber配列を代入する
Matrix* Matrix_Copy(Matrix* this, const Matrix* from);//コピー
Matrix_BaseNumber* Matrix_Element(Matrix* this, const unsigned int x, const unsigned int y);//行列の要素(x,y)へのポインタを返す 存在しない要素はNULLとする
Matrix_BaseNumber** Matrix_(Matrix* this);//行列の要素に二次元行列としてアクセスする
Matrix* Matrix_Ones(Matrix* this, const unsigned int width, const unsigned int height);//行列を1にする
Matrix* Matrix_Fill(Matrix* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber number);//行列をnumberだけにする
Matrix* Matrix_Eye(Matrix* this, const unsigned int size);//単位行列にする
Matrix* Matrix_Random(Matrix* this, const unsigned int width, const unsigned int height);//0以上1以下でランダムにする
Matrix* Matrix_SignedRandom(Matrix* this, const unsigned int width, const unsigned int height);//-1以上1以下でランダムにする
Matrix* Matrix_Arrange(Matrix* this, const unsigned int width, const unsigned int height, const Matrix_BaseNumber start, const Matrix_BaseNumber increase);//連続した数値を入れる
void Matrix_Print(const Matrix* this);//表示
char* Matrix_OutStr(const Matrix* this, char* buff, unsigned int* buffsize);//文字列出力

//行列計算関連
Matrix* Matrix_Add(Matrix* this, const Matrix* matrix);//各要素の足し算　ブロードキャストあり
Matrix* Matrix_AddFast(Matrix* this, const Matrix* mat1, const Matrix* mat2);//各要素の足し算高速版　ブロードキャストあり
Matrix* Matrix_Sub(Matrix* this, const Matrix* matrix);//各要素の引き算　ブロードキャストあり
Matrix* Matrix_SubFast(Matrix* this, const Matrix* mat1, const Matrix* mat2);//各要素の引き算高速版　ブロードキャストあり
Matrix* Matrix_Mul(Matrix* this, const Matrix* matrix);//各要素の掛け算　ブロードキャストあり
Matrix* Matrix_MulFast(Matrix* this, const Matrix* mat1, const Matrix* mat2);//各要素の掛け算高速版　ブロードキャストあり
Matrix* Matrix_Div(Matrix* this, const Matrix* matrix);//各要素の割り算　ブロードキャストあり
Matrix* Matrix_DivFast(Matrix* this, const Matrix* mat1, const Matrix* mat2);//各要素の割り算高速版　ブロードキャストあり
Matrix* Matrix_Dot(Matrix* this, const Matrix* matrix);//行列の掛け算
Matrix* Matrix_DotFast(Matrix* this, const Matrix* mat1, const Matrix* mat2);//行列の掛け算高速版
Matrix* Matrix_Transpose(Matrix* this);//行列の転置
Matrix* Matrix_Inverse(Matrix* this);//逆行列
Matrix* Matrix_MinorDeterminant(Matrix* this, const unsigned int x, const unsigned int y);//小行列式
Matrix_BaseNumber Matrix_Cofacter(const Matrix* this, const unsigned x, const unsigned y);//余因子
Matrix* Matrix_CofacterMatrix(Matrix* this);//余因子行列
Matrix_BaseNumber Matrix_Det(const Matrix* this);//行列式

//置換関連
Matrix* Matrix_Sigma(Matrix* this, const unsigned int width, const unsigned int sigmaIndex);//置換(σ())
Matrix_BaseNumber Matrix_Sgn(const Matrix* this);//sgn()



//math.c
Matrix_BaseNumber Matrix_Math_Random();//乱数生成 x64のみサポート
unsigned long long Matrix_Math_Factorial(unsigned int number);//階上

#endif

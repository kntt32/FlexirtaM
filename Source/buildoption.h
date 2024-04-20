#ifndef INCLUDED_BUILDOPTION
#define INCLUDED_BUILDOPTION

#define BASE_NUMBER_TYPE double
    //数値の内部表記を設定
#define Matrix_Struct Matrix_##double
    //型の名称
#define Matrix_Method(name) Matrix_##double##_##name
    //メゾッドの名前
#define BASE_NUMBER_CONVERT_OPARATER "%lf"
    //BASE_NUMBER_TYPE型の変換演算子
#define ENABLE_RDRAND 1
    //RDRAND命令を使用する　x64のみサポート
#define ENABLE_MULTITHREAD 1
    //マルチスレッドを使用する未サポート

#endif

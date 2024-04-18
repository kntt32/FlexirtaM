#ifndef INCLUDED_BUILDOPTION
#define INCLUDED_BUILDOPTION

#define BASE_NUMBER_TYPE float
    //数値の内部表記を設定
#define BASE_NUMBER_CONVERT_OPARATER "%f"
    //BASE_NUMBER_TYPE型の変換演算子
#define ENABLE_RDRAND 1
    //RDRAND命令を使用する　x64のみサポート
#define ENABLE_MULTITHREAD 1
    //マルチスレッドを使用する未サポート

#endif

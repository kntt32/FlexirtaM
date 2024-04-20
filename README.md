# FlexirtaM  
自作の行列計算ライブラリである  
`matrix.c`に行列関連のコードを、`math.c`に一般的な数学関連のコードを配置している  
コンパイル時のバイナリ設定は`buildoption.h`内のマクロで行う  


## 方針  
優先順に、  
1. 統一された読みやすいコードであること
2. 不具合がないこと
3. プラットフォーム非依存であること
   ただしパフォーマンスを上げる場合、プラットフォーム非依存な実装と高速な実装を、`buildoption.h`で選択できるようにし、互換性を保つこと
4. 高速であること
5. 数学的な用途において十分多機能であること


## 依存ライブラリ  
C言語のホスト環境標準ライブラリのみに依存している


## 使い方  
ビルドは`make buildlib`で行う  

このライブラリを利用するには、
- ソース中で`FlexirtaM.h`をインクルード
- `FlexirtaM.h`をインクルードする前にいくつかのマクロを定義 (以下カッコ内を`buildoption.h`設定に置き換え)  
   - `#define Matrix_BaseNumber (BASE_NUMBER_TYPE)`
   - `#define Matrix_Struct Matrix_##(BASE_NUMBER_TYPE)`
   - `#define Matrix_Method(name) Matrix_##(BASE_NUMBER_TYPE)##_##name`
- `FlexirtaM.a`のリンク  

が必要である  

また、`buildoption.h`内の`BASE_NUMBER_TYPE`によって関数名や型名が変化するため、数種類の`BASE_NUMBER_TYPE`のFlexirtaMを同時に使用できる

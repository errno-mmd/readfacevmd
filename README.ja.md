# readfacevmd

Readfacevmd は写真や動画から人の顔の表情を読み取って、MikuMikuDanceで使えるVMDフォーマットのモーションファイルを生成します。
Facial landmark detection (顔器官検出)、顔の各部分の動きの認識、頭の向きの認識にはOpenFace APIを使用します。

## 必要なもの

予め下記のライブラリをインストールする必要があります。

- [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)
- boost
- TBB
- dlib
- OpenBLAS
- OpenCV
- [ICU](http://site.icu-project.org/) - International Components for Unicode
- [Eigen](http://eigen.tuxfamily.org/)

注： OpenFaceのインストールの最後で 'make install' を行ってください。
OpenFaceのライブラリが /usr/local/lib に、ヘッダファイルが /usr/local/include/OpenFace
に格納されている必要があります。
他のディレクトリにインストールした場合は、それに合わせて CMakeLists.txt を変更してください。

また、ビルドには下記のツールが必要です。

- CMake
- GNU C++ Compiler

## 動作確認済み環境

Ubuntu Linux 17.10

## ビルド方法

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## 使い方
```
$ readfacevmd IMAGE_FILE VMD_FILE
```
上記コマンドを実行すると、readfacevmd は IMAGE_FILE を読んで表情を認識し、
VMD_FILE にモーションデータを出力します。

IMAGE_FILE は画像ファイルと動画ファイルの両方に対応しています。

処理が成功したら、VMD_FILE には表情のキーフレームと頭のモーションのキーフレームが
VMDフォーマットで出力されます。

## ライセンス
** 商用利用は禁止です。理由はOpenFaceおよび学習に用いられたデータセットの商用利用が禁止されているためです。 **

Readfacevmd自体はMITライセンスです。詳しくは [LICENSE](LICENSE) ファイルを参照してください。
ただし、ビルドしてできた実行ファイルに関しては各ライブラリのライセンスにも従う必要があります。
特に OpenFace は商用ライセンスを入手しない限り、商用利用ができません。
また、OpenFaceが学習に使っているデータセットのライセンスにも注意してください。
iBUG 300-W 等のデータセットは商用利用が禁止されています。
OpenFaceが使用しているデータセットについては https://github.com/TadasBaltrusaitis/OpenFace/wiki/Datasets
を参照してください。
もし利用者がOpenFaceの商用ライセンスを入手したうえでデータセットの権利関係もクリアした場合は、この限りではありません。

## 参考文献

Readfacevmd の機能は [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)
に強く依存しており、実現できたのは OpenFace toolkit が公開されているおかげです。
OpenFaceに関する詳細は下記の文献を参照してください。

**OpenFace 2.0: Facial Behavior Analysis Toolkit** :
Tadas Baltrušaitis, Amir Zadeh, Yao Chong Lim, and Louis-Philippe Morency,
IEEE International Conference on Automatic Face and Gesture Recognition, 2018

**Convolutional experts constrained local model for facial landmark detection** :
A. Zadeh, T. Baltrušaitis, and Louis-Philippe Morency.
Computer Vision and Pattern Recognition Workshops, 2017

**Constrained Local Neural Fields for robust facial landmark detection in the wild** :
Tadas Baltrušaitis, Peter Robinson, and Louis-Philippe Morency.
in IEEE Int. Conference on Computer Vision Workshops, 300 Faces in-the-Wild Challenge, 2013.

**Rendering of Eyes for Eye-Shape Registration and Gaze Estimation Erroll Wood** :
Tadas Baltrušaitis, Xucong Zhang, Yusuke Sugano, Peter Robinson, and Andreas Bulling in IEEE International. Conference on Computer Vision (ICCV), 2015

**Cross-dataset learning and person-specific normalisation for automatic Action Unit detection** :
Tadas Baltrušaitis, Marwa Mahmoud, and Peter Robinson in Facial Expression Recognition and Analysis Challenge,
IEEE International Conference on Automatic Face and Gesture Recognition, 2015

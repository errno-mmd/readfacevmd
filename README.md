# readfacevmd

Readfacevmd reads facial expression from photo / movie and generate a VMD motion file for MikuMikuDance. This tool utilizes the OpenFace APIs to detect facial landmarks,
to recognize Facial Action Units and to estimate a head pose.

Japanese version of this README: [README.ja.md](README.ja.md)

## Prerequirement

You need to install the following libraries to build / run readfacevmd.
- [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace)
- boost
- dlib
- OpenBLAS
- OpenCV
- [ICU](http://site.icu-project.org/) - International Components for Unicode
- [Eigen](http://eigen.tuxfamily.org/)

Note: You need to do 'make install' in the end of the OpenFace installation.
Make sure the OpenFace libraries and headers are installed into /usr/local/lib
and /usr/local/include/OpenFace, respectively.
Otherwise you need to modify CMakeLists.txt.

You also need the following tools to build readfacevmd.

- CMake
- GNU C++ Compiler

## How to build

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Usage
```
$ readfacevmd IMAGE_FILE VMD_FILE
```
Readfacevmd reads IMAGE_FILE, recognize facial expressions, then writes the motion data
to VMD_FILE.

IMAGE_FILE may be a photo image file or a movie file.

If the process finished successfully, VMD_FILE may contain some facial expression motion key frames and a head pose motion key frame in the VMD format.

## License
** You may not use readfacevmd commercially, because of restrictions of OpenFace and some datasets. **

Readfacevmd itself is under the MIT license. See [LICENSE](LICENSE) file for more details.

You have to respect OpenFace, boost, dlib, OpenBLAS, OpenCV, ICU, and Eigen licenses
when you use readfacevmd linked to those libraries.
Especially, commercial use of OpenFace is not allowed unless you have a commercial license of OpenFace toolkit.
You also have to respect the licenses of the datasets used for OpenFace's model training - https://github.com/TadasBaltrusaitis/OpenFace/wiki/Datasets

## References

This tool heavily depends on [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace),
an open source facial behavior analysis toolkit.
Please refer the following works for the details of OpenFace.

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

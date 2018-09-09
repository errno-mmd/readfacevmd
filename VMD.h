// -*- C++ -*-
// VMDデータ構造とVMDファイルinput/output機能を持つVMD classを定義する

#ifndef VMD_H
#define VMD_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <vector>

using std::ifstream;
using std::ofstream;
using std::vector;
using namespace Eigen;

// VMDファイルヘッダ
class VMD_Header {
public:
  VMD_Header() : version(""), modelname{'\0'} { }
  void read(ifstream& s);
  void write(ofstream& s);

  static const int version_len = 30;
  static const int modelname_len = 20;

  char version[version_len];   // VMD バージョン文字列
  char modelname[modelname_len];
};

// ボーンキーフレーム
class VMD_Frame {
public:
  VMD_Frame() : bonename(""), number(0), position(Vector3f::Zero()), rotation(Quaternionf::Identity()), interpolation{0} { }
  void read(ifstream& s);
  void write(ofstream& s);
  bool operator<(const VMD_Frame& right) const {
    return number < right.number;
  }
  
  static const int bonename_len = 15;
  static const int interpolation_len = 64;
  
  char bonename[bonename_len];  // ボーン名
  uint32_t number;   // frame number
  Vector3f position; // 位置(x, y, z)
  Quaternionf rotation; // 回転を表すクォータニオン
  uint8_t interpolation[interpolation_len];       // interpolation parameter
};

// 表情キーフレーム
class VMD_Morph {
public:
  VMD_Morph() : name(""), frame(0), weight(0) { }
  void read(ifstream& s);
  void write(ofstream& s);
  bool operator<(const VMD_Morph& right) const {
    return frame < right.frame;
  }

  static const int name_len = 15;
  char name[name_len];
  uint32_t frame;
  float weight;
};

// Camera
class VMD_Camera {
public:
  VMD_Camera() : frame(0), distance(0), position(Vector3f::Zero()), rotation(Vector3f::Zero()), interpolation{0},
		 view_angle(0), perspective(0) { }
  void read(ifstream& s);
  void write(ofstream& s);

  static const int interpolation_len = 24;

  uint32_t frame;
  float distance;
  Vector3f position;
  Vector3f rotation;
  uint8_t interpolation[interpolation_len];
  uint32_t view_angle;
  uint8_t perspective;
};

// Light
class VMD_Light {
public:
  VMD_Light() : frame(0), rgb(Vector3f::Zero()), position(Vector3f::Zero()) { }
  void read(ifstream& s);
  void write(ofstream& s);

  uint32_t frame;
  Vector3f rgb;
  Vector3f position;
};

// Self Shadow
class VMD_SelfShadow {
public:
  VMD_SelfShadow() : frame(0), mode(0), distance(0) { }
  void read(ifstream& s);
  void write(ofstream& s);

  uint32_t frame;
  uint8_t mode;
  float distance;
};

// IK on/off
class VMD_IkOnOff {
public:
  VMD_IkOnOff() : name(""), on_off(0) { }
  void read(ifstream& s);
  void write(ofstream& s);

  static const int name_len = 20;

  char name[name_len];
  uint8_t on_off;
};

// モデル表示・IK on/off
class VMD_ShowIk {
public:
  VMD_ShowIk() : frame(0), show(0), ik_on_off(0) { }
  void read(ifstream& s);
  void write(ofstream& s);

  uint32_t frame;
  uint8_t show;
  vector<VMD_IkOnOff> ik_on_off;
};

class VMD {
public:
  VMD() : header(), frame(0), morph(0), camera(0), light(0),
	  self_shadow(0), show_ik(0) {}
  int input(ifstream& s);
  int output(ofstream& s);
  int dump(ofstream& s);

  VMD_Header header;
  vector<VMD_Frame> frame;
  vector<VMD_Morph> morph;
  vector<VMD_Camera> camera;
  vector<VMD_Light> light;
  vector<VMD_SelfShadow> self_shadow;
  vector<VMD_ShowIk> show_ik;
private:
};

#endif // ifndef VMD_H

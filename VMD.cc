#include <Eigen/Core>
#include <Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <string>
#include "MMDFileIOUtil.h"
#include "VMD.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::to_string;

using namespace std;
using namespace Eigen;
using namespace MMDFileIOUtil;

void VMD_Header::read(ifstream& s)
{
  readCharArray(s, version, version_len);
  readCharArray(s, modelname, modelname_len);
}

void VMD_Frame::read(ifstream& s)
{
  //  cout << "VMD_Frame::read" << endl;
  readCharArray(s, bonename, bonename_len);
  //  cout << "bonename: " << bonename << endl;
  readUint32(s, number);
  //  cout << "frame number: " << number << endl;
  readFloat3(s, position);
  readQuaternionf(s, rotation);
  for (int i = 0; i < interpolation_len; i++) {
    readUint8(s, interpolation[i]);
  }
}

void VMD_Morph::read(ifstream& s)
{
  readCharArray(s, name, name_len);
  readUint32(s, frame);
  readFloat(s, weight);
}

void VMD_Camera::read(ifstream& s)
{
  readUint32(s, frame);
  readFloat(s, distance);
  readFloat3(s, position);
  readFloat3(s, rotation);
  for (int i = 0; i < interpolation_len; i++) {
    readUint8(s, interpolation[i]);
  }
  readUint32(s, view_angle);
  readUint8(s, perspective);
}

void VMD_Light::read(ifstream& s)
{
  readUint32(s, frame);
  readFloat3(s, rgb);
  readFloat3(s, position);
}

void VMD_SelfShadow::read(ifstream& s)
{
  readUint32(s, frame);
  readUint8(s, mode);
  readFloat(s, distance);
}

void VMD_IkOnOff::read(ifstream& s)
{
  readCharArray(s, name, name_len);
  readUint8(s, on_off);
}

void VMD_ShowIk::read(ifstream& s)
{
  readUint32(s, frame);
  readUint8(s, show);
  int ik_num;
  readInt(s, ik_num);
  ik_on_off.clear();
  for (int i = 0; i < ik_num; i++) {
    VMD_IkOnOff onoff;
    onoff.read(s);
    ik_on_off.push_back(onoff);
  }
}

int VMD::input(ifstream& s)
{
  s.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);
  try {
    header.read(s);

    cout << "version:" << header.version << endl;
    cout << "modelname:" << header.modelname << endl;
    
    int frame_num;
    readInt(s, frame_num);
    cout << "frame_num: " << frame_num << endl;
    frame.clear();
    for (int i = 0; i < frame_num; i++) {
      //      cout << i << endl;
      VMD_Frame f;
      f.read(s);
      frame.push_back(f);
    }

    int morph_num;
    readInt(s, morph_num);
    cout << "morph_num: " << morph_num << endl;
    morph.clear();
    for (int i = 0; i < morph_num; i++) {
      VMD_Morph tmp;
      tmp.read(s);
      morph.push_back(tmp);
    }

    int camera_num;
    readInt(s, camera_num);
    cout << "camera_num:" << camera_num << endl;
    camera.clear();
    for (int i = 0; i < camera_num; i++) {
      VMD_Camera tmp;
      tmp.read(s);
      camera.push_back(tmp);
    }

    int light_num;
    readInt(s, light_num);
    light.clear();
    for (int i = 0; i < light_num; i++) {
      VMD_Light tmp;
      tmp.read(s);
      light.push_back(tmp);
    }

    int self_shadow_num;
    readInt(s, self_shadow_num);
    self_shadow.clear();
    for (int i = 0; i < self_shadow_num; i++) {
      VMD_SelfShadow tmp;
      tmp.read(s);
      self_shadow.push_back(tmp);
    }

    int show_ik_num;
    readInt(s, show_ik_num);
    cout << "show_ik_num" << show_ik_num << endl;
    show_ik.clear();
    for (int i = 0; i < show_ik_num; i++) {
      VMD_ShowIk tmp;
      tmp.read(s);
      show_ik.push_back(tmp);
    }

  } catch (ifstream::failure& e) {
    std::cerr << "VMD file read error\n";
    return -1;
  }

  cout << "input end" << endl;
  return 0;
}

void VMD_Header::write(ofstream& s)
{
  writeCharArray(s, version, version_len);
  writeCharArray(s, modelname, modelname_len);
}

void VMD_Frame::write(ofstream& s)

{
  writeCharArray(s, bonename, bonename_len);
  writeUint32(s, number);
  writeFloat3(s, position);
  writeQuaternionf(s, rotation);
  for (int i = 0; i < interpolation_len; i++) {
    writeUint8(s, interpolation[i]);
  }
}

void VMD_Morph::write(ofstream& s)
{
  writeCharArray(s, name, name_len);
  writeUint32(s, frame);
  writeFloat(s, weight);
}

void VMD_Camera::write(ofstream& s)
{
  writeUint32(s, frame);
  writeFloat(s, distance);
  writeFloat3(s, position);
  writeFloat3(s, rotation);
  for (int i = 0; i < interpolation_len; i++) {
    writeUint8(s, interpolation[i]);
  }
  writeUint32(s, view_angle);
  writeUint8(s, perspective);
}

void VMD_Light::write(ofstream& s)
{
  writeUint32(s, frame);
  writeFloat3(s, rgb);
  writeFloat3(s, position);
}

void VMD_SelfShadow::write(ofstream& s)
{
  writeUint32(s, frame);
  writeUint8(s, mode);
  writeFloat(s, distance);
}

void VMD_IkOnOff::write(ofstream& s)
{
  writeCharArray(s, name, name_len);
  writeUint8(s, on_off);
}

void VMD_ShowIk::write(ofstream& s)
{
  writeUint32(s, frame);
  writeUint8(s, show);
  writeInt(s, ik_on_off.size());
  for (uint32_t i = 0; i < ik_on_off.size(); i++) {
    ik_on_off[i].write(s);
  }
}
 
int VMD::output(ofstream& s)
{
  //  cout << "output start" << endl;
  header.write(s);

  // cout << "frame number: " << frame.size() << endl;
  writeInt(s, frame.size());
  for (size_t i = 0; i < frame.size(); i++) {
    frame[i].write(s);
  }

  writeInt(s, morph.size());
  for (size_t i = 0; i < morph.size(); i++) {
    morph[i].write(s);
  }

  writeInt(s, camera.size());
  for (size_t i = 0; i < camera.size(); i++) {
    camera[i].write(s);
  }

  writeInt(s, light.size());
  for (size_t i = 0; i < light.size(); i++) {
    light[i].write(s);
  }

  writeInt(s, self_shadow.size());
  for (size_t i = 0; i < self_shadow.size(); i++) {
    self_shadow[i].write(s);
  }

  writeInt(s, show_ik.size());
  for (size_t i = 0; i < show_ik.size(); i++) {
    show_ik[i].write(s);
  }
  
  // cout << "output end" << endl;
  return 0;
}

// X軸方向移動の補間パラメータをセットする
void VMD_Frame::set_interpolation_x(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t* ip = interpolation;
  ip[ 0] = x1; // X_x1
  ip[ 4] = ip[19] = ip[34] = ip[49] = y1; // X_y1
  ip[ 8] = ip[23] = ip[38] = ip[53] = x2; // X_x2
  ip[12] = ip[27] = ip[42] = ip[57] = y2; // X_y2
}

void VMD_Frame::set_interpolation_y(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t* ip = interpolation;
  ip[ 1] = ip[16] = x1; // Y_x1
  ip[ 5] = ip[20] = ip[35] = ip[50] = y1; // Y_y1
  ip[ 9] = ip[24] = ip[39] = ip[54] = x2; // Y_x2
  ip[13] = ip[28] = ip[43] = ip[58] = y2; // Y_y2
}

void VMD_Frame::set_interpolation_z(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t* ip = interpolation;
  ip[ 2] = ip[17] = ip[32] = x1; // Z_x1
  ip[ 6] = ip[21] = ip[36] = ip[51] = y1; // Z_y1
  ip[10] = ip[25] = ip[40] = ip[55] = x2; // Z_x2
  ip[14] = ip[29] = ip[44] = ip[59] = y2; // Z_y2

  ip[2] = 0; // MMD 9.31では0になっている
}

void VMD_Frame::set_interpolation_r(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t* ip = interpolation;
  ip[ 3] = ip[18] = ip[33] = ip[48] = x1; // R_x1
  ip[ 7] = ip[22] = ip[37] = ip[52] = y1; // R_y1
  ip[11] = ip[26] = ip[41] = ip[56] = x2; // R_x2
  ip[15] = ip[30] = ip[45] = ip[60] = y2; // R_y2

  ip[3] = 0; // MMD 9.31では0になっている
}


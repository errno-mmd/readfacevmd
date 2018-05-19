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

void VMD_Skin::read(ifstream& s)
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

    int skin_num;
    readInt(s, skin_num);
    cout << "skin_num: " << skin_num << endl;
    skin.clear();
    for (int i = 0; i < skin_num; i++) {
      VMD_Skin tmp;
      tmp.read(s);
      skin.push_back(tmp);
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

  } catch (ifstream::failure e) {
    std::cerr << "PMX model read error\n";
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

void VMD_Skin::write(ofstream& s)
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

  writeInt(s, skin.size());
  for (size_t i = 0; i < skin.size(); i++) {
    skin[i].write(s);
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

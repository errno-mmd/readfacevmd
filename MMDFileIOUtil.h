// -*- C++ -*-
// MMD File I/O Utility

#ifndef MMDFILEIOUTIL_H
#define MMDFILEIOUTIL_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

using std::ifstream;
using std::ofstream;
using std::ostream;
using std::string;
using std::wstring;
using namespace Eigen;

namespace MMDFileIOUtil {
  enum CharCode {
    Utf16le = 0,
    Utf8
  };
  static const string YamlIndent("  ");

  void readFloat(ifstream& in, float& f);
  void writeFloat(ofstream& out, const float& f);
  void readFloat2(ifstream& in, Vector2f& f2);
  void writeFloat2(ofstream& out, const Vector2f& f2);
  void readFloat3(ifstream& in, Vector3f& f3);
  void writeFloat3(ofstream& out, const Vector3f& f3);
  void readFloat4(ifstream& in, Vector4f& f4);
  void writeFloat4(ofstream& out, const Vector4f& f4);
  void readQuaternionf(ifstream& s, Quaternionf& q);
  void writeQuaternionf(ofstream& s, Quaternionf& q);

  void readUint8(ifstream& in, uint8_t& u);
  void writeUint8(ofstream& out, const uint8_t& u);
  void readUint16(ifstream& s, uint16_t& u);
  void writeUint16(ofstream& s, const uint16_t& u);
  void readUint32(ifstream& s, uint32_t& u);
  void writeUint32(ofstream& s, const uint32_t& u);
  void readInt(ifstream& s, int& i);
  void writeInt(ofstream& s, const int& i);

  void readStringUtf16le(ifstream& s, string& str, int size);
  void readStringUtf8(ifstream& s, string& str, int size);
  void sjis_to_utf8(const char* from, string& str, int size);
  void utf8_to_sjis(const string& from, char* to, int size_to);
  void readStringSJIS(ifstream& s, string& str, int size);
  void readTextBufToString(ifstream& s, string& str, CharCode code);
  void readTextBufToString(ifstream& s, string& str, uint8_t code);

  void writeTextBufUtf16le(ofstream& s, const string& str);
  void writeTextBufUtf8(ofstream& s, const string& str);
  void writeStringSJIS(ofstream& s, const string& str, int size);
  void writeTextBufFromString(ofstream& s, const string& str, CharCode code);
  void writeTextBufFromString(ofstream& s, const string& str, uint8_t code);
  void readCharArray(ifstream& s, char* a, int size);
  void writeCharArray(ofstream& s, const char* a, int size);
  
  int readVertexIndex(ifstream& s, int& idx, const size_t size);
  int readIndex(ifstream& s, int& idx, const size_t size);
  int writeVertexIndex(ofstream& s, const int& idx, const size_t index_size);
  int writeIndex(ofstream& s, const int& idx, const size_t index_size);

  void dumpFloat2(ostream& s, const string& prefix, const string& label, const Vector2f& v);
  void dumpFloat3(ostream& s, const string& prefix, const string& label, const Vector3f& v);
  void dumpFloat4(ostream& s, const string& prefix, const string& label, const Vector4f& v);
  void dumpString(ostream& s, const string& prefix, const string& label, const string& str);
}

#endif // ifndef MMDFILEIOUTIL_H

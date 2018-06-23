// -*- C++ -*-
// MMD File I/O Utility

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <codecvt>
#include <fstream>
#include <iomanip>
#include <locale>
#include <string>
#include <vector>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <assert.h>
#include "MMDFileIOUtil.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::wstring;
using std::vector;
using namespace Eigen;

void MMDFileIOUtil::readFloat(ifstream& s, float& f)
{
  s.read(reinterpret_cast<char*>(&f), sizeof(float));
}

void MMDFileIOUtil::writeFloat(ofstream& s, const float& f)
{
  s.write(reinterpret_cast<char*>(const_cast<float*>(&f)), sizeof(float));
}

void MMDFileIOUtil::readFloat2(ifstream& s, Vector2f& f2)
{
  for (int i = 0; i < 2; i++) {
    readFloat(s, f2(i));
  }
}

void MMDFileIOUtil::writeFloat2(ofstream& s, const Vector2f& f2)
{
  for (int i = 0; i < 2; i++) {
    writeFloat(s, f2(i));
  }
}

void MMDFileIOUtil::readFloat3(ifstream& s, Vector3f& f3)
{
  for (int i = 0; i < 3; i++) {
    readFloat(s, f3(i));
  }
}

void MMDFileIOUtil::writeFloat3(ofstream& s, const Vector3f& f3)
{
  for (int i = 0; i < 3; i++) {
    writeFloat(s, f3(i));
  }
}

void MMDFileIOUtil::readFloat4(ifstream& s, Vector4f& f4)
{
  for (int i = 0; i < 4; i++) {
    readFloat(s, f4(i));
  }
}

void MMDFileIOUtil::writeFloat4(ofstream& s, const Vector4f& f4)
{
  for (int i = 0; i < 4; i++) {
    writeFloat(s, f4(i));
  }
}

void MMDFileIOUtil::readQuaternionf(ifstream& s, Quaternionf& q)
{
  Vector4f f;
  readFloat4(s, f);
  q.w() = f(3);
  q.x() = f(0);
  q.y() = f(1);
  q.z() = f(2);
}

void MMDFileIOUtil::writeQuaternionf(ofstream& s, Quaternionf& q)
{
  writeFloat(s, q.x());
  writeFloat(s, q.y());
  writeFloat(s, q.z());
  writeFloat(s, q.w());
}

void MMDFileIOUtil::readUint8(ifstream& s, uint8_t& u)
{
  s.read(reinterpret_cast<char*>(&u), 1);
}

void MMDFileIOUtil::writeUint8(ofstream& s, const uint8_t& u)
{
  s.write(reinterpret_cast<char*>(const_cast<uint8_t*>(&u)), 1);
}

void MMDFileIOUtil::readUint16(ifstream& s, uint16_t& u)
{
  s.read(reinterpret_cast<char*>(&u), 2);
}

void MMDFileIOUtil::writeUint16(ofstream& s, const uint16_t& u)
{
  s.write(reinterpret_cast<char*>(const_cast<uint16_t*>(&u)), 2);
}

void MMDFileIOUtil::readUint32(ifstream& s, uint32_t& u)
{
  s.read(reinterpret_cast<char*>(&u), 4);
}

void MMDFileIOUtil::writeUint32(ofstream& s, const uint32_t& u)
{
  s.write(reinterpret_cast<char*>(const_cast<uint32_t*>(&u)), 4);
}

void MMDFileIOUtil::readInt(ifstream& s, int& i)
{
  // in PMX format 'int' = 32bit signed integer
  int32_t tmp;
  s.read(reinterpret_cast<char*>(&tmp), sizeof(int32_t));
  i = static_cast<int>(tmp);
}

void MMDFileIOUtil::writeInt(ofstream& s, const int& i)
{
  // in PMX format 'int' = 32bit signed integer
  int32_t tmp = static_cast<int32_t>(i);
  s.write(reinterpret_cast<char*>(&tmp), sizeof(int32_t));
}

// sからUTF-16LE(w/o BOM)文字列をsize byte読み、UTF-8に変換してstrに格納する。
void MMDFileIOUtil::readStringUtf16le(ifstream& s, string& str, int size)
{
  char buf[sizeof(wchar_t)] = { 0 };
  vector<wchar_t> v;
  int length = size / 2;
  for (int i = 0; i < length; ++i) {
    s.read(buf, 2);
    wchar_t* wc = reinterpret_cast<wchar_t*>(buf);
    v.push_back(*wc);
  }
  wstring ws(v.begin(), v.end());
  
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  str = converter.to_bytes(ws);
}

// sからUTF-8文字列をsize byte読み、strに格納する。
void MMDFileIOUtil::readStringUtf8(ifstream& s, string& str, int size)
{
  char buf[size + 1];
  s.read(buf, size);
  buf[size] = '\0';
  str = buf; // copy
}

void MMDFileIOUtil::sjis_to_utf8(const char* from, string& str, int size)
{
  char buf_in[size + 1];
  strncpy(buf_in, from, size);
  buf_in[size] = '\0';

  const char* fromcode = "Shift-JIS";
  const char* tocode = "UTF-8";
  UErrorCode status = U_ZERO_ERROR;
  UConverter* conv = ucnv_open(fromcode, &status);
  //  cout << "status: " << status << endl;
  assert(U_SUCCESS(status));
  UnicodeString us(buf_in, fromcode);

  const int bufsize = 1024;
  char* buf = new char[bufsize];
  int32_t targetsize = us.extract(0, us.length(), buf, bufsize, tocode);
  //  cout << "targetsize: " << targetsize << endl;
  if (targetsize > bufsize - 1) {
    //    cout << "retry" << endl;
    delete(buf);
    buf = new char[targetsize + 1];
    targetsize = us.extract(0, us.length(), buf, targetsize, tocode);
    //    cout << "targetsize: " << targetsize << endl;
  }
  buf[targetsize] = '\0';
  ucnv_close(conv);
  
  str = buf;
}

void MMDFileIOUtil::utf8_to_sjis(const string& from, char* to, int size_to)
{
  const char* fromcode = "UTF-8";
  const char* tocode = "Shift-JIS";
  const char* text = from.c_str();
  UErrorCode status = U_ZERO_ERROR;
  UConverter* conv = ucnv_open(fromcode, &status);
  // cout << "status: " << status << endl;
  assert(U_SUCCESS(status));
  UnicodeString us(text, fromcode);

  const int bufsize = 1024;
  char* buf = new char[bufsize];
  int32_t targetsize = us.extract(0, us.length(), buf, bufsize, tocode);
  // cout << "targetsize: " << targetsize << endl;
  if (targetsize > bufsize - 1) {
    // cout << "retry" << endl;
    delete(buf);
    buf = new char[targetsize + 1];
    targetsize = us.extract(0, us.length(), buf, targetsize, tocode);
    // cout << "targetsize: " << targetsize << endl;
  }
  buf[targetsize] = '\0';
  // cout << s << endl;
  ucnv_close(conv);

  // buf から to に最大 size_to だけコピー。必ずしもnull terminateしない
  memset(to, '\0', size_to);
  strncpy(to, buf, size_to);
}

// sからShift-JIS文字列をsize byte読み、UTF-8に変換してstrに格納する。
void MMDFileIOUtil::readStringSJIS(ifstream& s, string& str, int size)
{
  char buf_in[size + 1];
  s.read(buf_in, size);
  buf_in[size] = '\0';
  sjis_to_utf8(buf_in, str, size);
}

// 'TextBuf'形式の文字列をsから読んでstrに格納する。
void MMDFileIOUtil::readTextBufToString(ifstream& s, string& str, MMDFileIOUtil::CharCode code)
{
  int size;
  readInt(s, size);

  switch (code) {
  case MMDFileIOUtil::Utf16le:
    readStringUtf16le(s, str, size);
    break;
  case MMDFileIOUtil::Utf8:
    readStringUtf8(s, str, size);
    break;
  default:
    break;
  }
}

void MMDFileIOUtil::readTextBufToString(ifstream& s, string& str, uint8_t code)
{
  readTextBufToString(s, str, static_cast<MMDFileIOUtil::CharCode>(code));
}


void MMDFileIOUtil::writeTextBufUtf16le(ofstream& s, const string& str)
{
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>> converter;
  wstring ws = converter.from_bytes(str);

  int size = ws.length() * 2;
  writeInt(s, size);

  for (size_t i = 0; i < ws.length(); ++i) {
    wchar_t tmp = ws[i];
    s.write(reinterpret_cast<char*>(&tmp), 2);
  }
}

void MMDFileIOUtil::writeTextBufUtf8(ofstream& s, const string& str)
{
  int size = str.length();
  writeInt(s, size);
  s.write(str.c_str(), size);
}

void MMDFileIOUtil::writeStringSJIS(ofstream& s, const string& str, int size)
{
  char* buf = new char[size];
  utf8_to_sjis(str, buf, size);
  s.write(buf, size);
}

// 文字列strの内容を'TextBuf'形式でsに書く。
void MMDFileIOUtil::writeTextBufFromString(ofstream& s, const string& str, MMDFileIOUtil::CharCode code)
{
  switch (code) {
  case MMDFileIOUtil::Utf16le:
    writeTextBufUtf16le(s, str);
    break;
  case MMDFileIOUtil::Utf8:
    writeTextBufUtf8(s, str);
    break;
  default:
    break;
  }
}

void MMDFileIOUtil::writeTextBufFromString(ofstream& s, const string& str, uint8_t code)
{
  writeTextBufFromString(s, str, static_cast<MMDFileIOUtil::CharCode>(code));
}

void MMDFileIOUtil::readCharArray(ifstream& s, char* a, int size)
{
  s.read(a, size);
}

void MMDFileIOUtil::writeCharArray(ofstream& s, const char* a, int size)
{
  s.write(a, size);
}

int MMDFileIOUtil::readVertexIndex(ifstream& s, int& idx, const size_t index_size)
{
  // 1 Byte: unsigned
  // 2 Byte: unsigned
  // 4 Byte: signed ...
  if (index_size == 1) {
    uint8_t tmp;
    s.read(reinterpret_cast<char*>(&tmp), index_size);
    idx = static_cast<int>(tmp);
  } else if (index_size == 2) {
    uint16_t tmp;
    s.read(reinterpret_cast<char*>(&tmp), index_size);
    idx = static_cast<int>(tmp);
  } else if (index_size == 4) {
    int32_t tmp;
    s.read(reinterpret_cast<char*>(&tmp), index_size);
    idx = static_cast<int>(tmp);
  } else {
    return -1;
  }
  return 0;
}

int MMDFileIOUtil::readIndex(ifstream& s, int& idx, const size_t index_size)
{
  if (index_size == 1) {
    int8_t tmp;
    s.read(reinterpret_cast<char*>(&tmp), index_size);
    idx = static_cast<int>(tmp);
  } else if (index_size == 2) {
    int16_t tmp;
    s.read(reinterpret_cast<char*>(&tmp), index_size);
    idx = static_cast<int>(tmp);
  } else if (index_size == 4) {
    int32_t tmp;
    s.read(reinterpret_cast<char*>(&tmp), index_size);
    idx = static_cast<int>(tmp);
  } else {
    return -1;
  }
  return 0;
}

int MMDFileIOUtil::writeVertexIndex(ofstream& s, const int& idx, const size_t index_size)
{
  // 1 Byte: unsigned
  // 2 Byte: unsigned
  // 4 Byte: signed ...
  if (index_size == 1) {
    uint8_t tmp = static_cast<uint8_t>(idx);
    s.write(reinterpret_cast<char*>(&tmp), index_size);
  } else if (index_size == 2) {
    uint16_t tmp = static_cast<uint16_t>(idx);
    s.write(reinterpret_cast<char*>(&tmp), index_size);
  } else if (index_size == 4) {
    int32_t tmp = static_cast<int32_t>(idx);
    s.write(reinterpret_cast<char*>(&tmp), index_size);
  } else {
    return -1;
  }
  return 0;
}

int MMDFileIOUtil::writeIndex(ofstream& s, const int& idx, const size_t index_size)
{
  if (index_size == 1) {
    int8_t tmp = static_cast<int8_t>(idx);
    s.write(reinterpret_cast<char*>(&tmp), index_size);
  } else if (index_size == 2) {
    int16_t tmp = static_cast<int16_t>(idx);
    s.write(reinterpret_cast<char*>(&tmp), index_size);
  } else if (index_size == 4) {
    int32_t tmp = static_cast<int32_t>(idx);
    s.write(reinterpret_cast<char*>(&tmp), index_size);
  } else {
    return -1;
  }
  return 0;
}


void MMDFileIOUtil::dumpFloat2(ostream& s, const string& prefix, const string& label, const Vector2f& v)
{
  s << std::scientific << std::setprecision(7) << prefix << label << ": [" << v(0) << ", " << v(1) << "]\n";
}

void MMDFileIOUtil::dumpFloat3(ostream& s, const string& prefix, const string& label, const Vector3f& v)
{
  s << std::scientific << std::setprecision(7) << prefix << label << ": [" << v(0) << ", " << v(1) << ", " << v(2) << "]\n";
}

void MMDFileIOUtil::dumpFloat4(ostream& s, const string& prefix, const string& label, const Vector4f& v)
{
  s << std::scientific << std::setprecision(7) << prefix << label << ": [" << v(0) << ", " << v(1) << ", " << v(2) << ", " << v(3) << "]\n";
}

void MMDFileIOUtil::dumpString(ostream& s, const string& prefix, const string& label, const string& str)
{
  s << prefix << label << '"' << str << '"' << '\n';
}

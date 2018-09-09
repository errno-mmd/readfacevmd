#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <Eigen/Core>
#include <unsupported/Eigen/FFT>
#include "VMD.h"
#include "MMDFileIOUtil.h"
#include "smoothvmd.h"
#include "reducevmd.h"

using namespace Eigen;
using namespace MMDFileIOUtil;
using namespace std;

bool smooth_and_reduce(VMD& vmd, float cutoff_freq, float threshold_pos, float threshold_rot, float threshold_morph)
{
  // キーフレームをボーンごとに分ける
  map<string, vector<VMD_Frame>> frame_map;
  for (unsigned int i = 0; i < vmd.frame.size(); i++) {
    VMD_Frame frame = vmd.frame[i];
    string name;
    sjis_to_utf8(frame.bonename, name, frame.bonename_len);
    frame_map[name].push_back(frame);
  }

  // ボーンごとに平滑化と間引きを行う
  for (auto iter = frame_map.begin(); iter != frame_map.end(); iter++) {
    string name = iter->first;
    vector<VMD_Frame>& fv = iter->second;
    if (fv.size() > 0) {
      smooth_bone_frame(fv, cutoff_freq);
      VMD_Frame last_frame = fv.back();
      fv = reduce_bone_frame(fv, 0, fv.size() - 1, threshold_pos, threshold_rot);
      fv.push_back(last_frame);
    }
  }

  // ボーンキーフレームを入れ替える
  cout << "vmd.frame.size(original): " << vmd.frame.size() << endl;
  vmd.frame.clear();
  for (auto iter = frame_map.begin(); iter != frame_map.end(); iter++) {
    vector<VMD_Frame> fv = iter->second;
    for (unsigned int i = 0; i < fv.size(); i++) {
      vmd.frame.push_back(fv[i]);
    }
  }
  cout << "vmd.frame.size(reduced): " << vmd.frame.size() << endl;

  
  // キーフレームをモーフごとに分ける
  map<string, vector<VMD_Morph>> morph_map;
  for (unsigned int i = 0; i < vmd.morph.size(); i++) {
    VMD_Morph morph = vmd.morph[i];
    string name;
    sjis_to_utf8(morph.name, name, morph.name_len);
    morph_map[name].push_back(morph);
  }

  // モーフごとに平滑化と間引きを行う
  for (auto iter = morph_map.begin(); iter != morph_map.end(); iter++) {
    string name = iter->first;
    vector<VMD_Morph>& mv = iter->second;
    if (mv.size() > 0) {
      smooth_morph_frame(mv, cutoff_freq);
      VMD_Morph last_morph = mv.back();
      mv = reduce_morph_frame(mv, 0, mv.size() - 1, threshold_morph);
      mv.push_back(last_morph);
    }
  }

  // 表情キーフレームを入れ替える
  cout << "vmd.morph.size(original): " << vmd.morph.size() << endl;
  vmd.morph.clear();
  for (auto iter = morph_map.begin(); iter != morph_map.end(); iter++) {
    vector<VMD_Morph> sv = iter->second;
    for (unsigned int i = 0; i < sv.size(); i++) {
      vmd.morph.push_back(sv[i]);
    }
  }
  cout << "vmd.morph.size(reduced) : " << vmd.morph.size() << endl;
  
  return true;
}

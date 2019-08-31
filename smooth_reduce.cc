#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <Eigen/Core>
#include <unsupported/Eigen/FFT>
#include "VMD.h"
#include "MMDFileIOUtil.h"
#include "fpschanger.h"
#include "smoothvmd.h"
#include "reducevmd.h"

using namespace Eigen;
using namespace MMDFileIOUtil;
using namespace std;

// VMDモーションの平滑化および間引きを行う
bool smooth_and_reduce(VMD& vmd, float cutoff_freq, float threshold_pos, float threshold_rot,
                       float threshold_morph, float srcfps, float tgtfps, bool bezier)
{
  cout << "vmd.frame.size(original): " << vmd.frame.size() << endl;
  // キーフレームをボーンごとに分ける
  map<string, vector<VMD_Frame>> frame_map;
  for (unsigned int i = 0; i < vmd.frame.size(); i++) {
    VMD_Frame frame = vmd.frame[i];
    string name;
    sjis_to_utf8(frame.bonename, name, frame.bonename_len);
    frame_map[name].push_back(frame);
  }
  // ボーンごとに平滑化と間引きを行い、vmdのボーンキーフレームを入れ替える
  vmd.frame.clear();
  for (auto iter = frame_map.begin(); iter != frame_map.end(); iter++) {
    vector<VMD_Frame>& fv = iter->second;
    if (fv.size() > 2) {
      smooth_bone_frame(fv, cutoff_freq, bezier);
      if (srcfps != tgtfps) {
        fv = change_fps_bone(fv, srcfps, tgtfps, bezier);
      }
      fv = reduce_bone_frame(fv, 0, fv.size() - 1, threshold_pos, threshold_rot, bezier);
    }
    for (unsigned int i = 0; i < fv.size(); i++) {
      vmd.frame.push_back(fv[i]);
    }
  }
  cout << "vmd.frame.size(reduced): " << vmd.frame.size() << endl;
  
  cout << "vmd.morph.size(original): " << vmd.morph.size() << endl;
  // キーフレームをモーフごとに分ける
  map<string, vector<VMD_Morph>> morph_map;
  for (unsigned int i = 0; i < vmd.morph.size(); i++) {
    VMD_Morph morph = vmd.morph[i];
    string name;
    sjis_to_utf8(morph.name, name, morph.name_len);
    morph_map[name].push_back(morph);
  }
  // モーフごとに平滑化と間引きを行い、vmdの表情キーフレームを入れ替える
  vmd.morph.clear();
  for (auto iter = morph_map.begin(); iter != morph_map.end(); iter++) {
    vector<VMD_Morph>& mv = iter->second;
    if (mv.size() > 2) {
      smooth_morph_frame(mv, cutoff_freq);
      if (srcfps != tgtfps) {
        mv = change_fps_morph(mv, srcfps, tgtfps);
      }
      mv = reduce_morph_frame(mv, 0, mv.size() - 1, threshold_morph);
    }
    for (unsigned int i = 0; i < mv.size(); i++) {
      vmd.morph.push_back(mv[i]);
    }
  }
  cout << "vmd.morph.size(reduced) : " << vmd.morph.size() << endl;
  
  return true;
}

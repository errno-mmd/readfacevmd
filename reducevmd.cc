// VMDモーションを間引く

#include <vector>
#include <Eigen/Core>
#include "VMD.h"
#include "interpolate.h"
#include "reducevmd.h"

#define _USE_MATH_DEFINES
#include <math.h>

// head番目からtail番目のボーンキーフレームのうち、残すべきものを再帰的に探して返す。
// ただし返値にtail番目のボーンは含まれないので、コール元で追加する必要がある。
vector<VMD_Frame> reduce_bone_frame_recursive(const vector<VMD_Frame>& v, int head, int tail, float threshold_pos, float threshold_rot, bool bezier)
{
  float max_pos_err = 0.0;
  float max_rot_err = 0.0;
  int max_idx_pos = 0;
  int max_idx_rot = 0;
  VMD_Frame head_frame = v[head];
  VMD_Frame tail_frame = v[tail];
  const int bezier_interpolation_limit = 60;
  if (bezier && tail - head < bezier_interpolation_limit) {
    optimize_bezier_parameter(tail_frame, v, head, tail);
  }

  // headフレームの値とtailフレームの値によって決まる補完曲線(直線)から最も離れた(誤差の大きい)フレームを探す
  for (int i = head + 1; i < tail; i++) {
    VMD_Frame f = interpolate_frame(head_frame, tail_frame, i, bezier);
    float pos_err = (f.position - v[i].position).norm();
    if (pos_err > max_pos_err) {
      max_idx_pos = i;
      max_pos_err = pos_err;
    }
    float rot_err = fabs(f.rotation.angularDistance(v[i].rotation) * 180 / M_PI);
    if (rot_err > max_rot_err) {
      max_idx_rot = i;
      max_rot_err = rot_err;
    }
  }

  // 補間曲線から最も離れたフレームの誤差が閾値を超えていたら、そのフレーム(max_idx_*)を残し、
  // [head, max_idx_*] と [max_idx_*, tail] のそれぞれの区間を再帰的に探す。
  vector<VMD_Frame> v1;
  if (max_pos_err > threshold_pos) {
    v1 = reduce_bone_frame_recursive(v, head, max_idx_pos, threshold_pos, threshold_rot, bezier);
    vector<VMD_Frame> v2 = reduce_bone_frame_recursive(v, max_idx_pos, tail, threshold_pos, threshold_rot, bezier);
    v1.insert(v1.end(), v2.begin(), v2.end());
  } else {
    if (max_rot_err > threshold_rot) {
      v1 = reduce_bone_frame_recursive(v, head, max_idx_rot, threshold_pos, threshold_rot, bezier);
      vector<VMD_Frame> v2 = reduce_bone_frame_recursive(v, max_idx_rot, tail, threshold_pos, threshold_rot, bezier);
      v1.insert(v1.end(), v2.begin(), v2.end());
    } else {
      v1.push_back(tail_frame);
    }
  }
  return v1;
}

// head番目からtail番目のボーンキーフレームのうち、残すべきものを探して返す。
vector<VMD_Frame> reduce_bone_frame(const vector<VMD_Frame>& v, int head, int tail, float threshold_pos, float threshold_rot, bool bezier)
{
  if (threshold_pos < 0 || threshold_rot < 0) {
    vector<VMD_Frame> v1(v);
    return v1;
  }

  vector<VMD_Frame> v1 = reduce_bone_frame_recursive(v, head, tail, threshold_pos, threshold_rot, bezier);
  v1.insert(v1.begin(), v.front());

  return v1;
}
  
// head番目からtail番目の表情キーフレームのうち、残すべきものを再帰的に探して返す。
// ただし返値にtail番目のフレームは含まれないので、コール元で追加する必要がある。
vector<VMD_Morph> reduce_morph_frame_recursive(const vector<VMD_Morph>& v, int head, int tail, float threshold)
{
  float max = 0.0;
  int max_idx = 0;
  int total = tail - head;
  for (int i = head + 1; i < tail; i++) {
    float iv = v[head].weight + (v[tail].weight - v[head].weight) * (i - head) / total;
    float e = abs(iv - v[i].weight);
    if (e > max) {
      max_idx = i;
      max = e;
    }
  }

  vector<VMD_Morph> v1;
  if (max > threshold) {
    v1 = reduce_morph_frame_recursive(v, head, max_idx, threshold);
    vector<VMD_Morph> v2 = reduce_morph_frame_recursive(v, max_idx, tail, threshold);
    v1.insert(v1.end(), v2.begin(), v2.end());
  } else {
    v1.push_back(v[head]);
  }
  return v1;
}

// head番目からtail番目の表情キーフレームのうち、残すべきものを探して返す。
vector<VMD_Morph> reduce_morph_frame(const vector<VMD_Morph>& v, int head, int tail, float threshold)
{
  if (threshold < 0) {
    vector<VMD_Morph> v1(v);
    return v1;
  }

  vector<VMD_Morph> v1 = reduce_morph_frame_recursive(v, head, tail, threshold);
  v1.push_back(v.back());
  return v1;
}


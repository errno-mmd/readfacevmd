// VMDモーションを間引く

#include <vector>
#include <Eigen/Core>
#include "VMD.h"
#include "reducevmd.h"

#define _USE_MATH_DEFINES
#include <math.h>

vector<VMD_Frame> reduce_bone_frame(const vector<VMD_Frame>& v, int head, int tail, float threshold_pos, float threshold_rot)
{
  if (threshold_pos < 0 || threshold_rot < 0) {
    vector<VMD_Frame> v1(v);
    return v1;
  }

  float max_pos_err = 0.0;
  float max_rot_err = 0.0;
  int max_idx_pos = 0;
  int max_idx_rot = 0;
  int total = tail - head;
  for (int i = head + 1; i < tail; i++) {
    Vector3f ip_pos = v[head].position + (v[tail].position - v[head].position) * (i - head) / total;
    float pos_err = (ip_pos - v[i].position).norm();
    if (pos_err > max_pos_err) {
      max_idx_pos = i;
      max_pos_err = pos_err;
    }
    float t = float(i - head) / total;
    Quaternionf ip_rot = v[head].rotation.slerp(t, v[tail].rotation);
    Quaternionf q_err = ip_rot * v[i].rotation.inverse();
    float rot_err = acos(q_err.w()) * 2 * 180 / M_PI; // 角度を取得。単位はdegree
    if (rot_err > max_rot_err) {
      max_idx_rot = i;
      max_rot_err = rot_err;
    }
  }

  vector<VMD_Frame> v1;
  if (max_pos_err > threshold_pos) {
    v1 = reduce_bone_frame(v, head, max_idx_pos, threshold_pos, threshold_rot);
    vector<VMD_Frame> v2 = reduce_bone_frame(v, max_idx_pos, tail, threshold_pos, threshold_rot);
    v1.insert(v1.end(), v2.begin(), v2.end());
  } else {
    if (max_rot_err > threshold_rot) {
      v1 = reduce_bone_frame(v, head, max_idx_rot, threshold_pos, threshold_rot);
      vector<VMD_Frame> v2 = reduce_bone_frame(v, max_idx_rot, tail, threshold_pos, threshold_rot);
      v1.insert(v1.end(), v2.begin(), v2.end());
    } else {
      v1.push_back(v[head]);
    }
  }
  return v1;
}

vector<VMD_Morph> reduce_morph_frame(const vector<VMD_Morph>& v, int head, int tail, float threshold)
{
  if (threshold < 0) {
    vector<VMD_Morph> v1(v);
    return v1;
  }

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
    v1 = reduce_morph_frame(v, head, max_idx, threshold);
    vector<VMD_Morph> v2 = reduce_morph_frame(v, max_idx, tail, threshold);
    v1.insert(v1.end(), v2.begin(), v2.end());
  } else {
    v1.push_back(v[head]);
  }
  return v1;
}




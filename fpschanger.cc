#include "fpschanger.h"

#include <vector>
#include "VMD.h"
#include "interpolate.h"

vector<VMD_Frame> change_fps_bone(const vector<VMD_Frame>& v, float srcfps, float tgtfps, bool bezier)
{
  vector<VMD_Frame> v1;
  if (v.size() == 0) {
    return v1;
  }

  VMD_Frame prev = v[0];
  v1.push_back(v[0]);
  int next_frame = 1;
  for (unsigned int i = 1; i < v.size(); ) {
    if (v[i].number == prev.number) {
      i++;
      continue;
    }
    float t_tgt = float(next_frame) / tgtfps;
    float t = float(v[i].number) / srcfps;
    if (t >= t_tgt) {
      float t_prev = float(prev.number) / srcfps;
      float ratio = (t_tgt - t_prev) / (t - t_prev);
      VMD_Frame f = make_intermediate_frame(prev, v[i], ratio, bezier);
      f.number = next_frame;
      v1.push_back(f);
      next_frame++;
      // iはインクリメントしない
    } else {
      prev = v[i];
      i++;
      continue;
    }
  }
  return v1;
}

vector<VMD_Morph> change_fps_morph(const vector<VMD_Morph>& v, float srcfps, float tgtfps)
{
  vector<VMD_Morph> v1;
  if (v.size() == 0) {
    return v1;
  }

  VMD_Morph prev = v[0];
  v1.push_back(v[0]);
  int next_frame = 1;
  for (unsigned int i = 1; i < v.size(); ) {
    if (v[i].frame == prev.frame) {
      i++;
      continue;
    }
    float t_tgt = float(next_frame) / tgtfps;
    float t = float(v[i].frame) / srcfps;
    if (t >= t_tgt) {
      float t_prev = float(prev.frame) / srcfps;
      float ratio = (t_tgt - t_prev) / (t - t_prev);
      VMD_Morph f = make_intermediate_morph(prev, v[i], ratio);
      f.frame = next_frame;
      v1.push_back(f);
      next_frame++;
      // iはインクリメントしない
    } else {
      prev = v[i];
      i++;
      continue;
    }
  }
  return v1;

}

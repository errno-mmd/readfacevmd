// -*- C++ -*-
// VMDモーションを平滑化する

#ifndef SMOOTHVMD_H
#define SMOOTHVMD_H

#include <vector>
#include "VMD.h"

// ボーンキーフレーム列fvの値を平滑化する
void smooth_bone_frame(vector<VMD_Frame>& fv, float cutoff_freq);

// 表情キーフレーム列mvの値を平滑化する
void smooth_morph_frame(vector<VMD_Morph>& mv, float cutoff_freq);

#endif // ifndef SMOOTHVMD_H

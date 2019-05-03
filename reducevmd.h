// -*- C++ -*-
// VMDモーションを間引く

#ifndef REDUCEVMD_H
#define REDUCEVMD_H

#include <vector>
#include "VMD.h"

// head番目からtail番目のボーンキーフレームのうち、残すべきものを探して返す。
vector<VMD_Frame> reduce_bone_frame(const vector<VMD_Frame>& v, int head, int tail, float threshold_pos, float threshold_rot, bool bezier=false);

// head番目からtail番目の表情キーフレームのうち、残すべきものを探して返す。
vector<VMD_Morph> reduce_morph_frame(const vector<VMD_Morph>& v, int head, int tail, float threshold);

// head_frameとtail_frameを元に、補間でframe_num番目のボーンフレームを作る
VMD_Frame interpolate_frame(const VMD_Frame& head_frame, const VMD_Frame& tail_frame, int frame_num, bool bezier=false);

// head_frameとtail_frameを元に、補間でframe_num番目の表情フレームを作る
VMD_Morph interpolate_morph(const VMD_Morph& head_frame, const VMD_Morph& tail_frame, int frame_num);

#endif // ifndef REDUCEVMD_H


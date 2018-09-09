// -*- C++ -*-
// VMDモーションを平滑化する

#ifndef SMOOTHVMD_H
#define SMOOTHVMD_H

void interpolate_frame(vector<VMD_Frame>& fv);
void interpolate_morph(vector<VMD_Morph>& fv);
void lowpass_filter(vector<float>& v, float cutoff_freq);
void smooth_bone_frame(vector<VMD_Frame>& fv, float cutoff_freq);
void smooth_morph_frame(vector<VMD_Morph>& mv, float cutoff_freq);

#endif // ifndef SMOOTHVMD_H

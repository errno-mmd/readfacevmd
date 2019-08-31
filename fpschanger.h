// -*- C++ -*-
// VMDモーションのフレームレートを変更する

#ifndef FPSCHANGER_H
#define FPSCHANGER_H

#include <vector>
#include "VMD.h"

vector<VMD_Frame> change_fps_bone(const vector<VMD_Frame>& v, float srcfps, float tgtfps, bool bezier);

vector<VMD_Morph> change_fps_morph(const vector<VMD_Morph>& v, float srcfps, float tgtfps);

#endif // ifndef FPSCHANGER_H

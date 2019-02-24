// -*- C++ -*-
// VMDモーションを平滑化して間引く

#ifndef SMOOTH_REDUCE_H
#define SMOOTH_REDUCE_H

#include "VMD.h"

// VMDモーションの平滑化および間引きを行う
bool smooth_and_reduce(VMD& vmd, float cutoff_freq, float threshold_pos, float threshold_rot, float threshold_morph);

#endif // ifndef SMOOTH_REDUCE_H

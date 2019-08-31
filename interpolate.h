// -*- C++ -*-
// VMDモーションの補間

#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include "VMD.h"

using namespace Eigen;

// 制御点(0,0), p1, p2, (1,1)の3次ベジェ曲線上の、パラメータtに対応する点を返す
inline float bezier3(float p1, float p2, float t)
{
  float s = 1 - t;
  return (3 * s * s * p1 + 3 * s * t * p2 + t * t) * t;
}

// 3次方程式 a*x^3 + b*x^2 + c*x + d = 0 の解を求める
Vector3cd solve_cubic_equation(double a, double b, double c, double d);

// x座標値がx_argとなる点をベジェ曲線上から探し、その点におけるパラメータtの値を返す
float bezier_findt(Vector2f p1, Vector2f p2, float x_arg);

// x座標値がx_argとなる点をベジェ曲線上から探し、その点のy座標値を返す
float bezier_y(Vector2f p1, Vector2f p2, float x_arg);

// head_frameとtail_frameを元に、中間のボーンフレームを作る
VMD_Frame make_intermediate_frame(const VMD_Frame& head_frame, const VMD_Frame& tail_frame, float ratio, bool bezier);

// head_frameとtail_frameを元に、補間でframe_num番目のボーンフレームを作る
VMD_Frame interpolate_frame(const VMD_Frame& head_frame, const VMD_Frame& tail_frame, int frame_num, bool bezier);

// head_frameとtail_frameを元に、中間の表情フレームを作る
VMD_Morph make_intermediate_morph(const VMD_Morph& head_frame, const VMD_Morph& tail_frame, float ratio);

// head_frameとtail_frameを元に、補間でframe_num番目の表情フレームを作る
VMD_Morph interpolate_morph(const VMD_Morph& head_frame, const VMD_Morph& tail_frame, int frame_num);

vector<VMD_Frame> fill_bone_frame(const vector<VMD_Frame>& fv, bool bezier);

vector<VMD_Morph> fill_morph_frame(vector<VMD_Morph>& mv);

// (head+1)番めから(tail-1)番目までの誤差が最小になるような補間曲線パラメータを探す
VectorXf find_bezier_parameter_r(const vector<VMD_Frame>& v, int head, int tail);

// head番めからtail番目までの誤差が最小になるような補間曲線パラメータを探す
VectorXf find_bezier_parameter_pos(const vector<VMD_Frame>& v, int head, int tail, int axis);

// head番めからtail番目までの誤差が最小になるようtail_frameの補間曲線パラメータを調整する
void optimize_bezier_parameter(VMD_Frame& tail_frame, const vector<VMD_Frame>& v, int head, int tail);

#endif // ifndef INTERPOLATE_H


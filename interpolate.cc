// VMDモーションの補間を行う
#include "interpolate.h"

#include <Eigen/Core>
#include <unsupported/Eigen/NonLinearOptimization>
#include <unsupported/Eigen/NumericalDiff>
#include "VMD.h"

using namespace Eigen;

// x座標値がx_argとなる点をベジェ曲線上から探し、その点のy座標値を返す
// ベジェ曲線の制御点は(0,0), p1, p2, (1,1)とする
// 0 < x_arg < 1 とする
float bezier_y(Vector2f p1, Vector2f p2, float x_arg)
{
  // 二分法でtを探す
  float epsilon = 1.0e-5;
  float lower = 0.0;
  float upper = 1.0;
  float t;
  float x;
  float x1 = p1.x();
  float x2 = p2.x();
  const int max_iteration = 20;
  for (int i = 0; i < max_iteration; i++) {
    t = (lower + upper) / 2;
    x = bezier3(x1, x2, t);
    if (abs(x - x_arg) < epsilon) {
      // x == x_arg となる t が見つかった
      break;
    }
    if (x < x_arg) {
      // x(t)は単調増加するので、
      // 真のxがもっと大きいということは、真のtはもっと大きいので、
      // 探索範囲を上半分に絞る
      lower = t;
    } else {
      upper = t;
    }
  }
  return bezier3(p1.y(), p2.y(), t);
}

float bezier_y_vmd(uint8_t ipx1, uint8_t ipy1, uint8_t ipx2, uint8_t ipy2, float x)
{
  return bezier_y(Vector2f(float(ipx1)/127, float(ipy1)/127), Vector2f(float(ipx2)/127, float(ipy2)/127), x);
}

VMD_Frame make_intermediate_frame(const VMD_Frame& head_frame, const VMD_Frame& tail_frame, float ratio, bool bezier)
{
  VMD_Frame f;
  memcpy(f.bonename, head_frame.bonename, f.bonename_len);
  if (bezier) {
    // ベジェ曲線補間
    const uint8_t* ip = tail_frame.interpolation;
    float y;

    // X座標値の補間
    y = bezier_y_vmd(ip[0], ip[4], ip[8], ip[12], ratio);
    f.position.x() = head_frame.position.x() * (1-y) + tail_frame.position.x() * y;

    // Y座標値の補間
    y = bezier_y_vmd(ip[16], ip[20], ip[24], ip[28], ratio);
    f.position.y() = head_frame.position.y() * (1-y) + tail_frame.position.y() * y;

    // Z座標値の補間
    y = bezier_y_vmd(ip[32], ip[36], ip[40], ip[44], ratio);
    f.position.z() = head_frame.position.z() * (1-y) + tail_frame.position.z() * y;

    // 回転の補間
    y = bezier_y_vmd(ip[48], ip[52], ip[56], ip[60], ratio);
    f.rotation = head_frame.rotation.slerp(y, tail_frame.rotation);
  } else {
    // 線形補間
    f.position = head_frame.position + (tail_frame.position - head_frame.position) * ratio;
    f.rotation = head_frame.rotation.slerp(ratio, tail_frame.rotation);
  }
  return f;
}

// head_frameとtail_frameを元に、補間でframe_num番目のボーンフレームを作る
VMD_Frame interpolate_frame(const VMD_Frame& head_frame, const VMD_Frame& tail_frame, int frame_num, bool bezier)
{
  int total = tail_frame.number - head_frame.number;
  float ratio = float(frame_num - head_frame.number) / total;
  VMD_Frame f = make_intermediate_frame(head_frame, tail_frame, ratio, bezier);
  f.number = frame_num;
  return f;
}

VMD_Morph make_intermediate_morph(const VMD_Morph& head_frame, const VMD_Morph& tail_frame, float ratio)
{
  VMD_Morph m = head_frame;
  m.weight = head_frame.weight + (tail_frame.weight - head_frame.weight) * ratio;
  return m;
}

// head_frameとtail_frameを元に、補間でframe_num番目の表情フレームを作る
VMD_Morph interpolate_morph(const VMD_Morph& head_frame, const VMD_Morph& tail_frame, int frame_num)
{
  int total = tail_frame.frame - head_frame.frame;
  float ratio = float(frame_num - head_frame.frame) / total;
  VMD_Morph m = make_intermediate_morph(head_frame, tail_frame, ratio);
  m.frame = frame_num;
  return m;
}

vector<VMD_Frame> fill_bone_frame(const vector<VMD_Frame>& fv, bool bezier)
{
  vector<VMD_Frame> fv_new;
  VMD_Frame f_old = fv[0];
  f_old.number = 0;
  fv_new.push_back(f_old);
  for (VMD_Frame f : fv) {
    // もしフレーム番号が重複していたら、重複したフレームは消す
    if (f.number == f_old.number) {
      continue;
    }
    // フレーム番号が連続していない場合、途中のフレームを補間する
    for (uint32_t i = f_old.number + 1; i < f.number; i++) {
      VMD_Frame interpolated = interpolate_frame(f_old, f, i, bezier);
      fv_new.push_back(interpolated);
    }
    fv_new.push_back(f);
    f_old = f;
  }
  return fv_new;
}

vector<VMD_Morph> fill_morph_frame(vector<VMD_Morph>& mv)
{
  vector<VMD_Morph> mv_new;
  VMD_Morph m_old = mv[0];
  m_old.frame = 0;
  mv_new.push_back(m_old);
  for (VMD_Morph m : mv) {
    // もしフレーム番号が重複していたら、重複したフレームは消す
    if (m.frame == m_old.frame) {
      continue;
    }
    // フレーム番号が連続していない場合、途中のフレームを補間する
    for (uint32_t i = m_old.frame + 1; i < m.frame; i++) {
      VMD_Morph interpolated = interpolate_morph(m_old, m, i);
      mv_new.push_back(interpolated);
    }
    mv_new.push_back(m);
    m_old = m;
  }
  return mv_new;
}

// Eigen::LevenbergMarquardt で非線形最小二乗法フィッティングに使う構造体
template<typename _Scalar, int NX=Eigen::Dynamic, int NY=Eigen::Dynamic>
struct Functor
{
    typedef _Scalar Scalar;
    enum {
        InputsAtCompileTime = NX,
        ValuesAtCompileTime = NY
    };
    typedef Matrix<Scalar, InputsAtCompileTime, 1> InputType;
    typedef Matrix<Scalar, ValuesAtCompileTime, 1> ValueType;
    typedef Matrix<Scalar, ValuesAtCompileTime, InputsAtCompileTime> JacobianType;

    const int m_inputs, m_values;

    Functor() : m_inputs(InputsAtCompileTime), m_values(ValuesAtCompileTime) {}
    Functor(int inputs, int values) : m_inputs(inputs), m_values(values) {}

    int inputs() const { return m_inputs; }
    int values() const { return m_values; }
};

struct position_functor : Functor<float>
{
  position_functor(int nparam, int nvalue, const vector<float>& x, const vector<float>& y)
  : Functor<float>(nparam, nvalue), x(x), y(y) {}

  const vector<float>& x;
  const vector<float>& y;

  // 各データ点(x[i], y[i])における誤差をf[i]に格納する
  // パラメータpはベジェ曲線の制御点の座標値
  // ベジェ曲線の制御点 = (0, 0), (p[0], p[1]), (p[2], p[3]), (1, 1) となる
  int operator() (const VectorXf& p, VectorXf& f) const
  {
    Vector2f cp1(p[0], p[1]);
    Vector2f cp2(p[2], p[3]);
    for (int i = 0; i < m_values; i++) {
      float by = bezier_y(cp1, cp2, x[i]);
      f[i] = y[0]*(1.0-by) + y[m_values - 1]*by - y[i];
    }
    return 0;
  }
};

struct rotation_functor : Functor<float>
{
  rotation_functor(int nparam, int nvalue, const vector<float>& x, const vector<Eigen::Quaternionf>& y)
  : Functor<float>(nparam, nvalue), x(x), y(y) {}

  const vector<float>& x;
  const vector<Eigen::Quaternionf>& y;

  // 各データ点における誤差をf[i]に格納する
  // パラメータpはベジェ曲線の制御点の座標値
  // ベジェ曲線の制御点 = (0, 0), (p[0], p[1]), (p[2], p[3]), (1, 1) となる
  int operator() (const VectorXf& p, VectorXf& f) const
  {
    Vector2f cp1(p[0], p[1]);
    Vector2f cp2(p[2], p[3]);
    for (int i = 0; i < m_values; i++) {
      float by = bezier_y(cp1, cp2, x[i]);
      Eigen::Quaternionf rot = y[0].slerp(by, y[m_values - 1]);
      f[i] = rot.angularDistance(y[i]);
    }
    return 0;
  }
};

// head番めからtail番目までの誤差が最小になるような補間曲線パラメータを探す
VectorXf find_bezier_parameter_pos(const vector<VMD_Frame>& v, int head, int tail, int axis)
{
  vector<float> x;
  vector<float> y;
  for (int i = head; i <= tail; i++) {
    int current = v[i].number;
    x.push_back(float(current - head)/(tail - head));
    y.push_back(v[i].position(axis));
  }
  // パラメータの初期値を与える
  // ベジェ曲線の制御点 = (0, 0), (p[0], p[1]), (p[2], p[3]), (1, 1) となる
  VectorXf p(4);
  p << 20.0/127, 20.0/127, 107.0/127, 107.0/127;
  position_functor functor(4, (tail - head + 1), x, y);
  NumericalDiff<position_functor> nd(functor);
  LevenbergMarquardt<NumericalDiff<position_functor>, float> lm(nd);
  lm.parameters.maxfev = 10;
  lm.minimize(p);
  return p;
}

// head番めからtail番目までの誤差が最小になるような補間曲線パラメータを探す
VectorXf find_bezier_parameter_rot(const vector<VMD_Frame>& v, int head, int tail)
{
  vector<float> x;
  vector<Eigen::Quaternionf> y;
  for (int i = head; i <= tail; i++) {
    int current = v[i].number;
    x.push_back(float(current - head)/(tail - head));
    y.push_back(v[i].rotation);
  }
  // パラメータの初期値を与える
  // ベジェ曲線の制御点 = (0, 0), (p[0], p[1]), (p[2], p[3]), (1, 1) となる
  VectorXf p(4);
  p << 20.0/127, 20.0/127, 107.0/127, 107.0/127;
  // パラメータpの最適化を行う
  rotation_functor functor(4, (tail - head + 1), x, y);
  NumericalDiff<rotation_functor> nd(functor);
  LevenbergMarquardt<NumericalDiff<rotation_functor>, float> lm(nd);
  lm.parameters.maxfev = 10;
  lm.minimize(p);
  return p;
}

void convert_interpolation(uint8_t* ip, const VectorXf p)
{
  for (int i = 0; i < 4; i++) {
    int k = int(p[i] * 127);
    if (k > 127) {
      ip[i] = 127;
    } else if (k < 0) {
      ip[i] = 0;
    } else {
      ip[i] = uint8_t(k);
    }
  }
}

// head番めからtail番目までの誤差が最小になるようtail_frameの補間曲線パラメータを調整する
void optimize_bezier_parameter(VMD_Frame& tail_frame, const vector<VMD_Frame>& v,
                               int head, int tail)
{
  uint8_t ip[4];
  VectorXf p;

  // Xの補間パラメータの最適化
  p = find_bezier_parameter_pos(v, head, tail, 0);
  convert_interpolation(ip, p);
  tail_frame.set_interpolation_x(ip[0], ip[1], ip[2], ip[3]);

  // Yの補間パラメータの最適化
  p = find_bezier_parameter_pos(v, head, tail, 1);
  convert_interpolation(ip, p);
  tail_frame.set_interpolation_y(ip[0], ip[1], ip[2], ip[3]);

  // Zの補間パラメータの最適化
  p = find_bezier_parameter_pos(v, head, tail, 2);
  convert_interpolation(ip, p);
  tail_frame.set_interpolation_z(ip[0], ip[1], ip[2], ip[3]);

  // 回転の補間パラメータの最適化
  p = find_bezier_parameter_rot(v, head, tail);
  convert_interpolation(ip, p);
  tail_frame.set_interpolation_r(ip[0], ip[1], ip[2], ip[3]);
}

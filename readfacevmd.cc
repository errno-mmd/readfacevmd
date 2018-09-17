// readfacevmd - reads facial expression from photo / movie and generate a VMD motion file.

// OpenFace Headers
#include <LandmarkCoreIncludes.h>
#include <FaceAnalyser.h>
#include <SequenceCapture.h>
#include <GazeEstimation.h>

#include <opencv2/core/core.hpp>
#include <boost/filesystem.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "smooth_reduce.h"
#include "MMDFileIOUtil.h"
#include "VMD.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define RFV_DLL_EXPORT
#include "readfacevmd.h"

using namespace std;
using namespace Eigen;

const int LANDMARK_NUM = 68;

// Action Unit ID
enum AUID {
  InnerBrowRaiser = 1,      // AU01 眉の内側を上げる
  OuterBrowRaiser = 2,      // AU02 眉の外側を上げる
  BrowLowerer = 4,          // AU04 眉を下げる
  UpperLidRaiser = 5,       // AU05 目を見開く
  CheekRaiser = 6,          // AU06 頬を上げる
  LidTightener = 7,         // AU07 細目
  NoseWrinkler = 9,         // AU09 鼻に皴を寄せる。怒り
  UpperLipRaiser = 10,      // AU10 上唇を上げる
  LipCornerPuller = 12,     // AU12 口の端を上げる。にやり
  Dimpler = 14,             // AU14 えくぼ
  LipCornerDepressor = 15,  // AU15 への字口
  ChinRaiser = 17,          // AU17 顎を上げる
  LipStrecher = 20,         // AU20 口を横に伸ばす
  LipTightener = 23,        // AU23 口をすぼめる
  LipPart = 25,             // AU25 口を開ける。「い」の口でもtrueになる
  JawDrop = 26,             // AU26 顎を下げる。「あ」の口の判定にはこちらを使う
  LipSuck = 28,             // AU28 唇を吸う
  Blink = 45,               // AU45 まばたき
};
const int AU_SIZE = 46;
const double ACTION_UNIT_MAXVAL = 5.0;

void dumprot(const Quaterniond& rot, string name)
{
  Vector3d v = rot.toRotationMatrix().eulerAngles(0, 1, 2);
  cout << name << ": " << v.x() * 180 / M_PI << "," << v.y()  * 180 / M_PI << "," << v.z()  * 180 / M_PI << endl;
}


// 回転のキーフレームを VMD_Frame の vector に追加する
void add_rotation_pose(vector<VMD_Frame>& frame_vec, const Quaterniond& rot, uint32_t frame_number, string bone_name)
{
    VMD_Frame frame;
    MMDFileIOUtil::utf8_to_sjis(bone_name, frame.bonename, frame.bonename_len);
    frame.number = frame_number;
    frame.rotation.w() = rot.w();
    frame.rotation.x() = rot.x();
    frame.rotation.y() = rot.y();
    frame.rotation.z() = rot.z();
    frame_vec.push_back(frame);
}

// 移動のキーフレームを VMD_Frame の vector に追加する
void add_position_pose(vector<VMD_Frame>& frame_vec, const Vector3f& pos, uint32_t frame_number, string bone_name)
{
    VMD_Frame frame;
    MMDFileIOUtil::utf8_to_sjis(bone_name, frame.bonename, frame.bonename_len);
    frame.number = frame_number;
    frame.position.x() = pos.x();
    frame.position.y() = pos.y();
    frame.position.z() = pos.z();
    frame_vec.push_back(frame);
}

// 頭の向き(回転)のキーフレームを VMD_Frame の vector に格納する
void add_head_pose(vector<VMD_Frame>& frame_vec, const Quaterniond& rot, uint32_t frame_number)
{
  string bone_name = u8"頭";
  add_rotation_pose(frame_vec, rot, frame_number, bone_name);
}

// センターの位置のキーフレームを VMD_Frame の vector に格納する
void add_center_frame(vector<VMD_Frame>& frame_vec, const Vector3f& pos, uint32_t frame_number)
{
  // string bone_name = u8"センター";
  string bone_name = u8"全ての親";
  add_position_pose(frame_vec, pos, frame_number, bone_name);
}

// 目の向き(回転)のキーフレームを VMD_Frame の vector に追加する
void add_gaze_pose(vector<VMD_Frame>& frame_vec, cv::Point3f gazedir_left, cv::Point3f gazedir_right,
		   const Quaterniond& head_rot, uint32_t frame_number)
{
  // TODO: 目の回転補正(eye_rot_amp)の適切な値を決める
  const double eye_rot_amp = 0.25;
  //  cout << "gazedir_left:" << gazedir_left << endl;
  //  cout << "gazedir_right:" << gazedir_right << endl;
  string bone_name = u8"両目";
  Vector3d front = head_rot * Vector3d(0, 0, -1);
  Vector3d leftdir;
  leftdir.x() = gazedir_left.x;
  leftdir.y() = - gazedir_left.y;
  leftdir.z() = gazedir_left.z;
  Quaterniond rot_left = Quaterniond::FromTwoVectors(front, leftdir);
  add_rotation_pose(frame_vec, rot_left, frame_number, bone_name);
  //  cout << "leftdir:" << leftdir.x() << "," << leftdir.y() << "," << leftdir.z() << endl;
  //  cout << "front:" << front.x() << "," << front.y() << "," << front.z() << endl;
  //  dumprot(rot_left, "rot_left");
  
  bone_name = u8"右目";
  Vector3d rightdir;
  rightdir.x() = gazedir_right.x;
  rightdir.y() = - gazedir_right.y;
  rightdir.z() = gazedir_right.z;
  Quaterniond rot_right = Quaterniond::FromTwoVectors(leftdir, rightdir);
  rot_right = Quaterniond::Identity().slerp(eye_rot_amp, rot_right); // 回転量を補正
  add_rotation_pose(frame_vec, rot_right, frame_number, bone_name);
  //  cout << "rightdir:" << rightdir.x() << "," << rightdir.y() << "," << rightdir.z() << endl;
  //  dumprot(rot_right, "rot_right");
}

// 表情フレームを VMD_Morph の vector に追加する 
void add_morph_frame(vector<VMD_Morph>& morph_vec, string name, uint32_t frame_number, float weight)
{
  if (weight > 1.0) {
    weight = 1.0;
  }
  if (weight < 0.0) {
    weight = 0.0;
  }
  VMD_Morph morph;
  MMDFileIOUtil::utf8_to_sjis(name, morph.name, morph.name_len);
  morph.frame = frame_number;
  morph.weight = weight;
  morph_vec.push_back(morph);
}

// 顔の動きを表すAction Unitをface_analyserから取り出す
void get_action_unit(double* au, FaceAnalysis::FaceAnalyser face_analyser) {
  for (int i = 0; i < AU_SIZE; i++) {
    au[i] = 0;
  }

  auto intensity = face_analyser.GetCurrentAUsReg();
  auto presence = face_analyser.GetCurrentAUsClass();

  bool valid[AU_SIZE];

  for (auto& p : presence) {
    string id_str = p.first.substr(2, 2); // 数字部分を取り出す
    int id = atoi(id_str.c_str());
    if (p.second == 0) {
      valid[id] = false;
    } else {
      valid[id] = true;
    }
  }
  
  for (auto& p : intensity) {
    string id_str = p.first.substr(2, 2); // 数字部分を取り出す
    int id = atoi(id_str.c_str());
    if (valid[id]) {
      au[id] = p.second / ACTION_UNIT_MAXVAL;
      // cout << "AU" << id << ": " << au[id] << endl;
    }
  }
}

// 顔の表情を推定して morph_vec に追加する
void estimate_facial_expression(vector<VMD_Morph>& morph_vec, double* au, uint32_t frame_number)
{
  // 口
  double mouth_a = au[AUID::JawDrop] * 2;
  double mouth_i = 0;
  double mouth_u = au[AUID::LipTightener] * 2;
  if (mouth_a < 0.1 && mouth_u < 0.1) {
    mouth_i = au[AUID::LipPart] * 2;
  }
  double mouth_smile = au[AUID::LipCornerPuller];

  add_morph_frame(morph_vec, u8"あ", frame_number, mouth_a);
  add_morph_frame(morph_vec, u8"い", frame_number, mouth_i);
  add_morph_frame(morph_vec, u8"う", frame_number, mouth_u);
  add_morph_frame(morph_vec, u8"にやり", frame_number, mouth_smile);

  // 目
  double blink = au[AUID::LidTightener];
  //  if (blink > 0.5 || au[AUID::Blink] > 0) {
  if (au[AUID::Blink] > 0.2) {
    blink = 1.0;
  }
  double eye_smile = 0;
  if (au[AUID::CheekRaiser] > 0.2) {
    eye_smile = blink;
    blink = 0;
  }
  add_morph_frame(morph_vec, u8"まばたき", frame_number, blink);
  add_morph_frame(morph_vec, u8"笑い", frame_number, eye_smile);

  // 眉
  add_morph_frame(morph_vec, u8"にこり", frame_number, au[AUID::InnerBrowRaiser]);
  add_morph_frame(morph_vec, u8"怒り", frame_number, au[AUID::OuterBrowRaiser]);
  add_morph_frame(morph_vec, u8"下", frame_number, au[AUID::BrowLowerer]);
}

void init_vmd_header(VMD_Header& h)
{
  memset(h.version, 0, h.version_len);
  strcpy(h.version, "Vocaloid Motion Data 0002");
  memset(h.modelname, 0, h.modelname_len);
  strcpy(h.modelname, "dummy model");
}

// image_file_name で指定された画像/動画ファイルから表情を推定して vmd_file_name に出力する
RFV_DLL_DECL int read_face_vmd(const char* image_file_name, const char* vmd_file_name, float cutoff_freq,
			       float threshold_pos, float threshold_rot, float threshold_morph)
{
  vector<string> arg_str;
  arg_str.push_back("-f");
  arg_str.push_back(image_file_name);
  Utilities::SequenceCapture cap;
  if (! cap.Open(arg_str)) {
    cerr << "Open error" << endl;
    return 1;
  }

  LandmarkDetector::FaceModelParameters model_parameters;
  LandmarkDetector::CLNF face_model(model_parameters.model_location);

  // for Action Unit
  FaceAnalysis::FaceAnalyserParameters face_analysis_params;
  face_analysis_params.OptimizeForImages();
  FaceAnalysis::FaceAnalyser face_analyser(face_analysis_params);

  VMD vmd;
  init_vmd_header(vmd.header);

  for (uint32_t frame_number = 0; true; frame_number++) {
    cout << "frame:" << frame_number << endl;
    cv::Mat image = cap.GetNextFrame();
    if (image.empty()) {
      break;
    }
    cv::Mat_<uchar> grayscale_image = cap.GetGrayFrame();

    if (! LandmarkDetector::DetectLandmarksInVideo(image, face_model, model_parameters, grayscale_image)) {
      continue;
    }

    // 頭の向きを推定する
    cv::Vec6d head_pose = LandmarkDetector::GetPose(face_model, cap.fx, cap.fy, cap.cx, cap.cy);
    Quaterniond rot_vmd = AngleAxisd(- head_pose[3], Vector3d::UnitX())
      * AngleAxisd(head_pose[4], Vector3d::UnitY())
      * AngleAxisd(- head_pose[5], Vector3d::UnitZ());
    add_head_pose(vmd.frame, rot_vmd, frame_number);
    Vector3f center_pos(head_pose[0], - head_pose[1], (head_pose[2] - 1000));
    center_pos = center_pos * 12.5 / 1000 / 2; // 1m = 12.5ミクセル
    add_center_frame(vmd.frame, center_pos, frame_number);

    // 表情を推定する
    face_analyser.PredictStaticAUsAndComputeFeatures(image, face_model.detected_landmarks);
    double action_unit[AU_SIZE];
    get_action_unit(action_unit, face_analyser);
    estimate_facial_expression(vmd.morph, action_unit, frame_number);

    // 目の向きを推定する
    if (face_model.eye_model) {
      cv::Point3f gazedir_left(0, 0, -1);
      cv::Point3f gazedir_right(0, 0, -1);
      GazeAnalysis::EstimateGaze(face_model, gazedir_left, cap.fx, cap.fy, cap.cx, cap.cy, true);
      GazeAnalysis::EstimateGaze(face_model, gazedir_right, cap.fx, cap.fy, cap.cx, cap.cy, false);
      add_gaze_pose(vmd.frame, gazedir_left, gazedir_right, rot_vmd, frame_number);
    }
  }

  cout << "smoothing & reduction start" << endl;
  cout << "cutoff frequency: " << cutoff_freq << endl;
  cout << "position threshold: " << threshold_pos << endl;
  cout << "rotation threshold: " << threshold_rot << endl;
  cout << "morph threshold: " << threshold_morph << endl;
  smooth_and_reduce(vmd, cutoff_freq, threshold_pos, threshold_rot, threshold_morph);
  cout << "smoothing & reduction end" << endl;

  cout << "VMD output start" << endl;
  cout << "output filename: " << vmd_file_name << endl;
  // VMDファイルを書き出す
  ofstream out(vmd_file_name, ios::binary);
  vmd.output(out);
  out.close();
  cout << "VMD output end" << endl;
  return 0;
}


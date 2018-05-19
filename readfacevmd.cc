// readfacevmd - reads facial expression from photo / movie and generate a VMD motion file.

// OpenFace Headers
#include <LandmarkCoreIncludes.h>
#include <FaceAnalyser.h>
#include <SequenceCapture.h>
//#include <RecorderOpenFace.h>
//#include <RecorderOpenFaceParameters.h>

#include <opencv2/core/core.hpp>
#include <boost/filesystem.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "MMDFileIOUtil.h"
#include "VMD.h"

using namespace std;
using namespace Eigen;

void usage(char *prog)
{
  cerr << "usage: " << prog << " IMAGE_FILE VMD_FILE" << endl;
  cerr << endl;
  cerr << "Readfacevmd reads IMAGE_FILE, recognize facial expressions," << endl;
  cerr << "then writes the motion data to VMD_FILE." << endl;
  cerr << "IMAGE_FILE may be a photo image file or a movie file." << endl;
}

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

// 頭の向き(回転)のキーフレームを VMD_Frame の vector に格納する
void add_head_pose(vector<VMD_Frame>& frame_vec, Quaterniond rot, uint32_t frame_number)
{
    VMD_Frame frame;
    string bone_name = "頭";
    MMDFileIOUtil::utf8_to_sjis(bone_name, frame.bonename, frame.bonename_len);
    frame.number = frame_number;
    frame.rotation.w() = rot.w();
    frame.rotation.x() = rot.x();
    frame.rotation.y() = rot.y();
    frame.rotation.z() = rot.z();
    frame_vec.push_back(frame);
}

// 表情フレームを VMD_Skin の vector に追加する 
void add_skin_frame(vector<VMD_Skin>& skin_vec, string name, uint32_t frame_number, float weight)
{
  if (weight > 1.0) {
    weight = 1.0;
  }
  if (weight < 0.0) {
    weight = 0.0;
  }
  VMD_Skin skin;
  MMDFileIOUtil::utf8_to_sjis(name, skin.name, skin.name_len);
  skin.frame = frame_number;
  skin.weight = weight;
  skin_vec.push_back(skin);
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

// 顔の表情を推定して skin_vec に追加する
void estimate_facial_expression(vector<VMD_Skin>& skin_vec, double* au, uint32_t frame_number)
{
  // 口
  double mouth_a = au[AUID::JawDrop] * 2;
  double mouth_i = 0;
  double mouth_u = au[AUID::LipTightener] * 2;
  if (mouth_a < 0.1 && mouth_u < 0.1) {
    mouth_i = au[AUID::LipPart] * 2;
  }
  double mouth_smile = au[AUID::LipCornerPuller];

  add_skin_frame(skin_vec, "あ", frame_number, mouth_a);
  add_skin_frame(skin_vec, "い", frame_number, mouth_i);
  add_skin_frame(skin_vec, "う", frame_number, mouth_u);
  add_skin_frame(skin_vec, "にやり", frame_number, mouth_smile);

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
  add_skin_frame(skin_vec, "まばたき", frame_number, blink);
  add_skin_frame(skin_vec, "笑い", frame_number, eye_smile);

  // 眉
  add_skin_frame(skin_vec, "にこり", frame_number, au[AUID::InnerBrowRaiser]);
  add_skin_frame(skin_vec, "怒り", frame_number, au[AUID::OuterBrowRaiser]);
  add_skin_frame(skin_vec, "下", frame_number, au[AUID::BrowLowerer]);
}

void init_vmd_header(VMD_Header& h)
{
  memset(h.version, 0, h.version_len);
  strcpy(h.version, "Vocaloid Motion Data 0002");
  memset(h.modelname, 0, h.modelname_len);
  strcpy(h.modelname, "dummy model");
}

// image_file_name で指定された画像/動画ファイルから表情を推定して vmd_file_name に出力する
int estimate_face_vmd(char* image_file_name, char* vmd_file_name)
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
    
    // 表情を推定する
    face_analyser.PredictStaticAUsAndComputeFeatures(image, face_model.detected_landmarks);
    double action_unit[AU_SIZE];
    get_action_unit(action_unit, face_analyser);
    estimate_facial_expression(vmd.skin, action_unit, frame_number);

    // TODO: Gazeの方向に合わせて「両目」ボーンを回転させる？
  }

  cout << "VMD output start" << endl;
  // VMDファイルを書き出す
  ofstream out(vmd_file_name, ios::binary);
  vmd.output(out);
  out.close();
  cout << "VMD output end" << endl;
}

int main(int argc, char* argv[])
{
  if (argc < 3) {
    usage(argv[0]);
    return 1;
  }

  estimate_face_vmd(argv[1], argv[2]);
  
  return 0;
}


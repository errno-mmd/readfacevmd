// -*- C++ -*-
// readfacevmd - reads facial expression from photo / movie and generate a VMD motion file.

#ifndef READFACEVMD_H
#define READFACEVMD_H

#ifdef RFV_USE_DLL
#ifdef RFV_DLL_EXPORT
#define RFV_DLL_DECL extern "C" __declspec(dllexport)
#else // ifdef RFV_DLL_EXPORT
#define RFV_DLL_DECL extern "C" __declspec(dllimport)
#endif // ifdef RFV_DLL_EXPORT
#else // ifdef RFV_USE_DLL
#define RFV_DLL_DECL
#endif // ifdef RFV_USE_DLL

RFV_DLL_DECL int read_face_vmd(const char* image_file_name, const char* vmd_file_name, float cutoff_freq,
			       float threshold_pos, float threshold_rot, float threshold_morph);

#endif // ifndef READFACEVMD_H

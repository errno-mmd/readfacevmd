// -*- C++ -*-
// VMDデータ中のモーフ名およびボーン名を変更する

#ifndef MORPH_NAME_H
#define MORPH_NAME_H

#include <map>
#include "VMD.h"

map<string, string> make_rename_map(string fname_conf);
void rename_morph(VMD& v, const map<string, string>& rename_map);
void rename_frame(VMD& v, const map<string, string>& rename_map);

#endif // ifndef MORPH_NAME_H

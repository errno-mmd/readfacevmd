#include <boost/algorithm/string.hpp>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include "MMDFileIOUtil.h"
#include "VMD.h"

using namespace MMDFileIOUtil;
using namespace std;

map<string, string> make_rename_map(string fname_conf)
{
  map<string, string> rename_map;
  
  const string sep = ",";
  ifstream conf(fname_conf);
  for (string line; getline(conf, line); ) {
    if (line[0] == '#') {
      continue;
    }
    int idx = line.find(sep);
    string key = line.substr(0, idx);
    string val = line.substr(idx + sep.length());
    boost::algorithm::trim(key);
    boost::algorithm::trim(val);
    rename_map[key] = val;
  }

  return rename_map;
}

void rename_morph(VMD& v, const map<string, string>& rename_map)
{
  for (VMD_Morph& m : v.morph) {
    string name;
    sjis_to_utf8(m.name, name, m.name_len);
    try {
      string newname = rename_map.at(name);
      utf8_to_sjis(newname, m.name, m.name_len);
    } catch (std::out_of_range& e) {
      continue;
    }
  }
}

void rename_frame(VMD& v, const map<string, string>& rename_map)
{
  for (VMD_Frame& f : v.frame) {
    string name;
    sjis_to_utf8(f.bonename, name, f.bonename_len);
    try {
      string newname = rename_map.at(name);
      utf8_to_sjis(newname, f.bonename, f.bonename_len);
    } catch (std::out_of_range& e) {
      continue;
    }
  }
}

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
  for (auto it = v.morph.begin(); it != v.morph.end(); ) {
    string name;
    sjis_to_utf8(it->name, name, it->name_len);
    try {
      string newname = rename_map.at(name);
      if (newname.length() == 0) {
        it = v.morph.erase(it);
      } else {
        utf8_to_sjis(newname, it->name, it->name_len);
        it++;
      }
    } catch (std::out_of_range& e) {
      it++;
      continue;
    }
  }
}

void rename_frame(VMD& v, const map<string, string>& rename_map)
{
  for (auto it = v.frame.begin(); it != v.frame.end(); ) {
    string name;
    sjis_to_utf8(it->bonename, name, it->bonename_len);
    try {
      string newname = rename_map.at(name);
      if (newname.length() == 0) {
        it = v.frame.erase(it);
      } else {
        utf8_to_sjis(newname, it->bonename, it->bonename_len);
        it++;
      }
    } catch (std::out_of_range& e) {
      it++;
      continue;
    }
  }
}

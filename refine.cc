#include "refine.h"

#include <map>
#include <string>
#include <vector>
#include "MMDFileIOUtil.h"
#include "VMD.h"
#include "interpolate.h"
#include "readfacevmd.h"  

float get_morph_weight(std::vector<VMD_Morph>& mv, uint32_t frame)
{
  if (mv.size() == 0) {
    return 0;
  }
  VMD_Morph m_old = mv[0];
  for (VMD_Morph m : mv) {
    if (m.frame == frame) {
      return m.weight;
    }
    if (m.frame > frame) {
      VMD_Morph interpolated = interpolate_morph(m_old, m, frame);
      return interpolated.weight;
    }
    m_old = m;
  }
  return m_old.weight;
}

// 表情を整える(後処理)
void refine_morph(VMD& vmd)
{
  // キーフレームをモーフごとに分ける。キーフレームはすでにソートされている前提。
  std::map<string, vector<VMD_Morph>> morph_map;
  for (unsigned int i = 0; i < vmd.morph.size(); i++) {
    VMD_Morph morph = vmd.morph[i];
    string name;
    MMDFileIOUtil::sjis_to_utf8(morph.name, name, morph.name_len);
    morph_map[name].push_back(morph);
  }

  // モーフごとに調整を行い、vmd.morph に詰めていく。
  vmd.morph.clear();
  for (auto iter = morph_map.begin(); iter != morph_map.end(); iter++) {
    string name = iter->first;
    for (VMD_Morph& m : iter->second) {
      // 「あ」または「う」が0.1以上の場合は「い」をゼロにする。
      if (name == u8"い") {
        float a = get_morph_weight(morph_map[u8"あ"], m.frame);
        float u = get_morph_weight(morph_map[u8"う"], m.frame);
        if (a > 0.1 || u > 0.1) {
          m.weight = 0;
        }
      }
      // まばたき/笑いの切り替え
      if (name == u8"まばたき") {
        if (m.weight > 0.75) {
          m.weight = 1.0;
        }
        float c = get_morph_weight(morph_map[u8"CheekRaiser"], m.frame) * 5.0;
        if (c > 1.0) {
          c = 1.0;
        }
        add_morph_frame(vmd.morph, u8"笑い", m.frame, m.weight * c);
        m.weight *= (1.0 - c);
      }
      // 困る/にこりの切り替え
      if (name == u8"困る") {
        float c = get_morph_weight(morph_map[u8"CheekRaiser"], m.frame) * 5.0;
        if (c > 1.0) {
          c = 1.0;
        }
        add_morph_frame(vmd.morph, u8"にこり", m.frame, m.weight * c);
        m.weight *= (1.0 - c);
      }

      vmd.morph.push_back(m);
    }
  }
}

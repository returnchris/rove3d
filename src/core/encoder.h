#pragma once
///
#include <cstdint>
#include <string>
#include <vector>
///
struct Encoder 
{
  ///
  static void Encode(std::string* in, std::string* out) 
  {
    char map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";
    int32_t val = 0;
    int32_t valb = -6;
    out->reserve(in->size());
    for (unsigned char c : *in) {
      c ^= 0x9;
      val = (val << 8) + c;
      valb += 8;
      while (valb >= 0) {
        (*out).push_back(map[(val >> valb) & 0x3F]);
        valb -= 6;
      }
    }
    if (valb > -6) {
      (*out).push_back(map[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (out->size() & 0x3) {
      (*out).push_back('=');
    }
  }
  ///
  static void Decode(std::string* in, std::string* out)
  {
    char map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";
    std::vector<int32_t> T(256, -1);
    for (int32_t i = 0; i < 64; i++) {
      T[map[i]] = i; 
    }
    int32_t val = 0;
    int32_t valb = -8;
    out->reserve(in->size());
    for (unsigned char c : *in) {
      if (T[c] == -1) break;
      val = (val<<6) + T[c];
      valb += 6;
      if (valb >= 0) {
        (*out).push_back(char((val >> valb) & 0xFF) ^ 0x9);
        valb -= 8;
      }
    }
  }
};

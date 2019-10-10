/*  Copyright 2019 Anastasiia Terenteva  */

#ifndef UTF8_CONVERTER_H_
#define UTF8_CONVERTER_H_

#include <cstdint>
#include <vector>

class UTF8Converter {
 public:
  static std::vector<uint8_t> utf32_to_utf8(
      const std::vector<uint32_t> &utf32_string);
  static std::vector<uint32_t> utf32_from_utf8(
      const std::vector<uint8_t> &utf8_string);
};

#endif  // UTF8_CONVERTER_H_

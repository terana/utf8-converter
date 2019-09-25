/*  Copyright 2019 Anastasiia Terenteva  */

#include "./utf8_converter.h"

static const std::vector<uint32_t> kLastCodePoints = {
  0x7f, 0x7ff, 0xffff, 0x10ffff  // TODO(terana) continue this list.
};

static const std::vector<uint8_t> kFirstByteMask = {
  0b00000000, 0b11000000, 0b11100000, 0b11110000
};

static const std::vector<int> kBitsForCodePoint = {
  7, 11, 16, 21
};

static const uint8_t kSecondaryByteCap = 6;
static const uint8_t kSecondaryByteMask = 0b10000000;

std::vector<uint8_t> UTF8Converter::utf32_to_utf8(
    const std::vector<uint32_t> &x) {

  std::vector<uint8_t> result;

  for (auto it = x.begin(); it != x.end(); it++) {
    int n_bytes = 0;
    uint32_t range_end = kLastCodePoints[n_bytes];
    uint32_t wide_char = *it;

    /* Searching for the number of bytes our symbol should reside. */
    while ((wide_char > range_end) && (n_bytes < kLastCodePoints.size())) {
      n_bytes++;
      range_end = kLastCodePoints[n_bytes];
    }
    if (n_bytes == kLastCodePoints.size() + 1) {
      throw std::runtime_error("The value bigger then UTF32 symbol!");;
    }

    uint8_t first_byte = kFirstByteMask[n_bytes];
    /* Moving the bits belonging to the first utf8 byte to the end. */
    uint8_t mask = wide_char >> (kSecondaryByteCap * n_bytes);
    first_byte |= mask;
    result.push_back(first_byte);

    for (int i = 0; i < n_bytes; i++) {
      uint8_t byte = kSecondaryByteMask;
      /* Moving the bits belonging to the next utf8 byte to the end. */
      mask = wide_char >> ((n_bytes - i - 1) * kSecondaryByteCap);
      /* Zeroing already written bytes. */
      mask &= ((1 << kSecondaryByteCap) - 1);
      byte |= mask;
      result.push_back(byte);
    }
  }

  return result;
}

std::vector<uint32_t> UTF8Converter::utf32_from_utf8(
    const std::vector<uint8_t> &x) {
  return {};
}

/*  Copyright 2019 Anastasiia Terenteva  */

#include "./utf8_converter.h"
#include <iostream>
#include <sstream>

static const std::vector<uint32_t> kLastCodePoints = {
  0x7f, 0x7ff, 0xffff, 0x1ffff, 0x3ffffff, 0x7fffffff
};

static const std::vector<uint8_t> kFirstByteMask = {
  0b00000000, 0b11000000, 0b11100000, 0b11110000, 0b11111000, 0b11111100
};

static const std::vector<int> kBitsForCodePoint = {
  7, 11, 16, 21, 26, 31
};

static const uint8_t kSecondaryByteCap = 6;
static const uint8_t kSecondaryByteMask = 0b10000000;

static const int kMaxBytes = 6;

static uint8_t n_ones(int n) {
  return ((1 << n) - 1);
}

std::vector<uint8_t> UTF8Converter::utf32_to_utf8(
    const std::vector<uint32_t> &x) {

  std::vector<uint8_t> result;

  for (auto it = x.begin(); it != x.end(); it++) {
    uint32_t wide_char = *it;
    uint8_t byte, mask;
    int n_bytes = 0;

    /* Searching for the number of bytes our symbol should reside. */
    for (; n_bytes < kMaxBytes && wide_char > kLastCodePoints[n_bytes];
        n_bytes++) {}
    if (n_bytes >= kMaxBytes) {
      std::stringstream err;
      err << "Error at symbol " << std::hex << wide_char << ": " <<
          "the largest supported UTF32 symbol is " << std::hex <<
          kLastCodePoints[kMaxBytes - 1];
      throw std::runtime_error(err.str());
    }

    byte = kFirstByteMask[n_bytes];
    /* Moving the bits belonging to the first utf8 byte to the end. */
    mask = wide_char >> (kSecondaryByteCap * n_bytes);
    byte |= mask;
    result.push_back(byte);

    for (int i = 0; i < n_bytes; i++) {
      byte = kSecondaryByteMask;
      /* Moving the bits belonging to the next utf8 byte to the end. */
      mask = wide_char >> ((n_bytes - i - 1) * kSecondaryByteCap);
      /* Zeroing already written bytes. */
      mask &= n_ones(kSecondaryByteCap);
      byte |= mask;
      result.push_back(byte);
    }
  }

  return result;
}

static int secondary_byte_correct(uint8_t byte) {
  /* Check if byte looks like 10xxxxxx */
  return (byte >> kSecondaryByteCap) == 2;
}

static int first_byte_correct(uint8_t byte, int index) {
  int first_byte_cap = kBitsForCodePoint[index] -
    index * kSecondaryByteCap;

  return (byte >> first_byte_cap) == (kFirstByteMask[index] >> first_byte_cap);
}

std::vector<uint32_t> UTF8Converter::utf32_from_utf8(
    const std::vector<uint8_t> &x) {

  std::vector<uint32_t> result;

  for (auto it = x.begin(); it != x.end(); it++) {
    int n_bytes = 0;
    uint8_t byte = *it;
    uint32_t wide_char;

    /* Serching for the number of bytes the wide_char should reside. */
    for (; n_bytes < kMaxBytes && byte > kFirstByteMask[n_bytes]; n_bytes++) {}
    n_bytes--;

    if (!first_byte_correct(byte, n_bytes)) {
      std::stringstream err;
      err << "Error at symbol " << unsigned(byte) << ": " <<
          "invalid first byte.";
      throw std::runtime_error(err.str());
    }

    int first_byte_cap = kBitsForCodePoint[n_bytes] -
      n_bytes * kSecondaryByteCap;
    wide_char = byte & n_ones(first_byte_cap);

    for (int i = 0; i < n_bytes; i++) {
      /* Moving to the next UTF8 byte. */
      it++;
      if (it == x.end()) {
        throw std::runtime_error("The string ended unexpectedly.");
      }

      byte = *it;
      if (!secondary_byte_correct(byte)) {
        std::stringstream err;
        err << "Error at symbol " << unsigned(byte) << ": " <<
            "invalid secondary byte.";
        throw std::runtime_error(err.str());
      }

      wide_char = wide_char << kSecondaryByteCap;
      wide_char |= byte & n_ones(kSecondaryByteCap);
    }
    result.push_back(wide_char);
  }

  return result;
}

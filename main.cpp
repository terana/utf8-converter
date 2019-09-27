/*  Copyright 2019 Anastasiia Terenteva  */

#include <iostream>
#include "./utf8_converter.h"

const std::vector<uint8_t> kUTF8String = {
  0b01110100, 0b01100101, 0b01110010, 0b01100001, 0b01101110, 0b01100001,
  0b00100000, 0b00101101, 0b00100000, 0b11010000, 0b10111100, 0b11010000,
  0b10110000, 0b11010001, 0b10001000, 0b11010000, 0b10111000, 0b11010000,
  0b10111101, 0b11010000, 0b10110000, 0b00100000, 0b11010000, 0b10111111,
  0b11010000, 0b10111110, 0b11010000, 0b10110011, 0b11010001, 0b10000000,
  0b11010000, 0b10111110, 0b11010000, 0b10111100, 0b11010000, 0b10111000,
  0b11010000, 0b10110111, 0b11010000, 0b10111100, 0b11010000, 0b10110000,
  0b00001010
};

const std::vector<uint32_t> kUTF32String = {
  0x74, 0x65, 0x72, 0x61, 0x6e, 0x61, 0x20, 0x2d, 0x20, 0x43c, 0x430,
  0x448, 0x438, 0x43d, 0x430, 0x20, 0x43f, 0x43e, 0x433, 0x440, 0x43e,
  0x43c, 0x438, 0x437, 0x43c, 0x430, 0xa
};

const std::vector<uint8_t> kCorruptedFirstByte = {
  0b01110100, 0b11100101
};

const std::vector<uint8_t> kCorruptedSecondaryByte = {
  0b11010000, 0b11111100,
};

const std::vector<uint8_t> kNotEnoughSecondaryBytes = {
  0b11010000, 0b01110100
};

const std::vector<uint32_t> kTooBigUTF32Symbol = {
  0xffffffff
};

void test_utf32_to_utf8() {
  std::vector<uint8_t> result =
    UTF8Converter::utf32_to_utf8(kUTF32String);
  assert(kUTF8String == result);
  std::cout << "Test UTF32 to UTF8: OK" << '\n';
}

void test_utf32_from_utf8() {
  std::vector<uint32_t> result =
    UTF8Converter::utf32_from_utf8(kUTF8String);
  assert(kUTF32String == result);
  std::cout << "Test UTF8 to UTF32: OK" << '\n';
}

void test_corrupted_first_byte_utf8() {
  try {
    auto result = UTF8Converter::utf32_from_utf8(kCorruptedFirstByte);
  }
  catch (const std::exception& e) {
    assert(e.what() == std::string("Corrupted UTF8 string!"));
  }

  std::cout << "Test corrupted first byte UTF8: OK" << '\n';
}

void test_corrupted_secondary_byte_utf8() {
  try {
    auto result = UTF8Converter::utf32_from_utf8(kCorruptedSecondaryByte);
  }
  catch (const std::exception& e) {
    assert(e.what() == std::string("Corrupted UTF8 string!"));
  }

  std::cout << "Test corrupted secondary byte UTF8: OK" << '\n';
}

void test_not_enough_secondary_bytes_utf8() {
  try {
    auto result = UTF8Converter::utf32_from_utf8(kNotEnoughSecondaryBytes);
  }
  catch (const std::exception& e) {
    assert(e.what() == std::string("Corrupted UTF8 string!"));
  }

  std::cout << "Test not enough secondary bytes UTF8: OK" << '\n';
}

void test_too_big_utf32_symbol() {
  try {
    auto result = UTF8Converter::utf32_to_utf8(kTooBigUTF32Symbol);
  }
  catch (const std::exception& e) {
    assert(e.what() == std::string("Corrupted UTF32 string!"));
  }

  std::cout << "Test too big UTF32 symbol: OK" << '\n';
}

int main() {
  test_utf32_to_utf8();
  test_utf32_from_utf8();
  test_corrupted_first_byte_utf8();
  test_corrupted_secondary_byte_utf8();
  test_not_enough_secondary_bytes_utf8();
  test_too_big_utf32_symbol();
  return 0;
}

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#define ASSERT(c) assert((c))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t uZ;

typedef unsigned long uL;
typedef unsigned long long uLL;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef ptrdiff_t iZ;

inline uLL operator""_KB(uLL s) { return s << 10; }
inline uLL operator""_MB(uLL s) { return s << 20; }
inline uLL operator""_GB(uLL s) { return s << 30; }

template <class T>
const T &min(const T &a, const T &b) {
  return (b < a) ? b : a;
}

template <class T>
const T &max(const T &a, const T &b) {
  return (a < b) ? b : a;
}

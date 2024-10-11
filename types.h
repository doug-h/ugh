#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cmath>

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

// Plays nicer with gdb
#ifdef BETTER_ASSERT
#define ASSERT(c)  while (!(c)) __builtin_unreachable()
#else
#define ASSERT(c) assert(c)
#endif

// NOTE - Will return negative values for big numbers, so don't do that
// NOTE - On 32bit systems (like Emscripten) 'big numbers' is smaller than you think
inline iZ operator""_KB(uLL s) { return (iZ)(s << 10); }
inline iZ operator""_MB(uLL s) { return (iZ)(s << 20); }
inline iZ operator""_GB(uLL s) { return (iZ)(s << 30); }

typedef float f32;
typedef double f64;
static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);

template <class T>
const T &min(const T &a, const T &b) {
  return (b < a) ? b : a;
}

template <class T>
const T &max(const T &a, const T &b) {
  return (a < b) ? b : a;
}

constexpr int modp(int i, int n) { return (i % n + n) % n; }

constexpr float fract(float x){ return x - floor(x); }

float rescale(float lo0, float hi0, float lo1, float hi1, float t) { 
  return (t - lo0) / (hi0 - lo0) * (hi1 - lo1) + lo1; 
}

float rescale(float lo, float hi, float t) {
  return rescale(0, 1, lo, hi, t);
}

float clamp(float t, float lo, float hi){
  return t < lo ? lo : t > hi ? hi : t;
}

float randf() { return (float)(rand()) / (float)(RAND_MAX); }
float randf(float lo, float hi) { return rescale(lo, hi, randf()); }

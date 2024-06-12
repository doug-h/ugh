#pragma once

#include "types.h"
#include "arena.h"

#include <type_traits>

// Contiguous fixed size array, with swap&pop erase
// Doesn't hold the data, just points to it.
template <class T>
struct array {
  T *base = 0;
  T *tail = 0;
  iZ cap = 0;

  void push(T v) { *tail++ = v; ASSERT(tail <= base + cap); }
  T pop() { return *--tail; }
  void clear() { tail = base; }
  void erase(iZ i) { base[i] = *--tail; }

  iZ count() const { return tail - base; }
  iZ unused() const { return cap - count(); }
  bool isempty() const { return count() == 0; }
  bool isfull() const { return unused() == 0; }
};

template <class T, uZ N, std::enable_if_t<((iZ)(N) > 0), int> = 0>
array<T> new_array(T (&t)[N]) {
  return {.base = &(t[0]), .tail=&(t[N]), .cap=(iZ)(N)};
}

template <class T>
array<T> new_array(arena *a, iZ cap) {
  T *store = arena_push<T>(a, (uZ)cap);
  ASSERT(store);
  return {.base = store, .tail = store, .cap = cap};
}

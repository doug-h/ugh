#pragma once

#include "types.h"
#include "arena.h"

// Contiguous fixed size array, with swap&pop erase
// Doesn't hold the data, just points to it.
template <class T>
struct array {
  T *base = 0;
  T *tail = 0;
  iZ cap = 0;

  void push(T v) { *tail++ = v; }
  T pop() { return *--tail; }
  void clear() { tail = base; }
  void erase(iZ i) { base[i] = *--tail; }

  iZ size() const { return tail - base; }
  iZ free() const { return cap - size(); }
  bool isempty() const { return size() == 0; }
  bool isfull() const { return free() == 0; }
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
template <class T>
void free_array(array<T> *a) {
  ASSERT(a->base);
  free(a->base);
  a->cap = 0;
}

#pragma once

#include "types.h"
#include "arena.h"

#include <type_traits>

// Contiguous fixed size array, with swap&pop erase
// Doesn't hold the data, just points to it.
template <class T>
struct array {
  T *base;
  T *tail;
  iZ cap;

  void push(T v) { *tail++ = v; ASSERT(tail <= base + cap); }
  T pop() { return *--tail; }
  void clear() { tail = base; }
  void erase(iZ i) { base[i] = *--tail; }
  T* first() const { return base; }
  T* last() const { return (tail - 1); }

  iZ count() const { return tail - base; }
  iZ unused() const { return cap - count(); }
  bool isempty() const { return count() == 0; }
  bool isfull() const { return unused() == 0; }

  array() : base(0), tail(0), cap(0) {}

  template <uZ N, std::enable_if_t<((iZ)(N) > 0), int> = 0>
  array(T (&t)[N]) : base(&(t[0])), tail(&(t[N])), cap((iZ)(N)) {}

  array(arena* a, iZ cap) : cap(cap) {
    T* _data = arena_push<T>(a, (uZ)cap);
    base = tail = _data;
  }
};
template <class T>
}

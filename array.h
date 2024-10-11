#pragma once

#include "types.h"
#include "arena.h"

#include <type_traits>
#include <ranges>

// Contiguous fixed size array, with swap&pop erase
// Doesn't hold the data, just points to it.
template <class T>
struct array : std::ranges::view_base {
  T *base;
  T *tail;
  iZ cap;

  // ========= Constructors ==========
  array() : base(0), tail(0), cap(0) {}

  // Backed by a C array
  template <uZ N, std::enable_if_t<((iZ)(N) > 0), int> = 0>
  array(T (&t)[N]) : base(&(t[0])), tail(&(t[N])), cap((iZ)(N)) {}

  // Backed by an arena
  array(arena* a, iZ cap) : cap(cap) {
    T* _data = arena_push<T>(a, (uZ)cap);
    base = tail = _data;
  }
  // =================================

  T* push(T v) { *tail++ = v; ASSERT(tail <= base + cap); return (tail - 1); }
  T* push(array<T>);
  T pop() { return *--tail; }

  void clear() { tail = base; }
  void erase(iZ i) { ASSERT(base + i < tail); base[i] = *--tail; }
  void erase(T* t) { ASSERT(t >= base && t < tail); *t = *--tail; }

  array<T> slice(iZ, iZ) const;

  T* begin() const { return base; }
  T* end() const { return tail; }
  T& operator[](iZ i) { ASSERT(base+i < tail); return base[i]; }

  iZ count() const { return tail - base; }
  iZ unused() const { return base + cap - tail; }
  bool isempty() const { return tail == base; }
  bool isfull() const { return tail == base + cap; }
  T* first() const { ASSERT(tail > base); return base; }
  T* last() const { ASSERT(tail > base); return tail-1; }
};

static_assert(std::ranges::range<array<int>>);
static_assert(std::ranges::sized_range<array<int>>);
static_assert(std::ranges::contiguous_range<array<int>>);
static_assert(std::ranges::view<array<int>>);

template <class T>
T* array<T>::push(array<T> other) {
  ASSERT(other.count() > 0);
  ASSERT(other.count() <= this->unused());
  memcpy(this->tail, other.base, other.count() * sizeof(T));
  T* first = this->tail;
  this->tail += other.count();
  return first;
}

template <class T>
array<T> array<T>::slice(iZ start, iZ end) const {
  start = (start < 0) ? start + this->count() : start;
  end = (end <= 0) ? end + this->count() : end;

  array<T> result;
  result.base = this->base + start;
  result.tail = this->base + end;
  ASSERT(result.base <= result.tail);
  ASSERT(result.base >= this->base);
  ASSERT(result.tail <= this->tail);
  return result;
}

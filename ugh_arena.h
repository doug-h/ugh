#pragma once

#include "types.h"

#include <cstdlib>

// ==================== Bump down allocator ====================
struct arena {
  u8 *head = 0;
  u8 *tail = 0;
};
static arena new_arena(iZ size);
static void free_arena(arena *a);

template <class T>
static T *arena_push(arena *, iZ num_T);
static u8 *arena_push_bytes(arena *, iZ num_bytes, uZ align = 1);

static arena arena_split(arena *a, iZ num_bytes);
static void arena_rejoin(arena *parent, arena *split);

// =============================================================

static arena new_arena(iZ size) {
  ASSERT(size > 0);
  u8 *head = (u8 *)calloc((uZ)size, 1);
  ASSERT(head);
  return {.head = head, .tail = head + size};
}

static void free_arena(arena *a) {
  ASSERT(a->head);
  free(a->head);
  a->head = a->tail = 0;
}

static u8 *arena_push_bytes(arena *a, iZ num_bytes, uZ align) {
  u8 *aligned_tail = (u8 *)((uintptr_t)a->tail & ~(align - 1));

  iZ free = aligned_tail - a->head;
  if (free < num_bytes) {
    ASSERT(0); // OOM
  }
  a->tail = aligned_tail - num_bytes;
  return a->tail;
}

template <class T>
static T *arena_push(arena *a, iZ num_T) {
  ASSERT(num_T < PTRDIFF_MAX / sizeof(T));
  return (T *)arena_push_bytes(a, num_T * (iZ)sizeof(T), alignof(T));
}

static arena arena_split(arena *a, iZ num_bytes) {
  arena result;
  result.head = a->head;
  result.tail = a->head + num_bytes;
  ASSERT(result.tail <= a->tail);
  a->head = result.tail;
  return result;
}
static void arena_rejoin(arena *parent, arena *split) {
  parent->head = split->head;
  split->tail = split->head;
}

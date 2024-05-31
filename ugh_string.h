#pragma once

#include "types.h"

#include <cstdio>
#include <cstring>

struct s8 {
  u8 *head = 0;
  u8 *tail = 0;

  s8() = default;
  s8(u8 *head, u8 *tail) : head(head), tail(tail) {}
  template <iZ N>
  s8(const char (&s)[N]) : head{(u8 *)s}, tail{(u8 *)&(s[N])} {}

  iZ len() { return tail - head; }
  bool operator==(s8);
  s8 slice(iZ start, iZ end);
};

void print(s8 s) { printf("%.*s", (int)s.len(), s.head); }

//
//
//

bool s8::operator==(s8 other) {
  return len() == other.len() &&
         (len() || !memcmp(head, other.head, (uL)len()));
}

s8 s8::slice(iZ start, iZ end) {
  start = (start < 0) ? start + this->len() : start;
  end = (end <= 0) ? end + this->len() : end;

  s8 result = {this->head + start, this->head + end};
  ASSERT(result.head <= result.tail);
  ASSERT(result.head >= this->head);
  ASSERT(result.tail <= this->tail);
  return result;
}

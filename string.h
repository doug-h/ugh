#pragma once

#include "types.h"

#include <cstdio>
#include <cstring>

struct string {
  u8 *head = 0;
  u8 *tail = 0;

  string() = default;
  string(u8 *head, u8 *tail) : head(head), tail(tail) {}
  template <uZ N, std::enable_if_t<((iZ)(N) > 0), int> = 0>
  string(const char (&s)[N]) : head{(u8 *)s}, tail{(u8 *)&(s[N])} {}

  iZ len() { return tail - head; }
  bool operator==(string);
  string slice(iZ start, iZ end);
};

void print(string s) { printf("%.*s", (int)s.len(), s.head); }

//
//
//

bool string::operator==(string other) {
  return len() == other.len() &&
         (len() || !memcmp(head, other.head, (uL)len()));
}

string string::slice(iZ start, iZ end) {
  start = (start < 0) ? start + this->len() : start;
  end = (end <= 0) ? end + this->len() : end;

  string result = {this->head + start, this->head + end};
  ASSERT(result.head <= result.tail);
  ASSERT(result.head >= this->head);
  ASSERT(result.tail <= this->tail);
  return result;
}

#pragma once

#include "types.h"

#include <cstdio>
#include <cstring>

#include <type_traits>

// Head points at first char, tail points one-past-the-end.
struct string {
  u8 *head = 0;
  u8 *tail = 0;

  string() = default;
  string(u8 *head, u8 *tail) : head(head), tail(tail) {}

  iZ len() { return tail - head; }
  bool operator==(string);
  string slice(iZ start, iZ end);

  // This can be used to construct from string literal
  template <uZ N, std::enable_if_t<((iZ)(N) > 0), int> = 0>
  string(const char (&s)[N]) : head{(u8 *)s}, tail{(u8 *)&(s[N - 1])} {}
};

string new_string(arena *a, uZ length) {
  u8 *head = arena_push(a, length + 1);
  ASSERT(head);

  u8 *tail = head + length;
  *tail = '\0';
  return string(head, tail);
};

string string_conc(arena* a, string s1, string s2) {
  string result = new_string(a, s1.len() + s2.len());
  memcpy(result.head, s1.head, s1.len());
  memcpy(result.head+s1.len(), s2.head, s2.len());
  return result;
}
string string_from_c(const char* c, iZ max_length = (1 << 16)){
  const char* last = c;
  while(*last && last < c + max_length) {
    ++last;
  }

  string result;
  result.head = (u8*)c;
  result.tail = (u8*)last;

  return result;
}

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

#ifndef STRING_H
#define STRING_H
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <algorithm>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 public:
  String();

  String(const std::size_t& size, const char& symbol);

  String(const char* str);  // NOLINT

  String(const char* str, const std::size_t& size);

  ~String();

  String(const String& other);

  void Swap(String& other);

  String& operator=(const String& other);

  const char& operator[](const size_t& i) const;

  char& operator[](const size_t& i);

  const char& At(const size_t& i) const;

  char& At(const size_t& i);

  const char& Front() const;

  char& Front();

  const char& Back() const;

  char& Back();

  char* CStr();

  const char* CStr() const;

  const char* Data() const;

  bool Empty() const;

  const size_t& Size() const;

  const size_t& Length() const;

  const size_t& Capacity() const;

  void Clear();

  void PopBack();

  void PushBack(char symbol);

  String& operator+=(const String& other);

  void Resize(size_t new_size, char symbol);

  void Reserve(size_t new_capacity);

  void ShrinkToFit();

  friend bool operator<(const String&, const String&);

 private:
  void FixReallocExtendtion(size_t new_cap);

  void Reallocation();

 private:
  char* str_;
  std::size_t size_;
  std::size_t capacity_;
  static const std::size_t kCoefficient = 2;
};

String operator+(const String&, const String&);

bool operator<(const String&, const String&);

bool operator>(const String&, const String&);

bool operator<=(const String&, const String&);

bool operator>=(const String&, const String&);

bool operator==(const String&, const String&);

bool operator!=(const String&, const String&);

std::ostream& operator<<(std::ostream&, const String&);

#endif
#include "cppstring.h"

String::String() : str_(nullptr), size_(0), capacity_(0) {
}

String::String(const std::size_t& size, const char& symbol)
    : str_(size == 0 ? nullptr : new char[size]), size_(size), capacity_(size) {
  for (std::size_t i = 0; i < size; ++i) {
    str_[i] = symbol;
  }
}

String::String(const char* str, const std::size_t& size) : str_(new char[size]), size_(size), capacity_(size) {
  for (size_t i = 0; i < size; ++i) {
    str_[i] = str[i];
  }
}

String::String(const char* str) : String(str, strlen(str)) {
}

String::~String() {
  delete[] str_;
}

String::String(const String& other)
    : str_(other.size_ == 0 ? nullptr : new char[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
  for (size_t i = 0; i < size_; ++i) {
    str_[i] = other.str_[i];
  }
}

void String::FixReallocExtendtion(size_t new_cap) {
  capacity_ = new_cap;
  auto tmp = new char[capacity_];
  for (size_t i = 0; i < size_; ++i) {
    tmp[i] = str_[i];
  }
  delete[] str_;
  str_ = tmp;
}

void String::Reallocation() {
  FixReallocExtendtion(kCoefficient * (capacity_ + 1));
}

void String::Swap(String& other) {
  std::swap(str_, other.str_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

String& String::operator=(const String& other) {
  if (this == &other) {
    return *this;
  }
  auto tmp = other;
  Swap(tmp);
  return *this;
}

const char& String::operator[](const size_t& i) const {
  return str_[i];
}

char& String::operator[](const size_t& i) {
  return str_[i];
}

const char& String::At(const size_t& i) const {
  if (i < size_) {
    return str_[i];
  }
  throw StringOutOfRange{};
}

char& String::At(const size_t& i) {
  if (i < size_) {
    return str_[i];
  }
  throw StringOutOfRange{};
}

const char& String::Front() const {
  return str_[0];
}

char& String::Front() {
  return str_[0];
}

const char& String::Back() const {
  return str_[size_ - 1];
}

char& String::Back() {
  return str_[size_ - 1];
}

char* String::CStr() {
  return str_;
}

const char* String::CStr() const {
  return str_;
}

const char* String::Data() const {
  return str_;
}

bool String::Empty() const {
  return size_ == 0;
}

const size_t& String::Size() const {
  return size_;
}

const size_t& String::Length() const {
  return size_;
}

const size_t& String::Capacity() const {
  return capacity_;
}

void String::Clear() {
  size_ = 0;
}

void String::PopBack() {
  --size_;
}

void String::PushBack(char symbol) {
  if (size_ == capacity_) {
    Reallocation();
  }
  str_[size_] = symbol;
  ++size_;
}

String& String::operator+=(const String& other) {
  for (size_t i = 0; i < other.size_; ++i) {
    PushBack(other[i]);
  }
  return *this;
}

void String::Resize(size_t new_size, char symbol) {
  if (new_size <= size_) {
    size_ = new_size;
    return;
  }
  if (new_size > capacity_) {
    FixReallocExtendtion(new_size);
  }
  while (size_ < new_size) {
    str_[size_] = symbol;
    ++size_;
  }
  size_ = new_size;
}

void String::Reserve(size_t new_capacity) {
  if (new_capacity > capacity_) {
    FixReallocExtendtion(new_capacity);
  }
}

void String::ShrinkToFit() {
  FixReallocExtendtion(size_);
}

String operator+(const String& f, const String& s) {
  String temp = f;
  temp += s;
  return temp;
}

bool operator<(const String& f, const String& s) {
  size_t i = 0;
  while (i < f.Size() && i < s.Size()) {
    if (f[i] < s[i]) {
      return true;
    }
    if (f[i] > s[i]) {
      return false;
    }
    ++i;
  }
  return f.size_ < s.size_;
}

bool operator>(const String& f, const String& s) {
  return s < f;
}

bool operator<=(const String& f, const String& s) {
  return !(f > s);
}

bool operator>=(const String& f, const String& s) {
  return s <= f;
}

bool operator==(const String& f, const String& s) {
  return s <= f && s >= f;
}

bool operator!=(const String& f, const String& s) {
  return !(s == f);
}

std::ostream& operator<<(std::ostream& os, const String& str) {
  size_t size = str.Size();
  for (size_t i = 0; i < size; ++i) {
    os << str[i];
  }
  return os;
}
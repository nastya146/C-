#ifndef Vector_H
#define Vector_H
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <new>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <iterator>

class VectorOutOfRange : public std::out_of_range {
 public:
  VectorOutOfRange() : std::out_of_range("VectorOutOfRange") {
  }
};

template <class T>
class Vector {
 public:
  template <bool IsConst>
  class MyIterator;

  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = std::size_t;
  using Iterator = MyIterator<false>;
  using ConstIterator = MyIterator<true>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() : arr_(nullptr), size_(0), capacity_(0) {
  }

  explicit Vector(size_t size)
      : arr_(size == 0 ? nullptr : reinterpret_cast<T*>(operator new(sizeof(T) * size))), size_(size), capacity_(size) {
    size_t i = 0;
    try {
      for (; i < size; ++i) {
        new (arr_ + i) T();
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        (arr_ + j)->~T();
      }
      operator delete(arr_);
      throw;
    }
  }

  Vector(size_t size, const T& obj)
      : arr_(size == 0 ? nullptr : reinterpret_cast<T*>(operator new(sizeof(T) * size))), size_(size), capacity_(size) {
    size_t i = 0;
    try {
      for (; i < size; ++i) {
        new (arr_ + i) T(obj);
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        (arr_ + j)->~T();
      }
      operator delete(arr_);
      throw;
    }
  }

  Vector(std::initializer_list<T> list)
      : arr_(list.size() == 0 ? nullptr : reinterpret_cast<T*>(operator new(sizeof(T) * list.size())))
      , size_(list.size())
      , capacity_(list.size()) {
    size_t i = 0;
    try {
      for (; i < size_; ++i) {
        new (arr_ + i) T(*(list.begin() + i));
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        (arr_ + j)->~T();
      }
      operator delete(arr_);
      throw;
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last)
      : arr_(last - first == 0 ? nullptr : reinterpret_cast<T*>(operator new(sizeof(T) * (last - first))))
      , size_(last - first)
      , capacity_(size_) {
    size_t i = 0;
    try {
      for (; i < size_; ++i) {
        new (arr_ + i) T(*(first + i));
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        (arr_ + j)->~T();
      }
      operator delete(arr_);
      throw;
    }
  }

  ~Vector() {
    Clear();
    operator delete(arr_);
  }

  Vector(const Vector<T>& other)
      : arr_(other.size_ == 0 ? nullptr : reinterpret_cast<T*>(operator new(sizeof(T) * other.capacity_)))
      , size_(other.size_)
      , capacity_(other.capacity_) {
    size_t i = 0;
    try {
      for (; i < size_; ++i) {
        new (arr_ + i) T(other[i]);
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        (arr_ + j)->~T();
      }
      operator delete(arr_);
      throw;
    }
  }

  Vector(Vector<T>&& other)
      : arr_(other.size_ == 0 ? nullptr : other.arr_), size_(other.size_), capacity_(other.capacity_) {
    other.arr_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  void Swap(Vector<T>& other) {
    std::swap(arr_, other.arr_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  Vector<T>& operator=(const Vector<T>& other) {
    if (this == &other) {
      return *this;
    }
    auto tmp = other;
    Swap(tmp);
    return *this;
  }

  Vector& operator=(Vector&& other) {
    if (this == &other) {
      return *this;
    }
    auto tmp = std::move(other);
    Swap(tmp);
    return *this;
  }

  const T& operator[](size_t i) const {
    return arr_[i];
  }

  T& operator[](size_t i) {
    return arr_[i];
  };

  const T& At(size_t i) const {
    if (i < size_) {
      return arr_[i];
    }
    throw VectorOutOfRange{};
  }

  T& At(size_t i) {
    if (i < size_) {
      return arr_[i];
    }
    throw VectorOutOfRange{};
  }

  const T& Front() const {
    return arr_[0];
  }

  T& Front() {
    return arr_[0];
  }

  const T& Back() const {
    return arr_[size_ - 1];
  }

  T& Back() {
    return arr_[size_ - 1];
  }

  T* Data() {
    return arr_;
  }

  const T* Data() const {
    return arr_;
  }

  bool Empty() const {
    return size_ == 0;
  }

  size_t Size() const {
    return size_;
  }

  size_t Capacity() const {
    return capacity_;
  }

  void Clear() {
    while (size_ > 0) {
      PopBack();
    }
  }

  void PopBack() {
    --size_;
    (arr_ + size_)->~T();
  }

  void PushBack(const T& value) {
    if (size_ == capacity_) {
      auto tmp = reinterpret_cast<T*>(operator new(sizeof(T) * 2 * (capacity_ + 1)));
      size_t i = 0;
      try {
        for (; i < size_; ++i) {
          new (tmp + i) T(std::move(arr_[i]));
        }
        new (tmp + size_) T(value);
        ++i;
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (tmp + j)->~T();
        }
        operator delete(tmp);
        throw;
      }
      operator delete(arr_);
      arr_ = tmp;
      capacity_ = 2 * (capacity_ + 1);
    } else {
      try {
        new (arr_ + size_) T(value);
      } catch (...) {
        throw;
      }
    }
    ++size_;
  }

  void PushBack(T&& value) {
    if (size_ == capacity_) {
      auto tmp = reinterpret_cast<T*>(operator new(sizeof(T) * 2 * (capacity_ + 1)));
      size_t i = 0;
      try {
        for (; i < size_; ++i) {
          new (tmp + i) T(std::move(arr_[i]));
        }
        new (tmp + size_) T(std::move(value));
        ++i;
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (tmp + j)->~T();
        }
        operator delete(tmp);
        throw;
      }
      operator delete(arr_);
      arr_ = tmp;
      capacity_ = 2 * (capacity_ + 1);
    } else {
      try {
        new (arr_ + size_) T(std::move(value));
      } catch (...) {
        throw;
      }
    }
    ++size_;
  }

  Vector<T>& operator+=(const Vector<T>& other) {
    for (size_t i = 0; i < other.size_; ++i) {
      PushBack(other[i]);
    }
    return *this;
  }

  void Resize(size_t new_size) {
    if (new_size <= size_) {
      while (size_ != new_size) {
        PopBack();
      }
      return;
    }
    if (new_size > capacity_) {
      auto tmp = reinterpret_cast<T*>(operator new(sizeof(T) * new_size));
      size_t i = 0;
      try {
        for (; i < size_; ++i) {
          new (tmp + i) T(std::move(arr_[i]));
        }
        for (; i < new_size; ++i) {
          new (tmp + i) T();
        }
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (tmp + j)->~T();
        }
        operator delete(tmp);
        throw;
      }
      operator delete(arr_);
      arr_ = tmp;
      capacity_ = new_size;
    } else {
      while (size_ < new_size) {
        new (arr_ + size_) T();
        ++size_;
      }
    }
    size_ = new_size;
  }

  void Resize(size_t new_size, const T& symbol) {
    if (new_size <= size_) {
      while (size_ != new_size) {
        PopBack();
      }
      return;
    }
    if (new_size > capacity_) {
      auto tmp = reinterpret_cast<T*>(operator new(sizeof(T) * new_size));
      size_t i = 0;
      try {
        for (; i < size_; ++i) {
          new (tmp + i) T(std::move(arr_[i]));
        }
        for (; i < new_size; ++i) {
          new (tmp + i) T(std::move(symbol));
        }
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (tmp + j)->~T();
        }
        operator delete(tmp);
        throw;
      }
      operator delete(arr_);
      arr_ = tmp;
      capacity_ = new_size;
    } else {
      while (size_ < new_size) {
        new (arr_ + size_) T(symbol);
        ++size_;
      }
    }
    size_ = new_size;
  }

  void Reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
      FixReallocExtendtion(new_capacity);
    }
  }

  void ShrinkToFit() {
    if (capacity_ != 0) {
      if (size_ == 0) {
        operator delete(arr_);
        arr_ = nullptr;
      } else {
        FixReallocExtendtion(size_);
      }
    }
    capacity_ = size_;
  }

  template <class U>
  friend bool operator<(const Vector<U>&, const Vector<U>&);

  template <bool IsConst>
  class MyIterator {
   public:
    using difference_type = std::ptrdiff_t;                       // NOLINT
    using pointer = Pointer;                                      // NOLINT
    using value_type = ValueType;                                 // NOLINT
    using reference = std::conditional_t<IsConst, const T&, T&>;  // NOLINT
    using iterator_category = std::random_access_iterator_tag;    // NOLINT
    MyIterator() = default;
    MyIterator(const MyIterator&) = default;
    MyIterator(MyIterator&&) = default;
    MyIterator& operator=(const MyIterator&) = default;

    MyIterator& operator++() {
      ++ptr_;
      return *this;
    }

    MyIterator operator++(int)const {
      auto tmp = *this;
      return ++tmp;
    }

    MyIterator& operator--() {
      --ptr_;
      return *this;
    }

    MyIterator operator--(int)const {
      auto tmp = *this;
      return --tmp;
    }

    MyIterator& operator+(size_t i) {
      ptr_ += i;
      return *this;
    }

    MyIterator operator+(size_t i) const {
      auto tmp = ptr_ + i;
      return MyIterator(tmp);
    }

    MyIterator& operator-(size_t i) {
      ptr_ -= i;
      return *this;
    }

    MyIterator operator-(size_t i) const {
      auto tmp = ptr_ - i;
      return MyIterator(tmp);
    }
    reference operator*() const {
      return *ptr_;
    }

    T* operator->() const {
      return ptr_;
    }

    bool operator==(const MyIterator& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const MyIterator& other) const {
      return !(*this == other);
    }

   private:
    friend Vector;

    explicit MyIterator(T* ptr) : ptr_(ptr) {
    }

   private:
    T* ptr_ = nullptr;
  };

  Iterator begin() {  // NOLINT
    return Iterator(arr_);
  }
  ConstIterator begin() const {  // NOLINT
    return cbegin();
  }
  ConstIterator cbegin() const {  // NOLINT
    return ConstIterator(arr_);
  }

  Iterator end() {  // NOLINT
    return Iterator(arr_ + size_);
  }

  ConstIterator end() const {  // NOLINT
    return cend();
  }
  ConstIterator cend() const {  // NOLINT
    return ConstIterator(arr_ + size_);
  }

  ReverseIterator rbegin() {  // NOLINT
    return static_cast<ReverseIterator>(Iterator(arr_ + size_));
  }

  ReverseIterator rend() {  // NOLINT
    return static_cast<ReverseIterator>(Iterator(arr_));
  }

  ConstReverseIterator crbegin() const {  // NOLINT
    return static_cast<ConstReverseIterator>(cend());
  }

  ConstReverseIterator crend() const {  // NOLINT
    return static_cast<ConstReverseIterator>(cbegin());
  }

  ConstReverseIterator rbegin() const {  // NOLINT
    return static_cast<ConstReverseIterator>(cend());
  }

  ConstReverseIterator rend() const {  // NOLINT
    return static_cast<ConstReverseIterator>(cbegin());
  }

 private:
  void FixReallocExtendtion(size_t new_cap) {
    auto tmp = reinterpret_cast<T*>(operator new(sizeof(T) * new_cap));
    size_t i = 0;
    try {
      for (; i < size_; ++i) {
        new (tmp + i) T(std::move(arr_[i]));
      }
    } catch (...) {
      for (size_t j = 0; j < i; ++j) {
        (tmp + j)->~T();
      }
      operator delete(tmp);
      throw;
    }
    operator delete(arr_);
    arr_ = tmp;
    capacity_ = new_cap;
  }

  void Reallocation() {
    FixReallocExtendtion(kCoefficient * (capacity_ + 1));
  }

 private:
  T* arr_;
  std::size_t size_;
  std::size_t capacity_;
  static const std::size_t kCoefficient = 2;
};

template <class T>
Vector<T> operator+(const Vector<T>& f, const Vector<T>& s) {
  Vector<T> temp = f;
  temp += s;
  return temp;
}

template <class T>
bool operator<(const Vector<T>& f, const Vector<T>& s) {
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

template <class T>
bool operator>(const Vector<T>& f, const Vector<T>& s) {
  return s < f;
}

template <class T>
bool operator<=(const Vector<T>& f, const Vector<T>& s) {
  return !(f > s);
}

template <class T>
bool operator>=(const Vector<T>& f, const Vector<T>& s) {
  return s <= f;
}

template <class T>
bool operator==(const Vector<T>& f, const Vector<T>& s) {
  return s <= f && s >= f;
}

template <class T>
bool operator!=(const Vector<T>& f, const Vector<T>& s) {
  return !(s == f);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& str) {
  size_t size = str.Size();
  for (size_t i = 0; i < size; ++i) {
    os << str[i];
  }
  return os;
}

#endif
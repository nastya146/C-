#ifndef SH_PTR_H
#define SH_PTR_H

#include <stdexcept>

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

template <class T>
class SharedPtr {
 public:
  SharedPtr() : ptr_(nullptr), counter_(nullptr) {
  }

  SharedPtr(std::nullptr_t) : SharedPtr() {  // NOLINT
  }

  explicit SharedPtr(T* ptr) : ptr_(ptr), counter_(new size_t(1)) {
  }

  SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (ptr_ != nullptr) {
      ++*counter_;
    }
  }

  SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), counter_(other.counter_) {
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
  }

  SharedPtr& operator=(const SharedPtr& other) {
    if (this != &other) {
      if (counter_ != nullptr) {
        if (*counter_ > 1) {
          --*counter_;
        } else {
          delete counter_;
          delete ptr_;
        }
      }
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      if (counter_ != nullptr) {
        ++*counter_;
      }
    }
    return *this;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
      if (counter_ != nullptr) {
        if (*counter_ > 1) {
          --*counter_;
        } else {
          delete counter_;
          delete ptr_;
        }
      }
      ptr_ = other.ptr_;
      counter_ = other.counter_;
      other.ptr_ = nullptr;
      other.counter_ = nullptr;
    }
    return *this;
  }

  ~SharedPtr() {
    if (counter_ != nullptr) {
      if (*counter_ > 1) {
        --*counter_;
        return;
      }
      delete counter_;
      delete ptr_;
    }
  }

  void Reset(T* ptr = nullptr) {
    T* temp = ptr;
    if (counter_ != nullptr && *counter_ > 1) {
      --*counter_;
    } else {
      delete counter_;
      delete ptr_;
    }
    ptr_ = ptr;
    counter_ = (ptr_ == nullptr ? nullptr : new size_t(1));
  }

  void Swap(SharedPtr<T>& other) {
    SharedPtr<T> temp = std::move(*this);
    *this = std::move(other);
    other = std::move(temp);
  }

  T* Get() const {
    return ptr_;
  }

  size_t UseCount() const {
    return counter_ == nullptr ? 0 : *counter_;
  }

  T& operator*() {
    return *ptr_;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

 private:
  T* ptr_;
  size_t* counter_;
};

#endif
#ifndef WEAK_PTR_IMPLEMENTED
#define WEAK_PTR_IMPLEMENTED

#include <stdexcept>

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

struct Counter {
  size_t strong_count_ = 0;
  size_t weak_count_ = 0;
};

template <class>
class WeakPtr;

template <class T>
class SharedPtr {
 public:
  SharedPtr() : ptr_(nullptr), counter_(nullptr) {
  }

  SharedPtr(std::nullptr_t) : SharedPtr() {  // NOLINT
  }

  explicit SharedPtr(T* ptr) : ptr_(ptr), counter_(new Counter{1, 0}) {
  }

  SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (ptr_ != nullptr) {
      ++counter_->strong_count_;
    }
  }

  SharedPtr(T* ptr, Counter* counter) : ptr_(ptr), counter_(counter) {
    if (counter_ != nullptr) {
      ++counter_->strong_count_;
    }
  }

  SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), counter_(other.counter_) {
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
  }

  SharedPtr& operator=(const SharedPtr& other) {
    SharedPtr temp(other);
    Swap(temp);
    return *this;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
      Reset();
      SharedPtr temp(other);
      Swap(temp);
      other.Reset();
    }
    return *this;
  }

  explicit SharedPtr(const WeakPtr<T>& other) {
    if (other.Expired()) {
      throw BadWeakPtr{};
    } else {
      SharedPtr temp = other.Lock();
      counter_ = temp.counter_;
      ptr_ = other.ptr_;
      ++counter_->strong_count_;
    }
  }

  ~SharedPtr() {
    Reset();
  }

  void Reset(T* ptr = nullptr) {
    if (counter_ != nullptr) {
      --counter_->strong_count_;
      if (counter_->strong_count_ == 0) {
        if (counter_->weak_count_ == 0) {
          delete counter_;
        }
        delete ptr_;
      }
    }
    ptr_ = ptr;
    counter_ = (ptr_ == nullptr ? nullptr : new Counter{1, 0});
  }

  void Swap(SharedPtr<T>& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
  }

  T* Get() const {
    return ptr_;
  }

  size_t UseCount() const {
    return counter_ == nullptr ? 0 : counter_->strong_count_;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

  template <class>
  friend class WeakPtr;

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

 private:
  T* ptr_;
  Counter* counter_;
};

template <class T>
class WeakPtr {
 public:
  template <class>
  friend class SharedPtr;

  WeakPtr() : ptr_(nullptr), counter_(nullptr) {
  }

  WeakPtr(const WeakPtr& other) : ptr_(other.ptr_), counter_(other.counter_) {
    if (counter_ != nullptr) {
      ++counter_->weak_count_;
    }
  }

  explicit WeakPtr(SharedPtr<T>&& other) noexcept : ptr_(other.ptr_), counter_(other.counter_) {
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
  }

  WeakPtr& operator=(const WeakPtr& other) {
    WeakPtr temp(other);
    Swap(temp);
    return *this;
  }

  WeakPtr& operator=(WeakPtr&& other) noexcept {
    if (this != &other) {
      Reset();
      WeakPtr temp(other);
      Swap(temp);
      other.Reset();
    }
    return *this;
  }

  WeakPtr(const SharedPtr<T>& other) noexcept : ptr_(other.Get()), counter_(other.counter_) {  // NOLINT
    if (counter_ != nullptr) {
      ++counter_->weak_count_;
    }
  }

  ~WeakPtr() {
    Reset();
  }

  void Reset() {
    if (counter_ != nullptr) {
      if (counter_->weak_count_ >= 1) {
        --counter_->weak_count_;
      }
      if (counter_->strong_count_ == 0 && counter_->weak_count_ == 0) {
        delete counter_;
      }
      counter_ = nullptr;
    }
    ptr_ = nullptr;
  }

  void Swap(WeakPtr<T>& other) {
    std::swap(ptr_, other.ptr_);
    std::swap(counter_, other.counter_);
  }

  size_t UseCount() const {
    return counter_ == nullptr ? 0 : counter_->strong_count_;
  }

  bool Expired() const {
    return counter_ == nullptr ? true : counter_->strong_count_ == 0;
  }

  SharedPtr<T> Lock() const {
    if (Expired()) {
      return SharedPtr<T>();
    }
    return SharedPtr<T>(ptr_, counter_);
  }

 private:
  T* ptr_;
  Counter* counter_;
};

#endif
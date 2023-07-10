#ifndef U_PTR_H
#define U_PTR_H

template <class T>
class UniquePtr {
 public:
  UniquePtr() : ptr_(nullptr) {
  }

  explicit UniquePtr(T* ptr) : ptr_(ptr) {
  }

  // copy constructor
  UniquePtr(const UniquePtr&) = delete;

  // copy assignment
  UniquePtr& operator=(const UniquePtr&) = delete;

  // move constructor
  UniquePtr(UniquePtr&& ptr) noexcept : ptr_(ptr.ptr_) {
    ptr.ptr_ = nullptr;
  }

  // move assignment
  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (ptr_ != other.ptr_) {
      delete ptr_;
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }

  // destructor
  ~UniquePtr() {
    delete ptr_;
  }

  T* Release() {
    T* temp = ptr_;
    ptr_ = nullptr;
    return temp;
  }

  void Reset(T* ptr = nullptr) {
    T* old_ptr = ptr_;
    ptr_ = (ptr_ == ptr ? nullptr : ptr);
    delete old_ptr;
  }

  void Swap(UniquePtr<T>& other) {
    UniquePtr<T> temp = std::move(*this);
    *this = std::move(other);
    other = std::move(temp);
  }

  T* Get() const {
    return ptr_;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

 private:
  T* ptr_;
};

#endif
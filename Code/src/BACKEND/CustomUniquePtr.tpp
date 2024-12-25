#include "CustomUniquePtr.h"

template <typename T>
CustomUniquePtr<T>::CustomUniquePtr(T* ptr) : mPtr(ptr) {}

template <typename T>
CustomUniquePtr<T>::~CustomUniquePtr() {
    delete mPtr;
}

template <typename T>
CustomUniquePtr<T>::CustomUniquePtr(CustomUniquePtr&& other) noexcept : mPtr(other.mPtr) {
    other.mPtr = nullptr;
}

template <typename T>
CustomUniquePtr<T>& CustomUniquePtr<T>::operator=(CustomUniquePtr&& other) noexcept {
    if (this != &other) {
        delete mPtr;
        mPtr = other.mPtr;
        other.mPtr = nullptr;
    }
    return *this;
}

template <typename T>
T* CustomUniquePtr<T>::get() const {
    return mPtr;
}

template <typename T>
T* CustomUniquePtr<T>::release() {
    T* temp = mPtr;
    mPtr = nullptr;
    return temp;
}

template <typename T>
void CustomUniquePtr<T>::reset(T* ptr) {
    if (mPtr != ptr) {
        delete mPtr;
        mPtr = ptr;
    }
}

template <typename T>
T& CustomUniquePtr<T>::operator*() const {
    return *mPtr;
}

template <typename T>
T* CustomUniquePtr<T>::operator->() const {
    return mPtr;
}
#include "CustomVector.h"
#include <stdexcept>
#include <iostream>

template <typename T>
CustomVector<T>::CustomVector() : mSize(0), mCapacity(1) {
    mData = new T[mCapacity];  // Allocate memory for T, not CustomUniquePtr<T>
}

template <typename T>
CustomVector<T>::~CustomVector() {
    delete[] mData;
}

template <typename T>
void CustomVector<T>::push_back(T element) {  // Accept T directly
    if (mSize == mCapacity) {
        resize(mCapacity * 2);
    }
    mData[mSize++] = std::move(element);
}

template <typename T>
void CustomVector<T>::erase(int index) {
    if (index < 0 || index >= mSize) {
        throw std::out_of_range("Index out of range");
    }
    for (int i = index; i < mSize - 1; ++i) {
        mData[i] = std::move(mData[i + 1]);
    }
    --mSize;
}

template <typename T>
T& CustomVector<T>::operator[](int index) const {  // Return T& instead of T*
    if (index < 0 || index >= mSize) {
        std::cerr << "Index out of range: " << index << " (size: " << mSize << ")" << std::endl;
        throw std::out_of_range("Index out of range");
    }
    return mData[index];
}

template <typename T>
int CustomVector<T>::size() const {
    return mSize;
}

template <typename T>
bool CustomVector<T>::empty() const {  // Implement empty method
    return mSize == 0;
}

template <typename T>
void CustomVector<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];  // Allocate as T, not CustomUniquePtr<T>
    for (int i = 0; i < mSize; ++i) {
        newData[i] = std::move(mData[i]);
    }
    delete[] mData;
    mData = newData;
    mCapacity = newCapacity;
}

template <typename T>
T* CustomVector<T>::begin() {  // Implement begin method
    return mData;
}

template <typename T>
T* CustomVector<T>::end() {  // Implement end method
    return mData + mSize;
}

template <typename T>
void CustomVector<T>::clear() {
    for (int i = 0; i < mSize; ++i) {
        mData[i].~T();  // Explicitly call the destructor for each element
    }
    mSize = 0;
}
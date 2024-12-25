#include "CustomVector.h"
#include <iostream>

template <typename T>
CustomVector<T>::CustomVector() : data(nullptr), vectorSize(0), capacity(0) {}

template <typename T>
CustomVector<T>::CustomVector(const CustomVector& other)
    : data(new T[other.capacity]), vectorSize(other.vectorSize), capacity(other.capacity) {
    for (std::size_t i = 0; i < vectorSize; ++i) {
        data[i] = other.data[i];
    }
}

template <typename T>
CustomVector<T>& CustomVector<T>::operator=(const CustomVector& other) {
    if (this != &other) {
        delete[] data;
        data = new T[other.capacity];
        vectorSize = other.vectorSize;
        capacity = other.capacity;
        for (std::size_t i = 0; i < vectorSize; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
CustomVector<T>::~CustomVector() {
    delete[] data;
}

template <typename T>
void CustomVector<T>::push_back(const T& value) {
    if (vectorSize == capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[vectorSize++] = value;
}

template <typename T>
void CustomVector<T>::pop_back() {
    if (vectorSize > 0) {
        --vectorSize;
    } else {
        throw std::out_of_range("Vector is empty");
    }
}

template <typename T>
void CustomVector<T>::erase(std::size_t index) {
    if (index >= vectorSize) {
        throw std::out_of_range("Index out of bounds");
    }
    // Shift elements to the left to fill the gap
    for (std::size_t i = index; i < vectorSize - 1; ++i) {
        data[i] = data[i + 1];
    }
    --vectorSize; // Decrease size after removing element
}

template <typename T>
T& CustomVector<T>::operator[](std::size_t index) {
    if (index >= vectorSize) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

template <typename T>
const T& CustomVector<T>::operator[](std::size_t index) const {
    if (index >= vectorSize) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

template <typename T>
std::size_t CustomVector<T>::size() const {
    return vectorSize;
}

template <typename T>
std::size_t CustomVector<T>::getCapacity() const {
    return capacity;
}

template <typename T>
bool CustomVector<T>::isEmpty() const {
    return vectorSize == 0;
}

template <typename T>
void CustomVector<T>::clear() {
    vectorSize = 0;
}

template <typename T>
void CustomVector<T>::resize(std::size_t newCapacity) {
    T* newData = new T[newCapacity];
    for (std::size_t i = 0; i < vectorSize; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
T* CustomVector<T>::begin() {
    return data;
}

template <typename T>
T* CustomVector<T>::end() {
    return data + vectorSize;
}

template <typename T>
const T* CustomVector<T>::begin() const {
    return data;
}

template <typename T>
const T* CustomVector<T>::end() const {
    return data + vectorSize;
}
#pragma once
#include "CustomVector.h"
#include <stdexcept>
#include <iostream>

#include <cstddef>
#include <stdexcept>

template <typename T> class CustomVector {
private:
    T* data;
    std::size_t vectorSize;
    std::size_t capacity;


public:
    CustomVector();
    CustomVector(const CustomVector& other);
    CustomVector& operator=(const CustomVector& other);
    ~CustomVector();

    void push_back(const T& value);
    void pop_back();
    void erase(std::size_t index);
    void resize(std::size_t newCapacity);

    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    std::size_t size() const;
    std::size_t getCapacity() const;

    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;
    bool isEmpty() const;
    void clear();
};

#include "../../src/BACKEND/CustomVector.tpp"

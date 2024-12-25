#ifndef CUSTOMVECTOR_H
#define CUSTOMVECTOR_H

#include "CustomUniquePtr.h"

template <typename T>
class CustomVector {
public:
    CustomVector();
    ~CustomVector();

    void push_back(T element);  // Accept T directly
    void erase(int index);
    T& operator[](int index) const;  // Return T& instead of T*
    int size() const;
    bool empty() const;
    T* begin();
    T* end();
    void clear();
private:
    T* mData;  // Store T directly, not CustomUniquePtr<T>
    int mSize;
    int mCapacity;

    void resize(int newCapacity);
};

#include "../../src/BACKEND/CustomVector.tpp"

#endif // CUSTOMVECTOR_H

#ifndef CUSTOMUNIQUEPTR_H
#define CUSTOMUNIQUEPTR_H

template <typename T>
class CustomUniquePtr {
public:
    CustomUniquePtr(T* ptr = nullptr);
    ~CustomUniquePtr();

    CustomUniquePtr(const CustomUniquePtr&) = delete;
    CustomUniquePtr& operator=(const CustomUniquePtr&) = delete;

    CustomUniquePtr(CustomUniquePtr&& other) noexcept;
    CustomUniquePtr& operator=(CustomUniquePtr&& other) noexcept;

    T* get() const;
    T* release();
    void reset(T* ptr = nullptr);

    T& operator*() const;
    T* operator->() const;

private:
    T* mPtr;
};

#include "../../src/BACKEND/CustomUniquePtr.tpp"

#endif // CUSTOMUNIQUEPTR_H
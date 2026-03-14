#pragma once
template <typename T>
class Singleton {
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static T* getInstance(QWidget * parent = nullptr) {
        if(!instance) instance = new T(parent);
        return instance;
    }
protected:
    static inline T * instance = nullptr;
};
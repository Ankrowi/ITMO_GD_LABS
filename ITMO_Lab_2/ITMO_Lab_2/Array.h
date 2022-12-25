#ifndef LAB2ARRAY_H
#define LAB2ARRAY_H

#include <cassert>

template<typename T>
class Array final {
public:
    Array() : size_(0) {
        capacity_ = defaultCapacity;
        data_ = (T*)malloc(sizeof(T) * capacity_);
    }
    Array(int capacity) : size_(0) {
        capacity_ = capacity > 0 ? capacity : defaultCapacity;
        data_ = (T*)malloc(sizeof(T) * capacity_);
    }
    Array(const Array& other) {
        size_ = other.size();
        capacity_ = other.capacity_;
        data_ = (T*)malloc(sizeof(T) * capacity_);
        for (size_t i = 0; i < size_; i++) {
            new (data_ + i) T(other.data_[i]);
        }
    }
    Array(Array&& other) :
        data_(nullptr), size_(0), capacity_(0) {
        swap(other);
    }
    Array& operator=(const Array& other) {
        Array<T> temp(other);
        swap(temp);
        return *this;
    }
    Array& operator=(Array&& other) {
        if (this == &other) return;
        clear();
        swap(other);
        return *this;
    }
    ~Array() {
        for (size_t i = 0; i < size_; i++)
            data_[i].~T();
        free(data_);
    }
    int insert(const T& value) {
        if (size_ == capacity_) {
            this->realoc();
        }
        new (data_ + size_) T(value);
        return size_++;
    }
    int insert(int index, const T& value) {
        assert(index < size_&& index >= 0);
        if (size_ == capacity_) {
            this->realoc();
        }
        new (data_ + size_) T(std::move(data_[size_ - 1]));
        for (size_t i = size_ - 1; i > index; --i) {
            data_[i] = std::move(data_[i - 1]);
        }
        data_[index] = value;
        return size_++;
    }
    void remove(int index) {
        assert(index < size_ && index >= 0 && size_ > 0);
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        data_[--size_].~T();
    }
    const T& operator[](int index) const {
        assert(index < size_&& index >= 0);
        return *(data_ + index);
    }
    T& operator[](int index) {
        assert(index < size_&& index >= 0);
        return *(data_ + index);
    }
    int size() const {
        return size_;
    }
    int capacity() const {
        return capacity_;
    }
    T* begin() {
        return data_;
    }
    T* end() {
        return data_ + size_;
    }
    class AbstractReversibleIterator {
    public:
        ~AbstractReversibleIterator() {}
        bool hasNext() const {
            if (position_ < 0 ||
                position_ >= array_.size()) return false;
            return true;
        }
        void next() {
            assert(hasNext());
            position_ += increment_;
        }
        const T& get() const {
            return array_[position_];
        }
    protected:
        AbstractReversibleIterator(Array<T>& arrayPtr, bool isReversed = false) {
            increment_ = isReversed ? ReverseInc : DefaultInc;
            array_ = arrayPtr;
            position_ = isReversed ? array_.size() - 1 : 0;
        }
        Array<T> array_;
        int position_;
        int increment_;
    private:
        const int DefaultInc = 1;
        const int ReverseInc = -1;
    };
    class Iterator : public AbstractReversibleIterator {
    public:
        using AbstractReversibleIterator::array_;
        using AbstractReversibleIterator::position_;
        Iterator(Array<T>& arrayPtr, bool isReversed = false)
            : AbstractReversibleIterator(arrayPtr, isReversed) { }
        void set(const T& value) {
            array_[position_] = value;
        }
    };
    class ConstIterator : public AbstractReversibleIterator {
    public:
        ConstIterator(Array<T>& arrayPtr, bool isReversed = false)
            : AbstractReversibleIterator(arrayPtr, isReversed) { }
    };
    Iterator iterator() {
        return Iterator(*this, false);
    }
    Iterator reverseIterator() {
        return Iterator(*this, true);
    }
    ConstIterator iterator() const {
        return ConstIterator(*this, false);
    }
    ConstIterator reverseIterator() const {
        return ConstIterator(*this, true);
    }
private:
    void swap(Array& other) {
        std::swap(this->data_, other.data_);
        std::swap(this->size_, other.size_);
        std::swap(this->capacity_, other.capacity_);
    }
    void realoc() {
        if (capacity_ == 0)
            capacity_ = defaultCapacity;
        else
            capacity_ *= scaleFactor;
        T* tempData = (T*)malloc(capacity_ * sizeof(T));
        //сперва через размещающий new и копирование
        for (size_t i = 0; i < size_; i++) {
            new (tempData + i) T(data_[i]);
            data_[i].~T();
        }
        free(data_);
        data_ = tempData;
    }
    void clear() {
        for (size_t i = 0; i < size_; ++i)
            data_[i].~T();
        size_ = 0;
        capacity_ = 0;
        free(data_);
    }
    size_t size_;
    size_t capacity_;
    T* data_;    
    const int defaultCapacity = 8;
    const float scaleFactor = 2.f;
};


#endif
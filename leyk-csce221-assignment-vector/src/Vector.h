#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { 
        // if array is null, set size to 1;


        if(array == nullptr) {
            _capacity = 1;
            _size = 0;
            array = new T[_capacity];
        } else { // double size; e.g size is same, capacity doubles, copy elements into new array
            _capacity *= 2;
            T* new_arr = new T[_capacity];
            for(size_t i = 0; i < _size; i++) { // copies old to new using move
                new_arr[i] = std::move(array[i]);
            }
            delete[] array; // delete old array
            array = new_arr; // set array to new array

        }
    }

public:
    Vector() noexcept { 
        array = nullptr;
        _capacity = 0;
        _size = 0;
     }
    Vector(size_t count, const T& value) { 
        _capacity = count;
        _size = count;
        array = new T[count];

        for (size_t i = 0; i < count; i++) {
            array[i] = value;
        }
     }
    explicit Vector(size_t count) { 
        _capacity = count;
        _size = count;
        array = new T[count](); 
     }

    // copy constructor - deep copy
    Vector(const Vector& other) { 
        _capacity = other._capacity;
        _size = other._size;
        array = new T[_capacity];
        for(size_t i = 0; i < _size; i++) {
            array[i] = other.array[i];
        }
     }
    // move constructor - shallow copy + destroy other
    Vector(Vector&& other) noexcept { 
        _capacity = other._capacity;
        _size = other._size;
        array = other.array;
        other.array = nullptr;
        other._capacity = 0;
        other._size = 0;
     }

    ~Vector() { 
        delete[] array;
     }

    // copy assignment - same as constructor, but original vector already exists, same with move 
    Vector& operator=(const Vector& other) { 
        if(this != &other) { // accounts for self copy
            delete[] array;
            _capacity = other._capacity;
            _size = other._size;
            array = new T[_capacity];
            for(size_t i = 0; i < _size; i++) {
                array[i] = other.array[i];
            }
        }
        return *this;
     }
    // move assignment  
    Vector& operator=(Vector&& other) noexcept { 
        if(this != &other) {
            delete[] array;
            _capacity = other._capacity;
            _size = other._size;
            array = other.array;
            other.array = nullptr;
            other._capacity = 0;
            other._size = 0;
        }
        return *this;
     }

    iterator begin() noexcept { 
        // returns an iterator to the first element
        return iterator(array); //since array is the address of the first element
    }
    iterator end() noexcept { 
        return iterator(array+_size);
     }

    [[nodiscard]] bool empty() const noexcept { return _size == 0; }
    size_t size() const noexcept { return _size; }
    size_t capacity() const noexcept { return _capacity; }

    T& at(size_t pos) { 
        if(pos >= _size) {
            throw std::out_of_range("Out of range");
        } else {
            return array[pos];
        }
    }
    const T& at(size_t pos) const { 
        if(pos >= _size) {
            throw std::out_of_range("Out of range");
        } else {
            return array[pos];
        }
     }
     
     //one liners
    T& operator[](size_t pos) { return array[pos]; }
    const T& operator[](size_t pos) const { return array[pos]; }
    T& front() { return array[0]; }
    const T& front() const { return array[0]; }
    T& back() { return array[_size - 1]; }
    const T& back() const { return array[_size - 1]; }

    void push_back(const T& value) { 
        insert(end(), value);
     }
    void push_back(T&& value) { 
        insert(end(), std::move(value));
     }
    void pop_back() { 
        erase(end()-1);
     }

    // inserts one element at pos
    iterator insert(iterator pos, const T& value) { 
        // if array is full, we need to grow
        // get index of pos
        size_t index = pos - iterator(array);
        
        if(_size == _capacity) {
            grow();
        }
        // pos iterator is now pointing to the new array at the index
        pos = iterator(array + index);
     
        // start from last element and shift then go backwards
        // size_t index = pos - array;
        
        for(size_t i = _size; i > index; i--) {
            array[i] = std::move(array[i-1]);
        }
        // insert value at pos
        *pos = value;
        _size++;
        return pos;
     }

    // insert using move()
    iterator insert(iterator pos, T&& value) { 
        // if array is full, we need to grow
        // get index of pos
        size_t index = pos - iterator(array);
        
        if(_size == _capacity) {
            grow();
        }
        // pos iterator is now pointing to the new array at the index
        pos = iterator(array + index);
     
        // start from last element and shift then go backwards
        // size_t index = pos - array;
        
        for(size_t i = _size; i > index; i--) {
            array[i] = std::move(array[i-1]);
        }
        // insert value at pos
        *pos = std::move(value);
        _size++;
        return pos;
     }
    
    // insert multiple elemets starting at pos
    iterator insert(iterator pos, size_t count, const T& value) { 
        // keep growing array until there's enough space to add all the values
        size_t index = pos - iterator(array);

        while(_size + count > _capacity) {
            grow();
        }
        pos = iterator(array + index);
        // shift elements to the right by count
        for(size_t i = _size; i > index; i--) {
            array[i + count - 1] = std::move(array[i - 1]);
        }
        // insert values starting at pos
        for(size_t i = 0; i < count; i++) {
            array[index + i] = value;
        }
        _size += count;
        return pos;
     }
    iterator erase(iterator pos) {  
        // no need to grow, just shift elements backwards, starting from pos
        for(size_t i = pos - iterator(array); i < _size-1; i++) {
            array[i] = std::move(array[i+1]); 
        }
        _size--;
        return pos;
     } 
    iterator erase(iterator first, iterator last) { 
        // "Erases elements in the range [first, last), including first and excluding last"
        size_t count = last - first;
        for(size_t i = first - iterator(array); i < _size - count; i++) {
            array[i] = std::move(array[i + (count)]);
        }
        _size -= count;
        return first;
     }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Points to some element in the vector (or nullptr)
        T* _ptr;

    public:
        iterator() { _ptr = nullptr; }
        explicit iterator(T* ptr) { _ptr = ptr; }

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept { 
            // this is supposed to return what the iterator references, hence the return of "reference"
            return *_ptr;
         }
        [[nodiscard]] pointer operator->() const noexcept { 
            // return the address of the element the iterator points to
            return _ptr;
        }

        // Prefix Increment: ++a
        iterator& operator++() noexcept {
            _ptr++; // increment the pointer
            return *this; // return dereferenced value of iterator
        }
        // Postfix Increment: a++
        iterator operator++(int) noexcept { 
            iterator temp = *this;
            _ptr++;
            return temp;
        }
        // Prefix Decrement: --a
        iterator& operator--() noexcept { 
            _ptr--; // same as prefix ++a, but subtract
            return *this; 
         }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept { 
            iterator temp = *this;
            _ptr--;
            return temp;
         }

        iterator& operator+=(difference_type offset) noexcept { 
            _ptr += offset;
            return *this;
         }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            iterator temp = *this;
            temp += offset;
            return temp;
        }
        
        iterator& operator-=(difference_type offset) noexcept { 
            _ptr -= offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { 
            iterator temp = *this;
            temp -= offset;
            return temp;
        }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { 
            return _ptr - rhs._ptr;
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept {
            return *(_ptr + offset);
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { 
            return _ptr == rhs._ptr;
         }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { 
            return _ptr != rhs._ptr;
        }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept { 
            return _ptr < rhs._ptr;
         }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { 
            return _ptr > rhs._ptr;
         }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { 
            return _ptr <= rhs._ptr;
        }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { 
            return _ptr >= rhs._ptr;
         }
    };


    void clear() noexcept { 
        _size = 0;
     }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept { 
    // "returns a copy of an iterator advanced forward by an offset"
    return iterator + offset;
 }

#endif

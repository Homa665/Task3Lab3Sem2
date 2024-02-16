#ifndef VECTOR_H
#define VECTOR_H


#include <cstddef>
#include <iterator>
#include <limits>

template <typename T>
class Vector {
public:
    Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}
    explicit Vector(size_t size) : m_data(new T[size]), m_size(size), m_capacity(size) {}
    Vector(const Vector<T>& other) : m_data(new T[other.m_size]), m_size(other.m_size), m_capacity(other.m_size) {
        for (size_t i = 0; i < m_size; i++) {
            m_data[i] = other.m_data[i];
        }
    }
    ~Vector() { delete[] m_data; }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] m_data;
            m_data = new T[other.m_size];
            m_size = other.m_size;
            m_capacity = other.m_size;
            for (size_t i = 0; i < m_size; i++) {
                m_data[i] = other.m_data[i];
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        if (m_size == m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        m_data[m_size] = value;
        m_size++;
    }

    void pop_back() {
        if (m_size > 0) {
            --m_size;
            m_data[m_size].~T(); // вызываем явно деструктор последнего элемента
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > m_capacity) {
            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < m_size; i++) {
                new_data[i] = m_data[i];
            }
            delete[] m_data;
            m_data = new_data;
            m_capacity = new_capacity;
        }
    }

    void assign(const T* first, const T* last) {
        clear();
        const size_t count = last - first;
        reserve(count);
        for (size_t i = 0; i < count; i++) {
            push_back(first[i]);
        }
    }

    const T& at(size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }

    T& at(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }

    const T& back() const {
        if (m_size == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return m_data[m_size - 1];
    }

    T& back() {
        if (m_size == 0) {
            throw std::out_of_range("Vector is empty");
        }
        return m_data[m_size - 1];
    }




    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }
    void clear() { m_size = 0; }

    T& operator[](size_t index) { return m_data[index]; }
    const T& operator[](size_t index) const { return m_data[index]; }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        Iterator& operator++() {
            m_ptr++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator== (const Iterator& a, const Iterator& b) {
            return a.m_ptr == b.m_ptr;
        };

        friend bool operator!= (const Iterator& a, const Iterator& b) {
            return a.m_ptr != b.m_ptr;
        };
    private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(m_data); }
    Iterator end() { return Iterator(m_data + m_size); }
    // Определение вложенного типа const_iterator
        class const_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = const T;
            using pointer = const T*;
            using reference = const T&;

            const_iterator(pointer ptr) : m_ptr(ptr) {}

            reference operator*() const { return *m_ptr; }
            pointer operator->() const { return m_ptr; }

            const_iterator& operator++() {
                m_ptr++;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const const_iterator& a, const const_iterator& b) {
                return a.m_ptr == b.m_ptr;
            }

            friend bool operator!=(const const_iterator& a, const const_iterator& b) {
                return a.m_ptr != b.m_ptr;
            }

        private:
            pointer m_ptr;
        };
    const_iterator begin() const { return const_iterator(m_data); }


    typename Vector<T>::const_iterator cbegin() const {
        return const_iterator(m_data);
    }

    T* data() {
            return m_data;
    }

    template <typename... Args>
    typename Vector<T>::Iterator emplace(Vector<T>::const_iterator pos, Args&&... args) {
        const size_t index = pos - cbegin();
        if (m_size == m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        for (size_t i = m_size; i > index; --i) {
            m_data[i] = std::move(m_data[i-1]);
        }
        ++m_size;
        m_data[index] = T(std::forward<Args>(args)...);
        return begin() + index;
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (m_size == m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        m_data[m_size++] = T(std::forward<Args>(args)...);
    }




    // Метод erase (удаление элемента по итератору)
    typename Vector<T>::Iterator erase(Vector<T>::const_iterator pos) {
        Iterator it = begin() + (pos - cbegin());
        for (Iterator i = it; i != end() - 1; ++i) {
            *i = std::move(*(i+1));
        }
        --m_size;
        return it;
    }

    // Метод erase (удаление диапазона элементов)
    typename Vector<T>::Iterator erase(Vector<T>::const_iterator first, Vector<T>::const_iterator last) {
        Iterator it_first = begin() + (first - cbegin());
        Iterator it_last = begin() + (last - cbegin());
        for (Iterator i = it_first; i != it_last; ++i) {
            *i = std::move(*(i + (it_last - it_first)));
        }
        m_size -= last - first;
        return it_first;
    }

    // Метод front
    T& front() {
        return *begin();
    }

    typename Vector<T>::Iterator insert(Vector<T>::const_iterator pos, const T& value) {
        const size_t index = pos - cbegin();
        if (m_size == m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        for (size_t i = m_size; i > index; --i) {
            m_data[i] = std::move(m_data[i-1]);
        }
        ++m_size;
        m_data[index] = value;
        return begin() + index;
    }

    typename Vector<T>::Iterator insert(Vector<T>::const_iterator pos, T&& value) {
        const size_t index = pos - cbegin();
        if (m_size == m_capacity) {
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);
        }
        for (size_t i = m_size; i > index; --i) {
            m_data[i] = std::move(m_data[i-1]);
        }
        ++m_size;
        m_data[index] = std::move(value);
        return begin() + index;
    }

    typename Vector<T>::Iterator insert(Vector<T>::const_iterator pos, size_t count, const T& value) {
        const size_t index = pos - cbegin();
        if (m_size + count > m_capacity) {
            reserve(m_size + count);
        }
        for (size_t i = m_size + count - 1; i >= index + count; --i) {
            m_data[i] = std::move(m_data[i-count]);
        }
        m_size += count;
        for (size_t i = index; i < index + count; ++i) {
            m_data[i] = value;
        }
        return begin() + index;
    }

    template <typename InputIt>
    typename Vector<T>::Iterator insert(Vector<T>::const_iterator pos, InputIt first, InputIt last) {
        const size_t index = pos - cbegin();
        const size_t count = std::distance(first, last);
        if (m_size + count > m_capacity) {
            reserve(m_size + count);
        }
        for (size_t i = m_size + count - 1; i >= index + count; --i) {
            m_data[i] = std::move(m_data[i-count]);
        }
        m_size += count;
        for (size_t i = index; first != last; ++i, ++first) {
            m_data[i] = *first;
        }
        return begin() + index;
    }

    size_t max_size() const {
        return std::numeric_limits<size_t>::max();
    }

    using reverse_iterator = std::reverse_iterator<Iterator>;

    typename Vector<T>::reverse_iterator rbegin() {
        return std::make_reverse_iterator(end());
    }

    typename Vector<T>::reverse_iterator rend() {
        return std::make_reverse_iterator(begin());
    }

    void resize(size_t newSize, const T& value) {
        if (newSize > m_capacity) {
            reserve(newSize);
        }

        if (newSize > m_size) {
            for (size_t i = m_size; i < newSize; ++i) {
                m_data[i] = value;
            }
        }

        m_size = newSize;
    }

    void swap(Vector<T>& other) {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }



private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;
};

#endif // VECTOR_H

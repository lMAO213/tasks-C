#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>

template <typename T>
class Deque {
public:

    //------------------------------------------------- Typical class functions ----------------------------------------

    Deque() { // constructor
        _deque.resize(size_of_vector);
        first_block_of_num_and_val = std::make_pair(_deque.size() / 2, size_of_vector / 2);
        last_block_of_num_and_val = std::make_pair(_deque.size() / 2, size_of_vector / 2 - 1);
        for(size_t i = 0; i < _deque.size(); i++)
            _deque[i] = reinterpret_cast<T*>(new int64_t[size_of_vector * sizeof(T)]);
    }

    Deque(const Deque& copy) { // copy constructor
        first_block_of_num_and_val = copy.first_block_of_num_and_val;
        last_block_of_num_and_val = copy.last_block_of_num_and_val;
        size_of_deque = copy.size_of_deque;

        _deque.resize(copy._deque.size());
        for(size_t i = 0; i < _deque.size(); i++) {
            _deque[i] = reinterpret_cast<T*>(new int8_t[size_of_vector * sizeof(T)]);

            if(i >= first_block_of_num_and_val.first and i <= last_block_of_num_and_val.first) {
                for(size_t j = 0; j < size_of_vector; j++) {
                    if(i == first_block_of_num_and_val.first and j >= first_block_of_num_and_val.second)
                        new(_deque[i] + j) T(copy._deque[i][j]);

                    else if (i == last_block_of_num_and_val.first and j <= last_block_of_num_and_val.second)
                        new(_deque[i] + j) T(copy._deque[i][j]);

                    else
                        new(_deque[i] + j) T(copy._deque[i][j]);
                }
            }
        }
    }

    explicit Deque(int val) {
        size_t new_length = std::max(
                static_cast<size_t>(ceil(static_cast<double>(val) / static_cast<double>(size_of_vector))),
                static_cast<size_t>(1)
        );

        _deque.resize(new_length);

        first_block_of_num_and_val = std::make_pair(0, 0);
        last_block_of_num_and_val = std::make_pair(new_length - 1, (val - 1) % size_of_vector);

        for(size_t i = 0; i < new_length; i++) {
            _deque[i] = reinterpret_cast<T*>(new int8_t[size_of_vector * sizeof(T)]);

            for (size_t j = 0; j < size_of_vector; j++) {
                if(i == new_length - 1 and j > last_block_of_num_and_val.second)
                    break;

                new(_deque[i] + j) T();
            }
        }

        size_of_deque = val;


    }

    explicit Deque(int len,  const T& val) { // constructor from (int, const T&)

        size_t new_length = std::max(
                static_cast<size_t>(ceil(static_cast<double>(len) / static_cast<double>(size_of_vector))),
                static_cast<size_t>(1)
                );

        _deque.resize(new_length);

        first_block_of_num_and_val = std::make_pair(0, 0);
        last_block_of_num_and_val = std::make_pair(new_length - 1, (len - 1) % size_of_vector);

        for(size_t i = 0; i < new_length; i++) {
            _deque[i] = reinterpret_cast<T*>(new int8_t[size_of_vector * sizeof(T)]);

            for (size_t j = 0; j < size_of_vector; j++) {
                if(i == new_length - 1 and j > last_block_of_num_and_val.second)
                    break;

                new(_deque[i] + j) T(val);
            }
        }

        size_of_deque = len;
    }

    //-------------------------------------------------------- Operators -----------------------------------------------

    Deque& operator=(const Deque& assignment) { // assigning a new deque
        Deque copy = Deque(assignment);

        size_of_deque = copy.size_of_deque;
        _deque = copy._deque;
        first_block_of_num_and_val = copy.first_block_of_num_and_val;
        last_block_of_num_and_val = copy.last_block_of_num_and_val;

        return *this;
    }

    T& operator[](size_t pos) { // returning element on pos
        size_t pos_in_arr = first_block_of_num_and_val.first * size_of_vector +
                first_block_of_num_and_val.second + pos;

        return _deque[pos_in_arr / size_of_vector][pos_in_arr % size_of_vector];
    }

    const T& operator[](size_t pos) const { // returning const element on pos
        size_t pos_in_arr = first_block_of_num_and_val.first * size_of_vector +
                first_block_of_num_and_val.second + pos;

        return _deque[pos_in_arr / size_of_vector][pos_in_arr % size_of_vector];
    }

    T& at(size_t pos) { // returning element on pos (by at)
        if(pos >= size_of_deque)
            throw std::out_of_range("Out of range");

        size_t pos_in_arr = first_block_of_num_and_val.first * size_of_vector +
                first_block_of_num_and_val.second + pos;

        return _deque[pos_in_arr / size_of_vector][pos_in_arr % size_of_vector];
    }

    const T& at(size_t pos) const{ // returning const element on pos (by at)
        if(pos >= size_of_deque)
            throw std::out_of_range("Out of range");

        size_t pos_in_arr = first_block_of_num_and_val.first * size_of_vector +
                first_block_of_num_and_val.second + pos;

        return _deque[pos_in_arr / size_of_vector][pos_in_arr % size_of_vector];
    }

    //------------------------------------------------- Usual functions ------------------------------------------------

    size_t size() const { // returning size of deque
        return size_of_deque;
    }

    void push_back(const T& val) { // inserting elem to the end of deque
        if(last_block_of_num_and_val.first == _deque.size() - 1 and
        last_block_of_num_and_val.second == size_of_vector - 1) {

            size_t tmp = _deque.size();
            _deque.resize(2 * _deque.size());

            for(size_t i = tmp; i < _deque.size(); i++)
                _deque[i] = reinterpret_cast<T*>(new int8_t[size_of_vector * sizeof(T)]);
        }

        if(last_block_of_num_and_val.second == size_of_vector - 1) {
            last_block_of_num_and_val.first++;
            last_block_of_num_and_val.second = 0;
        }
        else last_block_of_num_and_val.second++;
        new(_deque[last_block_of_num_and_val.first] + last_block_of_num_and_val.second) T(val);
        size_of_deque++;
    }

    void push_front(const T& val) { // inserting elem to begin of deque
        if(first_block_of_num_and_val == std::make_pair(size_t(0), size_t(0))) {
            size_t prev_size = _deque.size();
            _deque.resize(2 * prev_size);

            for(size_t i = 0; i < prev_size ; i++) {
                _deque[i + prev_size] = _deque[i];
                _deque[i] = reinterpret_cast<T*>(new int8_t[size_of_vector * sizeof(T)]);
            }
            first_block_of_num_and_val.first = prev_size;
            last_block_of_num_and_val.first += prev_size;
        }
        if(first_block_of_num_and_val.second == 0) {
            first_block_of_num_and_val.first--;
            first_block_of_num_and_val.second = size_of_vector - 1;
        }
        else first_block_of_num_and_val.second--;
        new(_deque[first_block_of_num_and_val.first] + first_block_of_num_and_val.second) T(val);
        size_of_deque++;
    }

    void pop_back() { // removing elem from end of deque
        (_deque[last_block_of_num_and_val.first] + last_block_of_num_and_val.second)->~T();
        if(last_block_of_num_and_val.second == 0) {
            last_block_of_num_and_val.first--;
            last_block_of_num_and_val.second = size_of_vector - 1;
        }
        else last_block_of_num_and_val.second--;
        size_of_deque--;
    }

    void pop_front() { // removing elem from begin of deque
        (_deque[first_block_of_num_and_val.first] + first_block_of_num_and_val.second)->~T();
        if(first_block_of_num_and_val.second == size_of_vector - 1) {
            first_block_of_num_and_val.first++;
            first_block_of_num_and_val.second = 0;
        }
        else first_block_of_num_and_val.second++;
        size_of_deque--;
    }

    //------------------------------------------------- Iterator struct ------------------------------------------------

    template <bool Const>

    struct Iterator {
        typename std::vector<T*>::const_iterator const_iterator_for_vector;
        size_t elem_ind;

        Iterator(typename std::vector<T*>::const_iterator const_iterator_for_vector, size_t elem_ind):
            const_iterator_for_vector(const_iterator_for_vector), elem_ind(elem_ind) {}

        Iterator& operator++() {
            if(elem_ind == size_of_vector - 1) {
                ++const_iterator_for_vector;
                elem_ind = 0;
            }
            else ++elem_ind;
            return *this;
        }

        Iterator& operator--() {
            if(elem_ind == 0) {
                elem_ind = size_of_vector - 1;
                const_iterator_for_vector--;
            } else
                elem_ind--;

            return *this;
        }

        int operator-(const Iterator<Const>& second) const{
            return (const_iterator_for_vector - second.const_iterator_for_vector) * size_of_vector +
            static_cast<int>(elem_ind) - static_cast<int>(second.elem_ind);
        }

        Iterator<Const> operator+(const int& len) const{
            int add_block = (elem_ind + len) / size_of_vector;
            int new_pos = (elem_ind + len) % size_of_vector;
            return Iterator<Const>(const_iterator_for_vector + add_block, new_pos);
        }

        Iterator<Const> operator-(const int& len) const {
            int left_shift = (elem_ind - len) / size_of_vector;
            int new_pos = (elem_ind - len) % size_of_vector;
            if(new_pos < 0) {
                left_shift -= 1;
                new_pos += size_of_vector;
            }
            return Iterator<Const>(const_iterator_for_vector + left_shift, new_pos);
        }

        bool operator<(const Iterator<Const>& second) const {
            return (*this - second) < 0;
        }

        bool operator>(const Iterator<Const>& second) const {
            return (second - *this) < 0;
        }

        bool operator==(const Iterator<Const>& second) const {
            return (!(*this < second) and !(*this > second));
        }

        bool operator!=(const Iterator<Const>& second) const{
            return !(*this == second);
        }

        bool operator<=(const Iterator<Const>& second) const{
            return !(*this > second);
        }

        bool operator>=(const Iterator<Const>& second) const{
            return !(*this < second);
        }

        std::conditional_t<Const, const T&, T&> operator*() const {
            return *(*const_iterator_for_vector + elem_ind);
        }

        std::conditional_t<Const, const T*, T*> operator->() const{
            return (*const_iterator_for_vector + elem_ind);
        }

    };

    using iterator = Iterator<false>;
    using const_iterator = const Iterator<true>;

    using reversed_iterator = std::reverse_iterator<iterator>;
    using reversed_const_iterator = std::reverse_iterator<const_iterator>;

    //------------------------------------------------ Iterators navigate ----------------------------------------------

    iterator begin() {
        return iterator(_deque.begin() + first_block_of_num_and_val.first, first_block_of_num_and_val.second);
    }

    const_iterator begin() const{
        return const_iterator(_deque.begin() + first_block_of_num_and_val.first, first_block_of_num_and_val.second);
    }

    iterator end() {
        auto copy = iterator(_deque.begin() + last_block_of_num_and_val.first, last_block_of_num_and_val.second);
        ++copy;
        return copy;
    }

    const_iterator end() const{
        auto copy = const_iterator(_deque.begin() + last_block_of_num_and_val.first, last_block_of_num_and_val.second);
        return ++copy;
    }

    const_iterator cbegin() const {
        return const_iterator(_deque.begin() + first_block_of_num_and_val.first, first_block_of_num_and_val.second);
    }

    const_iterator cend() const {
        auto copy = const_iterator(_deque.begin() + last_block_of_num_and_val.first, last_block_of_num_and_val.second);
        return ++copy;
    }

    //---------------------------------------------- Functions with iterator -------------------------------------------

    void insert(iterator it, const T& val) {
        push_back(val);

        for(auto i = end() - 1; i > it; --i) {
            std::iter_swap(i, i - 1);
        }
    }

    void erase(iterator it) {
        for(auto i = it; i < end() - 1; ++i) {
            std::iter_swap(i, i + 1);
        }

        pop_back();
    }

    //--------------------------------------------- Reversed iterators navigate ----------------------------------------

    reversed_iterator rbegin() {
        return reversed_iterator(end());
    }

    reversed_iterator rend() {
        return reversed_iterator(begin());
    }

    reversed_const_iterator rbegin() const {
        return reversed_iterator(cend());
    }

    reversed_const_iterator rend() const {
        return reversed_iterator(cbegin());
    }

    reversed_const_iterator crbegin() const {
        return reversed_iterator(cend());
    }

    reversed_const_iterator crend() const {
        return reversed_iterator(cbegin());
    }

    //-------------------------------------------------- End of public -------------------------------------------------

private:
    std::vector<T*> _deque;
    static const size_t size_of_vector = 1;
    std::pair<size_t, size_t> first_block_of_num_and_val = std::make_pair(0, 0);
    std::pair<size_t, size_t> last_block_of_num_and_val = std::make_pair(0, 0);
    size_t size_of_deque = 0;
};


#endif //DEQUE_H


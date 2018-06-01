#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cstdint>
#include <iterator>
#include <algorithm>
#include <memory>
#include <limits>
#include <cassert>
#include <vector>

struct my_vector {
    typedef uint64_t ull;

    my_vector() noexcept;
    explicit my_vector(size_t size);
    my_vector(const my_vector &other);
    ~my_vector();

    my_vector &operator=(const my_vector &other);

    void push_back(const ull &element);
    void pop_back();

    size_t size() const;
    bool empty() const;
    void resize(size_t new_size);
    void clear();

    const ull &back() const;
    bool operator==(const my_vector &other) const;
    ull &operator[](size_t ind);
    const ull &operator[](size_t ind) const;

private:
    struct big_data {
        size_t _capacity;
        std::shared_ptr<std::vector<ull>> _data;

        ~big_data();
    };

    union {
        ull small;
        big_data big;
    };

    bool is_small;
    size_t _size;

    void make_copy_for_sptr();
    void ensure_capacity(size_t new_size);
};

#endif
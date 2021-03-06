#include "my_vector.h"

typedef uint64_t ull;

my_vector::my_vector() noexcept : small(0), is_small(true), _size(0) {}

my_vector::my_vector(size_t size) : my_vector() {

    if (size <= 1) {
        small = 0;
        _size = size;
        is_small = true;
    } else {
        _size = big._capacity = size;
        is_small = false;

        new(&big._data) std::shared_ptr<ull>(new ull[size], array_deleter());
        std::fill(big._data.get(), big._data.get() + size, 0);
    }
}

my_vector::my_vector(const my_vector &other) {
    if (other.is_small) {
        small = other.small;
        is_small = true;
    } else {
        big._capacity = other.big._capacity;
        is_small = false;

        new(&big._data) std::shared_ptr<ull>(other.big._data);
    }

    _size = other._size;
}

my_vector::my_vector(const std::vector<ull> &other) {
    if (other.size() == 1) {
        small = other[0];
        is_small = true;
    } else if (other.size() > 1) {
        big._capacity = other.size();
        is_small = false;

        new(&big._data) std::shared_ptr<ull>(new ull[other.size()], array_deleter());
        std::copy(other.begin(), other.end(), big._data.get());
    }

    _size = other.size();
}

my_vector::big_data::~big_data() = default;

my_vector::~my_vector() {
    if (!is_small) {
        big.~big_data();
    }
}

my_vector &my_vector::operator=(const my_vector &other) {
    if (*this == other) {
        return *this;
    }

    clear();
    if (other.is_small) {
        if (!is_small) {
            big.~big_data();
        }
        small = other.small;
        is_small = true;
    } else {
        if (is_small) {
            new(&big._data) std::shared_ptr<ull>(other.big._data);
        } else {
            std::copy(other.big._data.get(), other.big._data.get() + other._size, big._data.get());
        }
        big._capacity = other.big._capacity;
        is_small = false;
    }

    _size = other._size;
    return *this;
}

std::size_t my_vector::size() const {
    return _size;
}

bool my_vector::empty() const {
    return _size == 0;
}

void my_vector::resize(size_t new_size) {
    if (new_size <= _size) {
        return;
    }
    while (_size < new_size) {
        push_back(0);
    }
}

void my_vector::ensure_capacity(size_t new_size) {
    if (is_small) {
        if (new_size > 1) {
            std::shared_ptr<ull> ptr(new ull[2]{small}, array_deleter());
            new(&big._data) std::shared_ptr<ull>(ptr);
            big._capacity = 2;
            is_small = false;
        }
    } else {
        if (new_size <= 1) {
            small = big._data.get()[0];
            is_small = true;
            big.~big_data();
        } else if (new_size > big._capacity || 2 * new_size < big._capacity) {
            new_storage(new_size * 2);
        }
    }
}

void my_vector::make_copy_for_sptr() {
    if (is_small) {
        return;
    }
    if (big._data.unique()) {
        return;
    }

    new_storage(_size);
}

void my_vector::push_back(const my_vector::ull &element) {
    make_copy_for_sptr();

    if (_size == 0) {
        small = element;
        is_small = true;
    } else {
        ensure_capacity(_size + 1);
        assert(big._capacity > _size);

        big._data.get()[_size] = element;
    }
    ++_size;
}

void my_vector::pop_back() {
    assert(_size > 0);

    if (_size == 1) {
        small = 0;
    } else {
        assert(!is_small);
        ensure_capacity(_size - 1);
    }
    --_size;
}

void my_vector::clear() {
    if (!is_small) {
        big.~big_data();
    }

    _size = 0;
    small = 0;
    is_small = true;
}

const my_vector::ull &my_vector::back() const {
    if (is_small) {
        return small;
    } else {
        return big._data.get()[_size - 1];
    }
}

ull &my_vector::operator[](size_t ind) {
    if (is_small) {
        assert(ind == 0);
        return small;
    } else {
        assert(ind < _size);
        make_copy_for_sptr();
        return big._data.get()[ind];
    }
}

const ull &my_vector::operator[](size_t ind) const {
    if (is_small) {
        assert(ind == 0);
        return small;
    } else {
        assert(ind < _size);
        return big._data.get()[ind];
    }
}

bool my_vector::operator==(const my_vector &other) const {
    if (size() != other.size()) {
        return false;
    }

    for (std::size_t i = 0; i < size(); ++i) {
        if (operator[](i) != other[i]) {
            return false;
        }
    }
    return true;
}

void my_vector::new_storage(size_t new_size) {
    assert(!is_small);
    assert(_size <= new_size);

    std::shared_ptr<ull> tmp(new ull[new_size], array_deleter());
    std::copy(big._data.get(), big._data.get() + _size, tmp.get());
    big._data = tmp;
    big._capacity = new_size;
}

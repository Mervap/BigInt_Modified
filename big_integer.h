#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <cstdlib>
#include "my_vector.h"

typedef uint32_t ui;
typedef uint64_t ull;

struct big_integer {
    big_integer();
    big_integer(big_integer const &other);
    big_integer(int a);
    big_integer(ull a);
    big_integer(bool new_sign, my_vector const &new_data);
    explicit big_integer(std::string const &str);

    big_integer& operator=(big_integer const &other);

    big_integer abs() const;
    big_integer& operator+=(big_integer const &a);
    big_integer& operator-=(big_integer const &a);
    big_integer& operator*=(big_integer const &a);
    big_integer& operator/=(big_integer const &a);
    big_integer& operator%=(big_integer const &a);

    big_integer& operator&=(big_integer const &a);
    big_integer& operator|=(big_integer const &a);
    big_integer& operator^=(big_integer const &a);

    big_integer& operator<<=(ui a);
    big_integer& operator>>=(ui a);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const &a, big_integer const &b);
    friend bool operator!=(big_integer const &a, big_integer const &b);
    friend bool operator<(big_integer const &a, big_integer const &b);
    friend bool operator>(big_integer const &a, big_integer const &b);
    friend bool operator<=(big_integer const &a, big_integer const &b);
    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend big_integer operator+(big_integer const &a, big_integer const &b);
    friend big_integer operator-(big_integer const &a, big_integer const &b);
    friend big_integer operator*(big_integer const &a, big_integer const &b);
    friend big_integer operator/(big_integer const &a, big_integer const &b);
    friend big_integer operator%(big_integer const &a, big_integer const &b);

    friend big_integer operator&(big_integer const &a, big_integer const &b);
    friend big_integer operator|(big_integer const &a, big_integer const &b);
    friend big_integer operator^(big_integer const &a, big_integer const &b);
    friend big_integer operator<<(big_integer const &a, ull b);
    friend big_integer operator>>(big_integer const &a, ull b);

    friend std::string to_string(big_integer const &a);
    void swap(big_integer &other) noexcept;
    bool is_zero() const;

private:
    bool sign;
    my_vector data;

    size_t length() const;
    ull get_digit(size_t ind) const;
    void make_fit();
    void correct();
};


#endif
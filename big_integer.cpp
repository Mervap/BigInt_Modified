#include "big_integer.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

__extension__ typedef unsigned __int128 ui128;

const ull MAX_DIGIT = UINT64_MAX;
const ull BASE = 64;
const ull BASE_10 = 1000000000000000000;

template<typename T>
ull ull_cast(T x) {
    return static_cast<ull>(x);
}

template<typename T>
ui128 __int128_cast(T x) {
    return static_cast<ui128>(x);
}

bool big_integer::is_zero() const {
    return (!sign) && (length() == 0);
}

size_t big_integer::length() const {
    return data.size();
}

big_integer big_integer::abs() const {
    return sign ? -(*this) : *this;
}

void big_integer::swap(big_integer &other) noexcept {
    std::swap(data, other.data);
    std::swap(sign, other.sign);
}


ull big_integer::get_digit(size_t ind) const {
    if (ind < length()) {
        return data[ind];
    } else if (sign) {
        return MAX_DIGIT;
    } else {
        return 0;
    }
}


void big_integer::make_fit() {
    while (!data.empty() && ((sign && data.back() == MAX_DIGIT) || (!sign && data.back() == 0))) {
        data.pop_back();
    }
}

big_integer::big_integer(bool new_sign, my_vector const &new_data) : sign(new_sign), data(new_data) {
    make_fit();
}

big_integer::big_integer() : sign(false) {}

big_integer::big_integer(big_integer const &other) : sign(other.sign), data(other.data) {
    make_fit();
}

big_integer::big_integer(int a) : sign(a < 0), data(1) {
    data[0] = ull_cast(a);
    make_fit();
}

big_integer::big_integer(ull a) : sign(0), data(1) {
    data[0] = ull_cast(a);
    make_fit();
}

ull string_to_int(std::string const &s) {
    ull ans = 0;
    for (auto a : s) {
        if (a < '0' || a > '9') {
            throw std::runtime_error("Incorrect char");
        }
        ans = ans * 10 + (a - '0');
    }
    return ans;
}

ull pow_dec(ui st) {
    ull res = 1;
    for (ui i = 0; i < st; ++i) {
        res *= 10;
    }
    return res;
}

big_integer string_to_number(std::string const &s) {
    big_integer res(0);
    size_t beg = 0;
    if (s[beg] == '-') {
        ++beg;
    }
    for (size_t i = beg; i + 18 <= s.length(); i += 18) {
        res = res * BASE_10 + string_to_int(s.substr(i, 18));
    }
    ui mod = (s.length() - beg) % 18;
    if (mod > 0) {
        res = res * pow_dec(mod) + string_to_int(s.substr(s.length() - mod, mod));
    }
    return beg > 0 ? -res : res;
}

big_integer::big_integer(std::string const &str) : big_integer(string_to_number(str)) {}

big_integer &big_integer::operator=(big_integer const &other) {
    big_integer tmp(other);
    swap(tmp);
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &a) {
    *this = *this + a;
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &a) {
    *this = *this - a;
    return *this;
}

big_integer &big_integer::operator*=(big_integer const &a) {
    *this = *this * a;
    return *this;
}

big_integer &big_integer::operator/=(big_integer const &a) {
    *this = *this / a;
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &a) {
    *this = *this % a;
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &a) {
    *this = *this & a;
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &a) {
    *this = *this | a;
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &a) {
    *this = *this ^ a;
    return *this;
}

big_integer &big_integer::operator<<=(ui a) {
    *this = *this << a;
    return *this;
}

big_integer &big_integer::operator>>=(ui a) {
    *this = *this >> a;
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    return ~(*this) + static_cast<ull>(1ULL);
}

big_integer big_integer::operator~() const {
    my_vector tmp(data);
    for (size_t i = 0; i < data.size(); ++i) {
        tmp[i] = ~data[i];
    }
    return big_integer(!sign, tmp);
}

big_integer &big_integer::operator++() {
    *this = *this + 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer tmp(*this);
    ++(*this);
    return tmp;
}

big_integer &big_integer::operator--() {
    *this = *this - 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer tmp(*this);
    --(*this);
    return tmp;
}

big_integer operator+(big_integer const &a, big_integer const &b) {
    size_t n = std::max(a.length(), b.length()) + 1;
    my_vector tmp(n);
    ui128 c = 0;
    ui128 sum = 0;

    for (size_t i = 0; i < n; ++i) {
        sum = c + a.get_digit(i) + b.get_digit(i);
        tmp[i] = ull_cast(sum);
        c = sum >> BASE;
    }

    return big_integer(tmp.back() & (1ll << (BASE - 1)), tmp);
}

big_integer operator-(big_integer const &a, big_integer const &b) {
    return a + (-b);

}

void mul_vector(my_vector &res, my_vector const &a, my_vector const &b) {
    for (size_t i = 0; i < a.size(); ++i) {
        ui128 c = 0;
        ui128 mul = 0;
        ui128 tmp = 0;

        for (size_t j = 0; j < b.size(); ++j) {
            size_t k = i + j;
            mul = (ui128) a[i] * b[j];
            tmp = (mul & MAX_DIGIT) + res[k] + c;
            res[k] = ull_cast(tmp);
            c = (mul >> BASE) + (tmp >> BASE);
        }
        res[i + b.size()] += ull_cast(c);
    }
}

void big_integer::correct() {
    if (!sign) {
        return;
    } else if (length() == 0) {
        sign = !sign;
        return;
    }

    size_t n = length();
    ui128 sum = ui128(~data[0]) + 1ULL;
    ui128 carry = sum >> BASE;
    data[0] = ull_cast(sum);
    for (size_t i = 1; i < n; ++i) {
        sum = carry + __int128_cast(~data[i]);
        data[i] = __int128_cast(sum);
        carry = sum >> BASE;
    }
    data.push_back(__int128_cast(carry + MAX_DIGIT));
    make_fit();
}

big_integer operator*(big_integer const &a, big_integer const &b) {

    if (a.is_zero() || b.is_zero()) {
        return 0;
    }

    big_integer abs_a(a.abs());
    big_integer abs_b(b.abs());

    size_t len = (abs_a.length() + abs_b.length() + 1);
    my_vector tmp(len);
    mul_vector(tmp, abs_a.data, abs_b.data);

    big_integer res(a.sign ^ b.sign, tmp);
    res.correct();
    return res;
}

bool compare_equal_vectors(my_vector const &a, my_vector const &b) {
    for (size_t i = a.size(); i > 0; --i) {
        if (a[i - 1] != b[i - 1]) {
            return a[i - 1] < b[i - 1];
        }
    }
    return false;
}


void mul_big_small(my_vector &res, my_vector const &a, const ull b) {
    size_t n = a.size();
    res.resize(n + 1);
    ui128 carry = 0, mul = 0, tmp = 0;
    for (size_t i = 0; i < n; ++i) {
        mul = (ui128) a[i] * b;
        tmp = (mul & MAX_DIGIT) + carry;
        res[i]  = ull_cast(tmp);
        carry = (mul >> BASE) + (tmp >> BASE);
    }
    res[n] = ull_cast(carry);
}

void sub_equal_vectors(my_vector &a, my_vector const &b) {
    ui128 sum = __int128_cast(~b[0]) + __int128_cast(a[0]) + 1;
    ui128 carry = sum >> BASE;
    a[0] = ull_cast(sum);
    for (size_t i = 1; i < a.size(); ++i) {
        sum = __int128_cast(~b[i]) + __int128_cast(a[i]) + carry;
        a[i] = ull_cast(sum);
        carry = sum >> BASE;
    }
}

ull get_trial(const ull a, const ull b, const ull c) {
    ui128 res = a;
    res = ((res << BASE) + b) / c;
    if (res > MAX_DIGIT) {
        res = MAX_DIGIT;
    }
    return ull_cast(res);
}

void constant_div(my_vector &res, my_vector &a, my_vector &b) {
    const ull f = ull_cast(((ui128) MAX_DIGIT + 1) / ((ui128) b.back() + 1));
    mul_big_small(a, a, f);
    mul_big_small(b, b, f);

    ull divisor = b.back();
    while (divisor == 0) {
        b.pop_back();
        divisor = b.back();
    }

    size_t n = a.size();
    size_t m = b.size();

    size_t len = n - m + 1;
    res.resize(len);
    my_vector dev(m + 1);
    my_vector div(m + 1);
    for (size_t i = 0; i < m; ++i) {
        dev[i] = a[n + i - m];
    }

    for (size_t i = 0; i < len; ++i) {
        dev[0] = a[n - m - i];
        ull qt = get_trial(dev[m], dev[m - 1], divisor);
        mul_big_small(div, b, qt);
        while ((qt >= 0) && compare_equal_vectors(dev, div)) {
            mul_big_small(div, b, --qt);
        }
        sub_equal_vectors(dev, div);
        for (size_t j = m; j > 0; --j) {
            dev[j] = dev[j - 1];
        }
        res[len - 1 - i]= qt;
    }
}

void shr(my_vector &a, ull b) {
    if (b == 0) {
        return;
    }

    size_t div = b / BASE;
    size_t mod = b % BASE;
    size_t new_size = 0;
    if (div < a.size()) {
        new_size = a.size() - div;
    }

    my_vector tmp(a.size());
    for (size_t i = 0; i < new_size; ++i) {
        ui128 x = (ui128) a[i + div] >> mod;
        ui128 y = 0;
        if (i + div + 1 < a.size()) {
            y = (ui128) a[i + div + 1] << (BASE - mod);
        }
        tmp[i] = ull_cast(x + y);
    }
    a = tmp;
}

big_integer operator/(big_integer const &a, big_integer const &b) {

    if (b.is_zero()) {
        throw std::runtime_error("Divison by zero");
    }
    big_integer abs_a(a.abs());
    big_integer abs_b(b.abs());
    if (abs_a < abs_b) {
        return 0;
    }

    const ull f = ull_cast(((ui128) MAX_DIGIT + 1) / ((ui128) abs_b.data.back() + 1));
    mul_big_small(abs_a.data, abs_a.data, f);
    mul_big_small(abs_b.data, abs_b.data, f);
    abs_a.make_fit();
    abs_b.make_fit();

    const size_t n = abs_a.length();
    const size_t m = abs_b.length();
    const size_t len = n - m + 1;

    my_vector tmp(len);
    my_vector dev(m + 2);
    my_vector div(m + 2);


    for (size_t i = 0; i < m; ++i) {
        dev[i] = abs_a.get_digit(n + i - m);
    }

    my_vector divisor;
    my_vector divis;
    if (m == 1) {
        divisor.push_back(abs_b.get_digit(m - 1));
    } else {
        divisor.push_back(abs_b.get_digit(m - 2));
        divisor.push_back(abs_b.get_digit(m - 1));
    }

    size_t N = 4 * BASE;
    my_vector acc({0, 0, 1, 0, 1});
    constant_div(divis, acc, divisor);

    abs_b.data.push_back(0);
    my_vector tmp2(9);
    for (size_t i = 0; i < len; ++i) {
        dev[0] = abs_a.get_digit(n - m - i);

        ull qt;
        if (m == 1) {
            my_vector acc1({dev[m - 1], dev[m], 0});
            mul_vector(tmp2, acc1, divis);
        } else {
            my_vector acc1({dev[m - 2], dev[m - 1], dev[m]});
            mul_vector(tmp2, acc1, divis);
        }
        shr(tmp2, N);
        qt = tmp2[0];

        mul_big_small(div, abs_b.data, qt);
        /*if (compare_equal_vectors(dev, div)) {
            --qt;                                   //??????????????????????????????
            mul_big_small(div, abs_b.data, qt);
        }*/

        sub_equal_vectors(dev, div);
        for (size_t j = m; j > 0; --j) {
            dev[j] = dev[j - 1];
        }
        tmp[len - 1 - i] = qt;
    }

    big_integer res(a.sign ^ b.sign, tmp);
    res.correct();
    return res;
}

big_integer operator%(big_integer const &a, big_integer const &b) {
    return a - (a / b) * b;
}

big_integer operator&(big_integer const &a, big_integer const &b) {
    size_t n = std::max(a.length(), b.length());
    my_vector tmp(n);

    for (size_t i = 0; i < n; ++i) {
        tmp[i] = a.get_digit(i) & b.get_digit(i);
    }
    return big_integer(a.sign & b.sign, tmp);
}

big_integer operator|(big_integer const &a, big_integer const &b) {
    size_t n = std::max(a.length(), b.length());
    my_vector tmp(n);

    for (size_t i = 0; i < n; ++i) {
        tmp[i] = a.get_digit(i) | b.get_digit(i);
    }
    return big_integer(a.sign | b.sign, tmp);
}

big_integer operator^(big_integer const &a, big_integer const &b) {
    size_t n = std::max(a.length(), b.length());
    my_vector tmp(n);

    for (size_t i = 0; i < n; ++i) {
        tmp[i] = a.get_digit(i) ^ b.get_digit(i);
    }
    return big_integer(a.sign ^ b.sign, tmp);
}

big_integer operator<<(big_integer const &a, ull b) {
    if (b == 0) {
        return big_integer(a);
    }
    size_t div = b / BASE;
    size_t mod = b % BASE;
    size_t new_size = a.length() + div + 1;
    my_vector tmp(new_size);
    tmp[div] = ull_cast((ui128) a.get_digit(0) << mod);

    for (size_t i = div + 1; i < new_size; ++i) {
        ui128 x = (ui128) a.get_digit(i - div) << mod;
        ui128 y = (ui128) a.get_digit(i - div - 1) >> (BASE - mod);
        tmp[i] = ull_cast(x + y);
    }
    return big_integer(a.sign, tmp);
}

big_integer operator>>(big_integer const &a, ull b) {
    if (b == 0) {
        return big_integer(a);
    }
    size_t div = b / BASE;
    size_t mod = b % BASE;
    size_t new_size = 0;
    if (div < a.length()) {
        new_size = a.length() - div;
    }
    my_vector tmp(new_size);
    for (size_t i = 0; i < new_size; ++i) {
        ui128 x = (ui128) a.get_digit(i + div) >> mod;
        ui128 y = (ui128) a.get_digit(i + div + 1) << (BASE - mod);
        tmp[i] = ull_cast(x + y);
    }
    return big_integer(a.sign, tmp);
}

bool operator==(big_integer const &a, big_integer const &b) {
    return (a.sign == b.sign) && (a.data == b.data);
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign;
    }
    if (a.length() != b.length()) {
        return a.length() < b.length();
    }

    for (size_t i = a.length(); i > 0; --i) {
        if (a.get_digit(i - 1) != b.get_digit(i - 1)) {
            return a.get_digit(i - 1) < b.get_digit(i - 1);
        }
    }
    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return b < a;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

std::string to_string(big_integer const &a) {
    if (a.is_zero()) {
        return "0";
    } else if (a.length() == 0) {
        return "-1";
    }

    std::string ans = "";
    big_integer abs_a(a.abs());

    while (!abs_a.is_zero()) {
        ull tmp = (abs_a % BASE_10).get_digit(0);
        for (size_t i = 0; i < 18; i++) {
            ans.push_back('0' + tmp % 10);
            tmp /= 10;
        }
        abs_a /= BASE_10;
    }

    while (!ans.empty() && ans.back() == '0') {
        ans.pop_back();
    }

    if (a.sign) {
        ans.push_back('-');
    }

    reverse(ans.begin(), ans.end());
    return ans;
}

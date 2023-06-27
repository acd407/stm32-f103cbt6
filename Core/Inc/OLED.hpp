#pragma once

#include <OLED.h>
#include <cmath>
#include <string>
using std::string;
#include <algorithm>
using std::reverse;

string trim (string s, char c);
string triml (string s, char c);

constexpr uint32_t count_digits (uint32_t Number, uint32_t n);

template <uint32_t base = 10>
string OLED_FMT (uint32_t Number) {
    auto i = count_digits (Number, base);
    string s (i, '0');
    while (Number) {
        s[--i] = (Number % base + '0');
        Number /= base;
    }
    return s;
}
template <uint32_t base = 10>
string OLED_FMT (int32_t Number) {
    return Number >= 0 ? OLED_FMT<base> (Number)
                       : OLED_FMT<base> (-Number).insert (0, '-');
}

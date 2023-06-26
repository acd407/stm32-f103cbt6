#pragma once

#include <cmath>
#include <cstdint>
#include <string>
using std::string;
#include <stm32f1xx_hal.h>
#include <algorithm>
using std::reverse;

#define OLED_Pin_SCL GPIO_PIN_12
#define OLED_Pin_SDA GPIO_PIN_13

void OLED_Init (void);
void OLED_Clear (void);
void OLED_ShowChar (uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString (uint8_t Line, uint8_t Column, const char *String);
void OLED_ShowStrings (uint8_t Line, uint8_t Column, const char *String);

string OLED_FMT_UDEC_A (uint32_t Number);
string OLED_FMT_DEC_A (int32_t Number);
string OLED_FMT_UDEC (uint32_t Number);
string OLED_FMT_DEC (int32_t Number);
string OLED_FMT_HEX (uint32_t Number);
string OLED_FMT_OCT (uint32_t Number);
string OLED_FMT_HEX (uint32_t Number);
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

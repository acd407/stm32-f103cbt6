#include <OLED.hpp>
#include <gpio.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <get_type_name.hpp>
#include <main.h>
#include <string>
#include <tuple>
#include <type_traits>

void mainxx() {
    OLED_Init();

    OLED_ShowStrings (0, 0, get_type_name<std::tuple<char, uint32_t>>().str);
    /* Infinite loop */
    while (1) {
        HAL_GPIO_TogglePin (GPIOA, GPIO_PIN_6);
        HAL_Delay (100);
    }
}
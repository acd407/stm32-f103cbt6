#pragma once

#include <sys/_stdint.h>
extern "C" {
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
}
#include <type_traits>

// Settings
namespace lcd1602 {
    // Port Select -----------------------------------------------------------
    const auto DAT_PORT = GPIOA;
    const auto CTL_PORT = GPIOB;

    // Control Pins ----------------------------------------------------------
    const auto PIN_RS = GPIO_PIN_13;
    const auto PIN_RW = GPIO_PIN_14;
    const auto PIN_EN = GPIO_PIN_15;

    // Data Pins -------------------------------------------------------------
    const auto PIN_D4 = GPIO_PIN_10;
    const auto PIN_D5 = GPIO_PIN_11;
    const auto PIN_D6 = GPIO_PIN_12;
    const auto PIN_D7 = GPIO_PIN_15;

} // namespace lcd1602

namespace lcd1602 {
    enum class Bits { FOUR = 0x00, EIGHT = 0x10 };
    enum class Rows { ONE = 0x00, TWO = 0x08 };
    enum class Quality { LOW = 0x00, HIGH = 0x04 };
    struct Working_Function {
        Bits data_bus;
        Rows row_count;
        Quality font_quality;
    };

    enum class Display { OFF = 0x00, ON = 0x04 };
    enum class Cursor { OFF = 0x00, ON = 0x02 };
    enum class Cursor_Blink { OFF = 0x00, ON = 0x01 };
    struct Display_Control {
        Display display_on_off;
        Cursor cursor_on_off;
        Cursor_Blink cursor_blink_on_off;
    };

    enum class Direction { DEC = 0x00, INC = 0x02 };
    enum class Shift { OFF = 0x00, ON = 0x01 };
    struct Entry_Mode {
        Direction cursor_shift_direction;
        Shift shift_on_off;
    };

    struct Init_Struct {
        Working_Function working_function;
        Display_Control display_control;
        Entry_Mode entry_mode;
    };

    HAL_StatusTypeDef Init (Init_Struct);
    HAL_StatusTypeDef SetCursor (uint8_t, uint8_t);
    HAL_StatusTypeDef WriteString (const char *);

    enum class State { CMD, BUSY, WRITE, READ };
    HAL_StatusTypeDef Transmit (uint8_t, State);

    enum class RW { WRITE = GPIO_PIN_RESET, READ = GPIO_PIN_SET };
    enum class RS { INSTRUCTION = GPIO_PIN_RESET, DATA = GPIO_PIN_SET };
    HAL_StatusTypeDef PinWrite (uint8_t, RS, RW);

    // Display Command
    enum class CMD {
        CLEAR = 0b00000001,         // 清屏
        CURSOR_RETURN = 0b00000010, // 光标回 (0, 0)
        CURSOR_LEFT = 0b00010000,   // 光标左移
        CURSOR_RIGHT = 0b00010100,  // 光标右移
        DISPLAY_LEFT = 0b00011000,  // 显示左移，光标跟随
        DISPLAY_RIGHT = 0b00011100, // 显示右移，光标跟随
    };
    template <typename T>
        requires std::is_same_v<T, lcd1602::CMD> || std::is_integral_v<T>
    HAL_StatusTypeDef WriteCMD (T arg) {
        Transmit (static_cast<uint8_t> (arg), State::CMD);
        return HAL_OK;
    }
} // namespace lcd1602

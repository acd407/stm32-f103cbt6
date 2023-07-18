#include "lcd1602.hpp"
#include <sys/_stdint.h>

HAL_StatusTypeDef lcd1602::Init (Init_Struct init_struct) {
    // Configure LCD Function Set to 4-Bit, 2 line and high quality font size
    uint8_t Function_Set =
        0x20 | static_cast<int> (init_struct.working_function.data_bus) |
        static_cast<int> (init_struct.working_function.row_count) |
        static_cast<int> (init_struct.working_function.font_quality);
    WriteCMD (Function_Set);
    HAL_Delay (1);

    // Configure LCD Display On / Off to Display: On, Cursor: On, Cursor Blink:
    // On
    uint8_t Display_Control =
        0x08 | static_cast<int> (init_struct.display_control.display_on_off) |
        static_cast<int> (init_struct.display_control.cursor_on_off) |
        static_cast<int> (init_struct.display_control.cursor_blink_on_off);
    WriteCMD (Display_Control);
    HAL_Delay (1);

    // Clear LCD
    WriteCMD (CMD::CLEAR);
    HAL_Delay (2);

    // Configure Entry Mode to I/D: Incrament mode and SH: Entire shif on
    uint8_t Entry_Mode =
        0x04 |
        static_cast<int> (init_struct.entry_mode.cursor_shift_direction) |
        static_cast<int> (init_struct.entry_mode.shift_on_off);
    WriteCMD (Entry_Mode);
    HAL_Delay (1);

    return HAL_OK;
}

HAL_StatusTypeDef lcd1602::WriteString (const char *Message) {
    while (*Message) {
        Transmit (*Message++, State::WRITE);
    }
    return HAL_OK;
}

HAL_StatusTypeDef lcd1602::SetCursor (uint8_t row, uint8_t col) {
    WriteCMD ((0x80 + 0x40 * row) | col);
    return HAL_OK;
}

HAL_StatusTypeDef lcd1602::Transmit (uint8_t data, State state) {
    switch (state) {
    case State::CMD:
        PinWrite (((data >> 4) & 0x0F), RS::INSTRUCTION, RW::WRITE);
        PinWrite ((data & 0x0F), RS::INSTRUCTION, RW::WRITE);
        break;
    case State::BUSY:
        PinWrite (((data >> 4) & 0x0F), RS::INSTRUCTION, RW::READ);
        PinWrite ((data & 0x0F), RS::INSTRUCTION, RW::READ);
        break;
    case State::WRITE:
        PinWrite (((data >> 4) & 0x0F), RS::DATA, RW::WRITE);
        PinWrite ((data & 0x0F), RS::DATA, RW::WRITE);
        break;
    case State::READ:
        PinWrite (((data >> 4) & 0x0F), RS::DATA, RW::READ);
        PinWrite ((data & 0x0F), RS::DATA, RW::READ);
        break;
    }
    HAL_Delay (2);
    return HAL_OK;
}

HAL_StatusTypeDef lcd1602::PinWrite (uint8_t data, RS rs, RW rw) {
    HAL_GPIO_WritePin (CTL_PORT, PIN_RS, static_cast<GPIO_PinState> (rs));
    HAL_GPIO_WritePin (CTL_PORT, PIN_RW, static_cast<GPIO_PinState> (rw));

    HAL_GPIO_WritePin (DAT_PORT, PIN_D7,
                       static_cast<GPIO_PinState> ((data >> 3) & 1));
    HAL_GPIO_WritePin (DAT_PORT, PIN_D6,
                       static_cast<GPIO_PinState> ((data >> 2) & 1));
    HAL_GPIO_WritePin (DAT_PORT, PIN_D5,
                       static_cast<GPIO_PinState> ((data >> 1) & 1));
    HAL_GPIO_WritePin (DAT_PORT, PIN_D4,
                       static_cast<GPIO_PinState> ((data >> 0) & 1));

    HAL_GPIO_WritePin (CTL_PORT, PIN_EN, GPIO_PIN_SET);
    HAL_GPIO_WritePin (CTL_PORT, PIN_EN, GPIO_PIN_RESET);
    return HAL_OK;
}

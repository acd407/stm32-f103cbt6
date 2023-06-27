#include "OLED.hpp"
#include "OLED_Font.h"
#include <cstdint>
#include <cstring>

/*引脚配置*/
#define OLED_W_SCL(x)                                                          \
    HAL_GPIO_WritePin (GPIOB, OLED_Pin_SCL, (GPIO_PinState) (x))
#define OLED_W_SDA(x)                                                          \
    HAL_GPIO_WritePin (GPIOB, OLED_Pin_SDA, (GPIO_PinState) (x))

/*引脚初始化*/
void OLED_I2C_Init (void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure {OLED_Pin_SCL, GPIO_MODE_OUTPUT_PP,
                                         GPIO_NOPULL, GPIO_SPEED_HIGH};
    HAL_GPIO_Init (GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = OLED_Pin_SDA;
    HAL_GPIO_Init (GPIOB, &GPIO_InitStructure);

    OLED_W_SCL (1);
    OLED_W_SDA (1);
}

/**
 * @brief  I2C开始
 * @param  无
 * @retval 无
 */
void OLED_I2C_Start (void) {
    OLED_W_SDA (1);
    OLED_W_SCL (1);
    OLED_W_SDA (0);
    OLED_W_SCL (0);
}

/**
 * @brief  I2C停止
 * @param  无
 * @retval 无
 */
void OLED_I2C_Stop (void) {
    OLED_W_SDA (0);
    OLED_W_SCL (1);
    OLED_W_SDA (1);
}

/**
 * @brief  I2C发送一个字节
 * @param  Byte 要发送的一个字节
 * @retval 无
 */
void OLED_I2C_SendByte (uint8_t Byte) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        OLED_W_SDA (Byte & (0x80 >> i));
        OLED_W_SCL (1);
        OLED_W_SCL (0);
    }
    OLED_W_SCL (1); // 额外的一个时钟，不处理应答信号
    OLED_W_SCL (0);
}

/**
 * @brief  OLED写命令
 * @param  Command 要写入的命令
 * @retval 无
 */
void OLED_WriteCommand (uint8_t Command) {
    OLED_I2C_Start();
    OLED_I2C_SendByte (0x78); // 从机地址
    OLED_I2C_SendByte (0x00); // 写命令
    OLED_I2C_SendByte (Command);
    OLED_I2C_Stop();
}

/**
 * @brief  OLED写数据
 * @param  Data 要写入的数据
 * @retval 无
 */
void OLED_WriteData (uint8_t Data) {
    OLED_I2C_Start();
    OLED_I2C_SendByte (0x78); // 从机地址
    OLED_I2C_SendByte (0x40); // 写数据
    OLED_I2C_SendByte (Data);
    OLED_I2C_Stop();
}

/**
 * @brief  OLED设置光标位置
 * @param  Y 以左上角为原点，向下方向的坐标，范围：0 ~ 7
 * @param  X 以左上角为原点，向右方向的坐标，范围：0 ~ 127
 * @retval 无
 */
void OLED_SetCursor (uint8_t Y, uint8_t X) {
    OLED_WriteCommand (0xB0 | Y);                 // 设置Y位置
    OLED_WriteCommand (0x10 | ((X & 0xF0) >> 4)); // 设置X位置低4位
    OLED_WriteCommand (0x00 | (X & 0x0F));        // 设置X位置高4位
}

/**
 * @brief  OLED清屏
 * @param  无
 * @retval 无
 */
void OLED_Clear (void) {
    uint8_t i, j;
    for (j = 0; j < 8; j++) {
        OLED_SetCursor (j, 0);
        for (i = 0; i < 128; i++) {
            OLED_WriteData (0x00);
        }
    }
}

/**
 * @brief  OLED显示一个字符
 * @param  Line 行位置，范围：0 ~ 3
 * @param  Column 列位置，范围：0 ~ 15
 * @param  Char 要显示的一个字符，范围：ASCII可见字符
 * @retval 无
 */
void OLED_ShowChar (uint8_t Line, uint8_t Column, char Char) {
    uint8_t i;
    OLED_SetCursor (Line * 2, Column * 8); // 设置光标位置在上半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteData (OLED_F8x16[Char - ' '][i]); // 显示上半部分内容
    }
    OLED_SetCursor (Line * 2 + 1,
                    Column * 8); // 设置光标位置在下半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteData (OLED_F8x16[Char - ' '][i + 8]); // 显示下半部分内容
    }
}

/**
 * @brief  OLED显示字符串
 * @param  Line 起始行位置，范围：0~3
 * @param  Column 起始列位置，范围：0 ~ 15
 * @param  String 要显示的字符串，范围：ASCII可见字符
 * @retval 无
 */
void OLED_ShowString (uint8_t Line, uint8_t Column, const char *str) {
    uint8_t i;
    for (i = 0; str[i] != '\0'; i++) {
        if (Column + i == 16)
            break;
        OLED_ShowChar (Line, Column + i, str[i]);
    }
}

/**
 * @brief  OLED显示字符串
 * @param  Line 起始行位置，范围：0~3
 * @param  Column 起始列位置，范围：0 ~ 15
 * @param  String 要显示的字符串，范围：ASCII可见字符
 * @retval 无
 */
void OLED_ShowStrings (uint8_t Line, uint8_t Column, const char *str) {
    auto len = strlen (str);
    OLED_ShowString (Line, Column, str);
    if (Column + len > 15) {
        OLED_ShowStrings ((Line + 1) % 4, 0, str + 16);
    }
}

/**
 * @brief  删除字符串头部匹配的字符
 * @param  s 要操作的字符串
 * @param  c 待删的字符
 * @retval 删除后的字符串，注意，不是引用
 */
string triml (string s, char c) {
    if (s.empty()) {
        return s;
    }
    s.erase (0, s.find_first_not_of (c));
    return s;
}

/**
 * @brief  删除字符串首尾匹配的字符
 * @param  s 要操作的字符串
 * @param  c 待删的字符
 * @retval 删除后的字符串，注意，不是引用
 */
string trim (string s, char c) {
    if (s.empty()) {
        return s;
    }
    s.erase (0, s.find_first_not_of (c));
    s.erase (s.find_last_not_of (c) + 1);
    return s;
}

/**
 * @brief  删除字符串头部匹配的字符
 * @param  s 要操作的字符串
 * @param  c 待删的字符
 * @retval 删除后的字符串，注意，原字符串很可能还有用，比如 free
 */
const char *triml (const char *s, char c) {
    if (s != NULL && strlen (s) > 0) {
        while (*s == c) {
            s++;
        }
    }
    return s;
}

/**
 * @brief  删除字符串首尾匹配的字符
 * @param  s 要操作的字符串
 * @param  c 待删的字符
 * @retval 删除后的字符串，注意，原字符串很可能还有用，比如 free
 */
char *trim (char *s, char c) {
    if (s != NULL && strlen (s) > 0) {
        while (*s == c) {
            s++;
        }
    }
    return s;
}

/**
 * @brief  计算 Number 在 n 进制作为字符串存储需要多少位
 * @param  Number 数
 * @param  n 进制
 * @retval 位数
 */
constexpr uint32_t count_digits (uint32_t Number, uint32_t n) {
    return Number == 0
               ? 1
               : static_cast<uint32_t> (std::log (Number) / std::log (n) + 1);
}

/**
 * @brief  OLED数字转字符串（十进制，无符号数）
 * @param  Number 要显示的数字，范围：0 ~ 4294967295
 * @retval 格式化后的字符串，前面填充 0
 */
const char *OLED_FMT_UDEC (uint32_t Number) {
    auto i = 10;
    string s (i, '0');
    while (Number) {
        s[--i] = (Number % 10 + '0');
        Number /= 10;
    }
    return s.c_str();
}

/**
 * @brief  OLED数字转字符串（十进制，有符号数）
 * @param  Number 要显示的数字，范围：-2147483648 ~ 2147483647
 * @retval 格式化后的字符串，和 OLED_FMT_DEC_A 行为一样
 */
const char *OLED_FMT_DEC (int32_t Number) {
    if (Number >= 0) {
        return trim (OLED_FMT_UDEC (Number), '0').c_str();
    } else {
        return trim (OLED_FMT_UDEC (Number), '0').insert (0, "-").c_str();
    }
}

/**
 * @brief  OLED数字转字符串（十六进制，无符号数）
 * @param  Number 要显示的数字，范围：0 ~ FFFF FFFF
 * @retval 格式化后的字符串，前面填充 0
 */
const char *OLED_FMT_HEX (uint32_t Number) {
    auto i = 8;
    string s (i, '0');
    while (Number) {
        s[--i] = (Number % 16 + '0');
        Number >>= 4;
    }
    return s.c_str();
}

/**
 * @brief  OLED数字转字符串（八进制，无符号数）
 * @param  Number 要显示的数字，范围：0 ~ 0377 7777 7777
 * @retval 格式化后的字符串，前面填充 0
 */
const char *OLED_FMT_OCT (uint32_t Number) {
    auto i = 11;
    string s (i, '0');
    while (Number) {
        s[--i] = (Number % 8 + '0');
        Number >>= 3;
    }
    return s.c_str();
}

/**
 * @brief  OLED数字转字符串（二进制，无符号数）
 * @param  Number 要显示的数字，范围：0 ~ 1<<32-1
 * @retval 格式化后的字符串，前面填充 0
 */
const char *OLED_FMT_BIN (uint32_t Number) {
    auto i = 32;
    string s (i, '0');
    while (Number) {
        s[--i] = (Number % 2 + '0');
        Number >>= 1;
    }
    return s.c_str();
}

/**
 * @brief  OLED初始化
 * @param  无
 * @retval 无
 */
void OLED_Init (void) {
    uint32_t i, j;

    for (i = 0; i < 1000; i++) // 上电延时
    {
        for (j = 0; j < 1000; j++)
            ;
    }

    OLED_I2C_Init(); // 端口初始化

    OLED_WriteCommand (0xAE); // 关闭显示

    OLED_WriteCommand (0xD5); // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand (0x80);

    OLED_WriteCommand (0xA8); // 设置多路复用率
    OLED_WriteCommand (0x3F);

    OLED_WriteCommand (0xD3); // 设置显示偏移
    OLED_WriteCommand (0x00);

    OLED_WriteCommand (0x40); // 设置显示开始行

    OLED_WriteCommand (0xA1); // 设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand (0xC8); // 设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand (0xDA); // 设置COM引脚硬件配置
    OLED_WriteCommand (0x12);

    OLED_WriteCommand (0x81); // 设置对比度控制
    OLED_WriteCommand (0xCF);

    OLED_WriteCommand (0xD9); // 设置预充电周期
    OLED_WriteCommand (0xF1);

    OLED_WriteCommand (0xDB); // 设置VCOMH取消选择级别
    OLED_WriteCommand (0x30);

    OLED_WriteCommand (0xA4); // 设置整个显示打开/关闭

    OLED_WriteCommand (0xA6); // 设置正常/倒转显示

    OLED_WriteCommand (0x8D); // 设置充电泵
    OLED_WriteCommand (0x14);

    OLED_WriteCommand (0xAF); // 开启显示

    OLED_Clear(); // OLED清屏
}

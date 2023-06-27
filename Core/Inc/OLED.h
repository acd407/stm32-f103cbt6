#ifndef __OLED_H
#define __OLED_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f1xx_hal.h>

#define OLED_Pin_SCL GPIO_PIN_12
#define OLED_Pin_SDA GPIO_PIN_13

void OLED_Init (void);
void OLED_Clear (void);
void OLED_ShowChar (uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString (uint8_t Line, uint8_t Column, const char *String);
void OLED_ShowStrings (uint8_t Line, uint8_t Column, const char *String);
char *trim (char *s, char c);
const char *triml (const char *s, char c);

const char *OLED_FMT_UDEC (uint32_t Number);
const char *OLED_FMT_DEC (int32_t Number);
const char *OLED_FMT_HEX (uint32_t Number);
const char *OLED_FMT_OCT (uint32_t Number);
const char *OLED_FMT_HEX (uint32_t Number);

#ifdef __cplusplus
}
#endif
#endif
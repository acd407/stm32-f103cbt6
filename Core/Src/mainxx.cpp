#include <cstdio>
#include <cstdlib>
#include <main.h>
#include <string.h>
#include <string>
#include <syscalls.h>

auto hello_str = "hello from stm32 using hal library!";
void mainxx() {

    puts (hello_str);
    int a = 0;
    scanf ("%d", &a);
    printf ("%d\n", a + 19);

    /* Infinite loop */
    while (1) {
        HAL_GPIO_TogglePin (GPIOC, GPIO_PIN_13);
        HAL_Delay (500);
    }
}

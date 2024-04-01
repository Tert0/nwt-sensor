#ifndef NWT_SENSOR_ERROR_H
#define NWT_SENSOR_ERROR_H

#include "config.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdarg.h>
#include <stdio.h>
#include "led.h"

#define E_HCSR04 1
#define E_GPS 2
#define E_SD 3

extern led_t errorLed;

void error_init();

// TODO test if the other core panics too

// TODO unmount sd card (save)

#define panic_unknown(fmt, ...) {printf("\nPANIC:\n");printf(fmt, ##__VA_ARGS__);led_on(&errorLed); sleep_ms(100); panic(fmt, ##__VA_ARGS__);}

#define panic_code(code, fmt, args...) {printf("\nPANIC:\n");printf(fmt, ##args);led_error(&errorLed, code); sleep_ms(100); panic(fmt, ##args);}

#endif // NWT_SENSOR_ERROR_H
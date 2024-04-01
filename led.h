#ifndef NWT_SENSOR_LED_H
#define NWT_SENSOR_LED_H

#include "config.h"
#include "pico/stdio.h"
#include "hardware/pio.h"
#include "led.pio.h"
#include <stdio.h>
#define LED_PIO pio1

typedef struct {
    uint sm;
} led_t;

void led_load();
void led_init(led_t* led, uint pin);

void led_off(led_t* led);

void led_on(led_t* led);

void led_blink(led_t* led);

void led_error(led_t* led, uint8_t error);

#endif //NWT_SENSOR_LED_H

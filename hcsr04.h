#ifndef NWT_SENSOR_HCSR04_H
#define NWT_SENSOR_HCSR04_H

#include "config.h"
#include <stdio.h>
#include "hardware/pio.h"
#include "pico/time.h"
#include "hcsr04.pio.h"
#include "error.h"

#define HCSR04_PIO pio0
#define HCSR04_MAX_COUNT 2883584
#define HCSR04_OK 0
#define HCSR04_OUT_OF_RANGE 1
#define HCSR04_NO_MEASUREMENT 2

typedef struct {
    uint sm;
} hcsr04_t;

void hcsr04_load();

void hcsr04_init(hcsr04_t* hcsr04, uint trigger_pin, uint echo_pin);

// Request measurement
void hcsr04_request(hcsr04_t* hcsr04);

// Wait for measurement result
bool hcsr04_wait(hcsr04_t* hcsr04);

/*
 * distance in mm
 *
 * return value:
 * 0: ok
 * 1: object out of range (no echo received)
 * 2: no measurement result (empty fifo queue)
 */
uint8_t hcsr04_get(hcsr04_t* hcsr04, uint32_t* distance);

int16_t hcsr04_get_distance(hcsr04_t* hcsr04);

void hcsr04_test(hcsr04_t* hcsr04);

#endif //NWT_SENSOR_HCSR04_H

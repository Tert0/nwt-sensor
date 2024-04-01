#ifndef NWT_SENSOR_STATE_H
#define NWT_SENSOR_STATE_H

#include "pico/types.h"

#define STATE_READY_NO 0
#define STATE_READY_YES 1
#define STATE_READY_INIT 2
#define STATE_BATTERY_LEVEL_OK 0
#define STATE_BATTERY_LEVEL_LOW 1
#define STATE_BATTERY_LEVEL_CRITICAL 2

typedef struct {
    uint8_t ready;
    bool running;
    uint8_t battery;
} state_t;

#endif //NWT_SENSOR_STATE_H

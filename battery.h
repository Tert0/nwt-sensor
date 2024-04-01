#ifndef NWT_SENSOR_BATTERY_H
#define NWT_SENSOR_BATTERY_H

#include "config.h"
#include "state.h"
#include "hardware/adc.h"
#include "stdio.h"

#define BATTERY_MAX_VOLTAGE 4.4 // 4.35
#define BATTERY_LOW_VOLTAGE 3.3
#define BATTERY_CRITICAL_VOLTAGE 3.0 // TODO adjust

void battery_init();

float battery_read_voltage();

void battery_get(state_t* state);

#endif //NWT_SENSOR_BATTERY_H

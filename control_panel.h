#ifndef NWT_SENSOR_CONTROL_PANEL_H
#define NWT_SENSOR_CONTROL_PANEL_H

#include "config.h"
#include "pico/types.h"
#include "state.h"
#include "led.h"

typedef struct {
    led_t ledReady;
    led_t ledRunning;
    led_t ledBatteryLevel;
} control_panel_t;

void control_panel_init(control_panel_t* cp);

void control_panel_apply(control_panel_t* cp, state_t* state);

#endif //NWT_SENSOR_CONTROL_PANEL_H

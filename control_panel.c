#include "control_panel.h"
#include "error.h"

void control_panel_init(control_panel_t* cp) {
    led_init(&cp->ledReady, CONTROL_PANEL_READY_LED_PIN);
    led_init(&cp->ledRunning, CONTROL_PANEL_RUNNING_LED_PIN);
    led_init(&cp->ledBatteryLevel, CONTROL_PANEL_BATTERY_LED_PIN);
}

void control_panel_apply(control_panel_t* cp, state_t* state) {
    switch (state->ready) {
        case STATE_READY_INIT:
            led_blink(&cp->ledReady);
            break;
        case STATE_READY_YES:
            led_on(&cp->ledReady);
            break;
        case STATE_READY_NO:
            led_off(&cp->ledReady);
            break;
        default:
            panic_unknown("control_panel_apply: unreachable case state->ready == %d", state->ready);
    }

    if(state->running) {
        led_on(&cp->ledRunning);
    } else {
        led_off(&cp->ledRunning);
    }

    switch (state->battery) {
        case STATE_BATTERY_LEVEL_OK:
            led_off(&cp->ledBatteryLevel);
            break;
        case STATE_BATTERY_LEVEL_LOW:
            led_blink(&cp->ledBatteryLevel);
            break;
        case STATE_BATTERY_LEVEL_CRITICAL:
            led_on(&cp->ledBatteryLevel);
            break;
        default:
        panic_unknown("control_panel_apply: unreachable case state->battery == %d", state->battery);
    }
}
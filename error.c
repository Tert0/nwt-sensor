#include "error.h"

led_t errorLed;

void error_init() {
    led_init(&errorLed, CONTROL_PANEL_ERROR_LED_PIN);
    led_off(&errorLed);
}
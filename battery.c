#include "battery.h"

#if BATTERY_POWERED

void battery_init() {
    adc_init();

    adc_gpio_init(28);

    adc_select_input(2);
}

float battery_read_voltage() {
    float voltage = (3.3f / (1 << 12)) * (float) adc_read();
    /* calculate battery voltage
     *
     * Voltage divider:
     * GND - 330Ω - ADC2 - 200Ω - BATTERY
     * Vi = Vo * (53/33)
     */

    return voltage * (53.0f / 33.0f);
}

void battery_get(state_t* state) {
    float voltage = battery_read_voltage();
#if defined(DEBUG) && DEBUG >= 2
    printf("Read battery voltage: %f\n", voltage);
#endif
    if(voltage <= BATTERY_CRITICAL_VOLTAGE) {
        state->battery = STATE_BATTERY_LEVEL_CRITICAL;
    } else if(voltage <= BATTERY_LOW_VOLTAGE) {
        state->battery = STATE_BATTERY_LEVEL_LOW;
    } else {
        state->battery = STATE_BATTERY_LEVEL_OK;
    }
}

#endif
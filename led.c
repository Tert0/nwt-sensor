#include "led.h"

bool ledProgramLoaded = false;
uint ledProgramOffset;

void led_load() {
    ledProgramOffset = pio_add_program(LED_PIO, &led_program);
    ledProgramLoaded = true;
#ifdef DEBUG
    printf("Loaded LED pio program\n");
#endif
}
void led_init(led_t* led, uint pin) {
    if(!ledProgramLoaded) led_load();
    led->sm = pio_claim_unused_sm(LED_PIO, true);
    led_program_init(LED_PIO, led->sm, ledProgramOffset, pin);
}

void led_off(led_t* led) {
    pio_sm_put(LED_PIO, led->sm, 0);
}

void led_on(led_t* led) {
    pio_sm_put(LED_PIO, led->sm, 1);
}

void led_blink(led_t* led) {
    pio_sm_put(LED_PIO, led->sm, 2);
}

void led_error(led_t* led, uint8_t error) {
    pio_sm_put(LED_PIO, led->sm, 2 + error);
}
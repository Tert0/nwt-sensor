#include "config.h"
#include <stdio.h>
#include <hardware/adc.h>
#include "pico/stdlib.h"
#include "lwgps/lwgps.h"
#include "pico/multicore.h"
#include "gps.h"
#include "hcsr04.h"
#include "error.h"
#include "data.h"
#include "sd.h"
#include "control_panel.h"
#include "battery.h"

control_panel_t c_panel;

state_t state = {
        .ready = STATE_READY_INIT,
        .running = false,
        .battery = STATE_BATTERY_LEVEL_OK
};

// this will be synced with state.running; set by irq
bool running = false;

absolute_time_t lastButtonPress;

sd_t sd;

void on_button_press(uint gpio, uint32_t events) {
    if(absolute_time_diff_us(lastButtonPress, get_absolute_time()) > 1000000) {
#if DEBUG && DEBUG >= 3
        printf("button pressed\n");
#endif
        running  = !running;
    }
    lastButtonPress = get_absolute_time();
}

int main()
{
#ifdef PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, true);
    gpio_put(PICO_DEFAULT_LED_PIN, true);
#endif
    stdio_init_all();
    error_init();

    sleep_ms(2000);

#if BATTERY_POWERED
    battery_init();

    float voltage = battery_read_voltage();
    if(voltage > BATTERY_MAX_VOLTAGE) {
        printf("Detected high voltage (%fV > %fV). The device is probably USB powered\n", voltage, BATTERY_MAX_VOLTAGE);
    }

    battery_get(&state);
#endif

    // Initialize control panel
    control_panel_init(&c_panel);
    control_panel_apply(&c_panel, &state);

    lwgps_t gps;
    gps_init(&gps);
    sleep_ms(10000);
    gps_wait(&gps);
    gps_set_rtc(&gps);

    // start sd card initialisation (requires correct rtc time)
    sd_init(&sd);

    hcsr04_load();

    hcsr04_t hcsr04_1; // right
    hcsr04_t hcsr04_2; // back
    hcsr04_t hcsr04_3; // left

    hcsr04_init(&hcsr04_1, HCSR04_RIGHT_TRIGGER_PIN,HCSR04_RIGHT_ECHO_PIN);
    hcsr04_init(&hcsr04_2, HCSR04_BACK_TRIGGER_PIN, HCSR04_BACK_ECHO_PIN);
    hcsr04_init(&hcsr04_3, HCSR04_LEFT_TRIGGER_PIN, HCSR04_LEFT_ECHO_PIN);

    hcsr04_test(&hcsr04_1);
    hcsr04_test(&hcsr04_2);
    hcsr04_test(&hcsr04_3);

    state.ready = STATE_READY_YES;
    control_panel_apply(&c_panel, &state);

    // button press interrupt
    gpio_set_irq_enabled_with_callback(CONTROL_PANEL_BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, &on_button_press);

    while(true) {
        // wait until measurement started
        while(!running){}

#ifdef DEBUG
        printf("started measurement\n");
#endif

        state.running = running;
        control_panel_apply(&c_panel, &state);

        sd_write(&sd, "\n", 1);
        while(running) {
#if DEBUG && DEBUG >= 2
            printf("started measurement iter\n");
#endif
            absolute_time_t iterBegin = get_absolute_time();
            int16_t distance_right, distance_back, distance_left;

            hcsr04_request(&hcsr04_1);
            hcsr04_request(&hcsr04_2);
            hcsr04_request(&hcsr04_3);

#if BATTERY_POWERED
            battery_get(&state);
            control_panel_apply(&c_panel, &state);
#endif

            hcsr04_wait(&hcsr04_1);
            hcsr04_wait(&hcsr04_2);
            hcsr04_wait(&hcsr04_3);

            distance_right = hcsr04_get_distance(&hcsr04_1);
            distance_back = hcsr04_get_distance(&hcsr04_2);
            distance_left = hcsr04_get_distance(&hcsr04_3);

            data_t data = {
                    .distance1 = distance_right,
                    .distance2 = distance_back,
                    .distance3 = distance_left,
                    .latitude = gps.latitude,
                    .longitude = gps.longitude,
                    .time = gps.hours * 10000 + gps.minutes * 100 + gps.seconds,
                    .date = gps.date * 10000 + gps.month * 100 + gps.year,
                    .pulse = -1
            };

            char buff[100];

            data_serialize(&data, buff);

            char* ptr = buff;

            while(*ptr) ptr++;

            int len = ptr - buff;

            sd_write(&sd, buff, len);
            sd_sync(&sd);

            sleep_us(100000 - absolute_time_diff_us(iterBegin, get_absolute_time()));
#if DEBUG && DEBUG >= 2
            printf("ended measurement iter\n");
#endif
        }
        sd_sync(&sd);
        state.running = running;
        control_panel_apply(&c_panel, &state);
#ifdef DEBUG
        printf("ended measurement\n");
#endif
    }
}

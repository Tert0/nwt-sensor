#include <hardware/uart.h>
#include <hardware/gpio.h>
#include <pico/time.h>
#include <stdio.h>
#include "gps.h"

absolute_time_t last_data_received;
lwgps_t* lw_gps;

void on_uart_rx() {
    while(uart_is_readable(GPS_UART_ID)) {
        // TODO maybe buffer input or use dma
        char c = uart_getc(GPS_UART_ID);
        lwgps_process(lw_gps, &c, 1, NULL);
    }
    last_data_received = get_absolute_time();
}


void gps_init(lwgps_t* gps) {
    lw_gps = gps;

    // Set up our UART with the required speed.
    uart_init(GPS_UART_ID, GPS_BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(GPS_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GPS_UART_RX_PIN, GPIO_FUNC_UART);

    lwgps_init(gps);

    // Set Interrupt Handler
    irq_set_exclusive_handler(GPS_UART_IRQ, on_uart_rx);
    // Enable Interrupt
    irq_set_enabled(GPS_UART_IRQ, true);

    uart_set_irq_enables(GPS_UART_ID, true, false);
#ifdef DEBUG
    puts("Initialized GPS\n");
#endif
}

void gps_wait(lwgps_t* gps) {
#ifdef DEBUG
    printf("Waiting for GPS\n");
#endif
    sleep_ms(3500);
    bool started = false;
    int i = 0;
    while(
            gps->is_valid == 0 ||
            gps->fix == 0 || // invalid
            gps->fix_mode == 1 || // no fix
            gps->latitude == 0.0 ||
            gps->longitude == 0.0 ||
            gps->sats_in_view <= 1
            ) {
        sleep_ms(100);
        started = true;
        if(i%20==0) {
#ifdef DEBUG
            int64_t diff = absolute_time_diff_us(last_data_received, get_absolute_time());
            printf("GPS: %d/%d (%llds ago)\n", gps->sats_in_use, gps->sats_in_view, diff / 1000000);
#endif
            if(diff >= 10000000) {
                panic_code(E_GPS, "GPS module: no data for %lldms\n", diff / 1000);
            }
        }
        i++;
    }

    if(started) {
        sleep_ms(1000);
    }
#ifdef DEBUG
    printf("Waited (successfully) for GPS\n");
#endif
}

void gps_set_rtc(lwgps_t* gps) {
    datetime_t t = {
            .year = (int16_t) (2000 + gps->year), // Sorry to all after 2099. I could not care less
            .month = (int8_t) gps->month,
            .day = (int8_t) gps->date,
            .hour = (int8_t) gps->hours,
            .min = (int8_t) gps->minutes,
            .sec = (int8_t) gps->seconds
    };

    rtc_init();
    rtc_set_datetime(&t);

    // clk_sys is >2000x faster than clk_rtc, so datetime is not updated immediately when rtc_get_datetime() is called.
    // tbe delay is up to 3 RTC clock cycles (which is 64us with the default clock settings)
    sleep_us(64);

#ifdef DEBUG
    char datetime_buff[100];
    datetime_to_str(datetime_buff, sizeof(datetime_buff), &t);
    printf("set rtc to %s\n", datetime_buff);
#endif
}
#ifndef NWT_SENSOR_GPS_H
#define NWT_SENSOR_GPS_H

#include "lwgps/lwgps.h"
#include "config.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "error.h"

#define GPS_UART_ID uart0
#define GPS_BAUD_RATE 9600
#define GPS_UART_TX_PIN 16
#define GPS_UART_RX_PIN 17
#define GPS_UART_IRQ UART0_IRQ

void gps_init(lwgps_t* gps);

void gps_wait(lwgps_t* gps);

void gps_set_rtc(lwgps_t* gps);

#endif //NWT_SENSOR_GPS_H

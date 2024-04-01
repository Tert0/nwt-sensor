#ifndef NWT_SENSOR_DATA_H
#define NWT_SENSOR_DATA_H

#include "pico/stdlib.h"
#include <stdio.h>

typedef struct {
    int16_t distance1; // distance < 0 -> null
    int16_t distance2;
    int16_t distance3;
    double latitude;
    double longitude;
    uint32_t time;
    uint32_t date;
    int16_t pulse; // pulse < 0 -> null
} data_t;

void data_serialize(const data_t* data, char* str);

#endif //NWT_SENSOR_DATA_H

#include "data.h"

void data_serialize(const data_t* data, char* str) {
    int n;
    char* ptr = str;

    n = sprintf(ptr, "$");
    ptr += n;

    if(data->distance1 < 0) {
        n = sprintf(ptr, ",");
    } else {
        n = sprintf(ptr, "%d,", data->distance1);
    }
    ptr += n;

    if(data->distance2 < 0) {
        n = sprintf(ptr, ",");
    } else {
        n = sprintf(ptr, "%d,", data->distance2);
    }
    ptr += n;

    if(data->distance3 < 0) {
        n = sprintf(ptr, ",");
    } else {
        n = sprintf(ptr, "%d,", data->distance3);
    }
    ptr += n;

    n = sprintf(ptr, "%.6f,%.6f,", data->latitude, data->longitude);
    ptr += n;

    if(data->pulse < 0) {
        n = sprintf(ptr, ",");
    } else {
        n = sprintf(ptr, "%d,", data->pulse);
    }
    ptr += n;

    sprintf(ptr, "%06lu,%06lu,", data->time, data->date);

    uint8_t checksum = 0;

    char* c = str;
    while(*c) {
        if(*c != '$' && *c != ',') checksum ^= *c;
        c++;
    }

    sprintf(c, "%d\n", checksum);
}
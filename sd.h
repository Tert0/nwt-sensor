#ifndef NWT_SENSOR_SD_H
#define NWT_SENSOR_SD_H

#include "hw_config.h"
#include "error.h"
#include "f_util.h"
#include "config.h"

typedef struct {
    sd_card_t* sd;
    FIL file;
    bool dirty;
} sd_t;

void sd_init(sd_t* sd);

void sd_destroy(sd_t* sd);

unsigned int sd_write(sd_t* sd, const void* buff, uint32_t size);

void sd_sync(sd_t* sd);

void sd_copy_file(sd_t* sd, char* srcFile, char* dstFile);

#endif //NWT_SENSOR_SD_H

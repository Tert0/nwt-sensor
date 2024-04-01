#include "sd.h"

#define FILENAME "DATA"

void sd_init(sd_t* sd) {
    if (!sd_init_driver()) {
        panic_code(E_SD, "ERROR: Could not initialize SD card\n");
        return;
    }

#ifdef DEBUG
    printf("initialized SD card driver\n");
#endif

    // Mount SD card
    sd->sd = sd_get_by_num(0);
    sd_card_t* pSD = sd->sd;
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) panic_code(E_SD, "f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
#ifdef DEBUG
    printf("mounted SD card\n");
#endif

    // Create backups directory
    fr = f_mkdir("backups");
    if(FR_OK != fr && FR_EXIST != fr)
        panic_code(E_SD, "f_mkdir(backups) error: %s (%d)\n", FRESULT_str(fr), fr);

    FILINFO fInfo;
    fr = f_stat(FILENAME, &fInfo);
    bool existed = fr == FR_OK && fInfo.fsize > 0;

    // Open file
    fr = f_open(&sd->file, FILENAME, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        panic_code(E_SD, "f_open(%s) error: %s (%d)\n", FILENAME, FRESULT_str(fr), fr);

    sd->dirty = false;

    /*if(existed) {
        sd_write(sd, "\n", 1);
    }*/
}

unsigned int sd_write(sd_t* sd, const void* buff, uint32_t size) {
    sd->dirty = true;
    unsigned int n;
    f_write(&sd->file, buff, size, &n);
    return n;
}

void sd_sync(sd_t* sd) {
    FRESULT fr = f_sync(&sd->file);
    if(fr == FR_OK)
        sd->dirty = false;
    else
        panic_code(E_SD, "f_sync error: %s (%d)\n", FRESULT_str(fr), fr);
}

void sd_destroy(sd_t* sd) {
    if(sd->dirty) {
        sd_sync(sd);
    }

    FRESULT fr = f_close(&sd->file);
    if(FR_OK != fr)
        panic_code(E_SD, "f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    fr = f_unmount(sd->sd->pcName);
    if(FR_OK != fr)
        panic_code(E_SD, "f_unmount error: %s (%d)\n", FRESULT_str(fr), fr);

#ifdef DEBUG
    printf("closed file and umounted sd card\n");
#endif
}
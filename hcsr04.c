#include "hcsr04.h"

uint hcsr04Offset;

void hcsr04_load() {
    hcsr04Offset = pio_add_program(HCSR04_PIO, &hcsr04_program);
#ifdef DEBUG
    printf("Loaded HCSR04 pio program\n");
#endif
}

void hcsr04_init(hcsr04_t* hcsr04, uint trigger_pin, uint echo_pin) {
    hcsr04->sm = pio_claim_unused_sm(HCSR04_PIO, true);

    hcsr04_program_init(HCSR04_PIO, hcsr04->sm, hcsr04Offset, trigger_pin, echo_pin);
#ifdef DEBUG
    printf("initialized HCSR04 on pio core %d with pins %d, %d\n", hcsr04->sm, trigger_pin, echo_pin);
#endif
}

void hcsr04_request(hcsr04_t* hcsr04) {
    pio_sm_put(HCSR04_PIO, hcsr04->sm, 1);
#if DEBUG && DEBUG >= 2
    printf("Requested HCSR04 measurement on %d\n", hcsr04->sm);
#endif
}

bool hcsr04_wait(hcsr04_t* hcsr04) {
    absolute_time_t start = get_absolute_time();
    while(pio_sm_is_rx_fifo_empty(HCSR04_PIO, hcsr04->sm)) {
        if(absolute_time_diff_us(start, get_absolute_time()) >= 50000) {
#ifdef DEBUG
            printf("HCSR04 wait timed out on %d\n", hcsr04->sm);
#endif
            // timeout after 50ms
            return false;
        }
        sleep_ms(1);
    }
    return true;
}

uint8_t hcsr04_get(hcsr04_t* hcsr04, uint32_t* distance) {
    if(pio_sm_is_rx_fifo_empty(HCSR04_PIO, hcsr04->sm)) {
        return HCSR04_NO_MEASUREMENT;
    }

    uint32_t value = pio_sm_get(HCSR04_PIO, hcsr04->sm);

    if(value == 0) {
        return HCSR04_OUT_OF_RANGE;
    }

    // Each countdown cycle takes 2 clock cycles.
    uint64_t cycles = (HCSR04_MAX_COUNT - value) * 2;

    /* Explanation of values:
     * 1/125000000: 125MHz PIO clock
     * 343 m/s: speed of sound
     * 1000: get millimeters instead of meters
     * 1/2: the measured time is the two-way distance, and we want the one way distance
     */
    *distance = (uint32_t) (cycles * 1000 * 343 / 125000000 / 2);

    return HCSR04_OK;
}

int16_t hcsr04_get_distance(hcsr04_t* hcsr04) {
    uint32_t distance;
    if(hcsr04_get(hcsr04, &distance) != HCSR04_OK) {
        return -1;
    }
    if(distance > 5000) {
        return -1;
    }
    return (int16_t) distance;
}

void hcsr04_test(hcsr04_t* hcsr04) {
    hcsr04_request(hcsr04);

    bool has_result = hcsr04_wait(hcsr04);

    uint32_t d;
    uint8_t result = hcsr04_get(hcsr04, &d);
    if(!has_result && result == HCSR04_NO_MEASUREMENT) {
        panic_code(E_HCSR04, "hcsr04(%d) error: no measurement result (%d and %d)\n", hcsr04->sm, has_result, result)
    }

#ifdef DEBUG
    printf("hcsr04(%d) successfully tested\n", hcsr04->sm);
#endif
}
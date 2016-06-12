#ifndef _UPS_DEFS_H_
#define _UPS_DEFS_H_

#include <stdint.h>

typedef struct {
    uint8_t report_id;
    uint8_t design_capacity;
    uint8_t capacity_granularity_1;
    uint8_t capacity_granularity_2;
    uint8_t warning_capacity_limit;
    uint8_t remaining_time_limit;
    uint8_t full_charge_capacity;
} report7_t;

typedef struct {
    uint8_t report_id;
    uint8_t remaining_capacity;
    uint16_t runtime_to_empty;
    uint16_t remaining_time_limit;
} report8_t;

enum {
    PWRFLAG_ACPRESENT = 0x01,
    PWRFLAG_CHARGING = 0x02,
    PWRFLAG_DISCHARGING = 0x04,
    PWRFLAG_BELOWCAPACITYLIMIT = 0x08,
    PWRFLAG_FULLYCHARGED = 0x10,
    PWRFLAG_REMAININGTIMELIMITEXPIRED = 0x20,
};

typedef struct {
    uint8_t report_id;
    uint8_t flags;
} report11_t;

typedef struct {
    uint8_t report_id;
    uint8_t data;
} report_byte_t;

typedef struct {
    uint8_t report_id;
    uint16_t data;
} report_word_t;

typedef struct {
    uint8_t report_id;
    uint8_t data[6];
} report_debug_t;

#endif
#ifndef _UPS_H_
#define _UPS_H_

#include <stdint.h>
#include "ups_defs.h"

extern report8_t reportBuffer8;
extern report11_t reportBuffer11;
extern report7_t reportBuffer7;
extern report_debug_t reportBufferDebugOut, reportBufferDebugIn;

void poll_batt(void);
void poll_status(void);
void poll_load(void);
void report_fill(void);
void ups_init(void);

double ups_force_scale(void);

extern double adc_to_percent(double);

#endif
/*
This is an example of a battery state of charge curve
implemented using a look up table stored in flash memory

The voltage of the battery varies too much between under load annd not underload
hence this method is not good enough to be used

This bit of code is not really tested.
*/

#include "ups.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <math.h>

#define ADC_TO_VOLTAGE(x) ((x / 65535.0) * 14.0)

PROGMEM const uint16_t batt_curve[] = {
 // mV   , percent
    65535, 100,
    12800, 100,
    12750, 75,
    12500, 50,
    12000, 30,
    11500, 10,
    11000, 0,
    0, 0
};

double adc_to_percent(double x)
{
    double voltage = ADC_TO_VOLTAGE(x);
    voltage *= 1000;
    uint16_t v16 = (uint16_t)lround(voltage);
    uint8_t i;
    for (i = 0; i < 255; i += 2)
    {
        uint16_t tvh = pgm_read_word(&batt_curve[i]);
        uint16_t tph = pgm_read_word(&batt_curve[i + 1]);
        uint16_t tvl = pgm_read_word(&batt_curve[i + 2]);
        uint16_t tpl = pgm_read_word(&batt_curve[i + 3]);
        if (v16 > tvl)
        {
            double vrange = tvh - tvl;
            double vdiff = v16 - tvl;
            double prange = tph - tpl;
            double r;
            if (prange == 0) {
                return ((double)tpl) / ((double)100.0);
            }
            r = (vdiff / vrange) * prange;
            r += (double)tpl;
            return r / 100.0;
        }
        if (tvl <= 0)
        {
            return 0;
        }
    }
}

#include "ups.h"
#include "adc.h"
#include "config.h"

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <math.h>

#ifdef USE_AUTO_SCALE
double ups_load_scaler(void);
double ups_save_scaler(double x);
#endif

int8_t adc_to_status(uint16_t adc)
{
	#define IS_IN_RANGE(x, y, r) (((x) < ((y) + (r))) && ((x) > ((y) - (r))))
	if (IS_IN_RANGE(adc, 19133, 1000) || adc < 19682)
	{
		// 220K and 100K and 51K
		return (1 << 2) | (1 << 1) | (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 21329, 1000))
	{
		// 100K and 51K
		return (1 << 1) | (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 24735, 1000))
	{
		// 220K and 51K
		return (1 << 2) | (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 28533, 1000))
	{
		// 51K
		return (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 34417, 1000))
	{
		// 220K and 100K
		return (1 << 2) | (1 << 1);
	}
	else if (IS_IN_RANGE(adc, 42239, 1000))
	{
		// 100K
		return (1 << 2);
	}
	else if (IS_IN_RANGE(adc, 58079, 1000))
	{
		// 220K
		return (1 << 2);
	}
	else if (adc > (65535 - 2000))
	{
		// none
		return 0x00;
	}
	return -1;
}

#define FILTER_CONST 0.9

double batt_percent = 0;
uint8_t status_flags = 0;
volatile double adc_voltage_scale;

void poll_batt(void)
{
    #ifdef FILTER_CONST
    static double filtered = 0;
    #endif
    double analog;
    uint16_t adc;

    adc = adc_read(BATT_CHAN);

    reportBufferDebugOut.report_id = 0x20;
    reportBufferDebugOut.data[0] = adc & 0xFF;
    reportBufferDebugOut.data[1] = adc >> 8;

    analog = adc;
    analog *= adc_voltage_scale;
    #ifdef FILTER_CONST
    if (filtered == 0) {
        filtered = analog;
    }
    filtered = (filtered * FILTER_CONST) + (analog * (1 - FILTER_CONST));
    analog = filtered;
    #endif

    batt_percent = adc_to_percent(analog);
}

void poll_status(void)
{
    enum
    {
        STS_UNKNOWN,
        STS_DISCHARGING,
        STS_RECHARGING,
        STS_FULL,
    };

    static int8_t prev_sts = -1;
    static uint8_t stable = 0;
    static char charger_intelligent = 0;
    static double max_batt = 0;
    int8_t tmpsts, sts = -1;
    int8_t rpt = -1;
    uint16_t adc;
    #ifdef USE_AUTO_SCALE
    static uint64_t trusted_full_cnt = 0;
    static char scaler_dirty = 0;
    #endif

    adc = adc_read(STATUS_CHAN);

    reportBufferDebugOut.report_id = 0x20;
    reportBufferDebugOut.data[2] = adc & 0xFF;
    reportBufferDebugOut.data[3] = adc >> 8;

    tmpsts = adc_to_status(adc);
    if (tmpsts == prev_sts && tmpsts >= 0)
    {
        if (stable > 3)
        {
            sts = tmpsts;
        }
        else
        {
            stable++;
            return;
        }
    }
    else
    {
        prev_sts = tmpsts;
        stable = 0;
        return;
    }

    prev_sts = tmpsts;

    if (sts != 0x00 && sts != (1 << 2))
    {
        charger_intelligent = 1;
    }

    if (charger_intelligent != 0 && (sts & 0x01) != 0x00)
    {
        max_batt = 1.0;
    }
    else if (charger_intelligent == 0 && batt_percent > max_batt)
    {
        max_batt = batt_percent > 1.0 ? 1.0 : batt_percent;
    }
    else if (batt_percent <= 0.9 && sts == 0x00)
    {
        max_batt = batt_percent;
    }

    if (sts == 0)
    {
        rpt = STS_DISCHARGING;
    }
    else
    {
        if (max_batt >= 1.0)
        {
            rpt = STS_FULL;
        }
        else
        {
            rpt = STS_RECHARGING;
        }
    }

    if (charger_intelligent != 0)
    {
        if (sts & (1 << 0))
        {
            rpt = STS_FULL;
            #ifdef USE_AUTO_SCALE
            if (trusted_full_cnt > 64) // has been full charge for a while
            {
                // tweak the scaler
                if (batt_percent < 1.00)
                {
                    adc_voltage_scale += 0.001;
                    scaler_dirty = 1;
                }
                else if (batt_percent > 1.02)
                {
                    adc_voltage_scale -= 0.001;
                    scaler_dirty = 1;
                }

                if (trusted_full_cnt == 600) // save only once
                {
                    if (scaler_dirty != 0)
                    {
                        ups_save_scaler(adc_voltage_scale);
                        scaler_dirty = 0;
                    }
                }
            }
            trusted_full_cnt++; // don't really care about overflow, doesn't hurt us
            #endif
        }
        else if (sts & (1 << 1))
        {
            rpt = STS_RECHARGING;
        }
        else
        {
            rpt = STS_DISCHARGING;
        }
    }

    if (rpt == STS_DISCHARGING)
    {
        status_flags = (1 << 2);
        #ifdef USE_AUTO_SCALE
        trusted_full_cnt = 0;
        #endif
    }
    else if (rpt == STS_RECHARGING)
    {
        status_flags = (1 << 1) | (1 << 0);
        #ifdef USE_AUTO_SCALE
        trusted_full_cnt = 0;
        #endif
    }
    else if (rpt == STS_FULL)
    {
        status_flags = (1 << 4) | (1 << 0);
    } 
}

void report_fill(void)
{
    uint16_t tmp;
    uint16_t total = 100; // TODO: current measurement to predict
    double percent = batt_percent;
    if (percent > 0.98) {
    	percent = 1.0;
    }
    else if (percent < 0.02) {
    	percent = 0.0;
    }

    reportBuffer8.report_id = 0x08;
    tmp = (uint8_t)lround(100 * percent);
    reportBuffer8.remaining_capacity = tmp > 255 ? 255 : tmp;
    reportBuffer8.remaining_time_limit = (uint16_t)lround(total);
    tmp = (uint16_t)lround(total * percent);
    reportBuffer8.runtime_to_empty = tmp > total ? total : tmp;

    reportBuffer11.flags = status_flags;

    reportBuffer7.report_id = 0x07;
    reportBuffer7.design_capacity = 100;
    reportBuffer7.capacity_granularity_1 = 1;
    reportBuffer7.capacity_granularity_2 = 1;
    reportBuffer7.warning_capacity_limit = 50;
    reportBuffer7.remaining_time_limit = (uint8_t)lround(total);
    reportBuffer7.full_charge_capacity = 100;

    reportBufferDebugOut.report_id = 0x20;
    reportBufferDebugOut.data[4] = 0;
    reportBufferDebugOut.data[5] = 0;
}

void ups_init()
{
    #ifdef USE_AUTO_SCALE
    adc_voltage_scale = ups_load_scaler();
    #else
    adc_voltage_scale = 1.0;
    #endif

    adc_init();
    poll_batt();
    poll_status();
    report_fill();
}

#ifdef USE_AUTO_SCALE
double ups_load_scaler(void)
{
    uint8_t tmpbuf[sizeof(double)];
    double* scaler;
    char allzeros = 1, allones = 1, i;
    eeprom_read_block(tmpbuf, 8, sizeof(double));

    // check if data is valid
    for (i = 0; i < sizeof(double); i++)
    {
        if (tmpbuf[i] != 0)
        {
            allzeros = 0;
        }
        if (tmpbuf[i] != 0xFF)
        {
            allones = 0;
        }
    }
    if (allzeros == 0 && allones != 0)
    {
        scaler = (double*)tmpbuf;
        if (scaler > 0.5 && scaler < 5) {
            return (*scaler);
        }
    }
    return 1.0; // invalid, so return default
}

double ups_save_scaler(double x)
{
    eeprom_write_block((const void*)&x, (void*)8, sizeof(double));
}

double ups_force_scale(void)
{
    adc_voltage_scale = 1.0 / batt_percent;
    adc_voltage_scale += 0.001;
    ups_save_scaler(adc_voltage_scale);
}

#endif

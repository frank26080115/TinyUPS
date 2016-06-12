#include "ups.h"
#include "adc.h"
#include "config.h"

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#define ADC_TO_VOLTAGE(x) ((x / 65535.0) * 14.0)

int8_t adc_to_status(uint16_t adc)
{
	#define IS_IN_RANGE(x, y, r) (((x) < ((y) + (r))) && ((x) > ((y) - (r))))
	if (IS_IN_RANGE(adc, 19133, 1000) || adc < 19682) // 19712
	{
		// 220K and 100K and 51K
		return (1 << 2) | (1 << 1) | (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 21329, 1000))
	{
		// 100K and 51K
		return (1 << 1) | (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 24735, 1000)) // 25152
	{
		// 220K and 51K
		return (1 << 2) | (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 28533, 1000))
	{
		// 51K
		return (1 << 0);
	}
	else if (IS_IN_RANGE(adc, 34417, 1000)) // 34688
	{
		// 220K and 100K
		return (1 << 2) | (1 << 1);
	}
	else if (IS_IN_RANGE(adc, 42239, 1000))
	{
		// 100K
		return (1 << 2);
	}
	else if (IS_IN_RANGE(adc, 58079, 2000)) // 57088
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
    double percent, load;
    uint16_t adc;

    adc = adc_read(BATT_CHAN);

    load = CONSTANT_OUTPUT_BATTERY_LOAD;
    if (status_flags == ((1 << 4) | (1 << 0))) {
        load = 0.0;
    }

    reportBufferDebugOut.report_id = 0x20;
    reportBufferDebugOut.data[0] = adc & 0xFF;
    reportBufferDebugOut.data[1] = adc >> 8;

    analog = adc;

    if (adc_voltage_scale <= 0)
    {
        double inc;
        for (inc = 0.1; inc <= 100.0; inc += 0.01)
        {
            if (calc_remaining_percent(ADC_TO_VOLTAGE(analog * inc), load) >= 1.0) {
                break;
            }
        }
        adc_voltage_scale = inc;
    }

    analog *= adc_voltage_scale;
    #ifdef FILTER_CONST
    if (filtered == 0) { // no need for ramp up
        filtered = analog;
    }
    filtered = (filtered * FILTER_CONST) + (analog * (1 - FILTER_CONST));
    analog = filtered;
    #endif
    // recharging curve is not implemented, so load cannot be negative
    percent = calc_remaining_percent(ADC_TO_VOLTAGE(analog), load) / 100.0;

    #ifdef FAKE_ALWAYS_FULL
    percent = 1.0;
    #endif

    #ifdef ENABLE_TEST_SHUTDOWN
    if (bit_is_clear(PINB, 3)) {
        percent = 0.01;
    }
    #endif

    batt_percent = percent;
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
    int8_t tmpsts, sts = -1;
    int8_t rpt = -1;
    uint16_t adc;

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

    if (sts == 0)
    {
        rpt = STS_DISCHARGING;
    }
    else
    {
        if (batt_percent >= 0.98)
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
    }
    else if (rpt == STS_RECHARGING)
    {
        status_flags = (1 << 1) | (1 << 0);
    }
    else if (rpt == STS_FULL)
    {
        status_flags = (1 << 4) | (1 << 0);
    }

    #ifdef FAKE_ALWAYS_FULL
    status_flags = (1 << 4) | (1 << 0);
    #endif

    #ifdef ENABLE_TEST_SHUTDOWN
    if (bit_is_clear(PINB, 3)) {
        status_flags = (1 << 2);
    }
    #endif
}

void report_fill(void)
{
    uint16_t tmp;
    uint16_t total = 0x0120; // TODO: current measurement to predict
    double percent = batt_percent;
    if (percent > 0.98) {
        percent = 1.0;
    }
    else if (percent < 0.02) {
        percent = 0.0;
    }

    reportBuffer8.report_id = 0x08;
    tmp = (uint8_t)lround(100 * percent);
    reportBuffer8.remaining_capacity = tmp > 100 ? 100 : tmp;
    #ifdef FAKE_CYBERPOWER
    reportBuffer8.remaining_time_limit = 0x012C;
    tmp = (uint16_t)lround(((double)0x052E) * percent);
    reportBuffer8.runtime_to_empty = tmp;
    #else
    reportBuffer8.remaining_time_limit = (uint16_t)lround(total);
    tmp = (uint16_t)lround(total * percent);
    reportBuffer8.runtime_to_empty = tmp > total ? total : tmp;
    #endif

    reportBuffer11.report_id = 0x0B;
    reportBuffer11.flags = status_flags;

    reportBuffer7.report_id = 0x07;
    #ifdef FAKE_CYBERPOWER
    reportBuffer7.design_capacity = 100;
    reportBuffer7.capacity_granularity_1 = 0x05;
    reportBuffer7.capacity_granularity_2 = 0x0A;
    reportBuffer7.warning_capacity_limit = 0x14;
    reportBuffer7.remaining_time_limit = 0x0A;
    reportBuffer7.full_charge_capacity = 100;
    #else
    reportBuffer7.design_capacity = 100;
    reportBuffer7.capacity_granularity_1 = 1;
    reportBuffer7.capacity_granularity_2 = 1;
    reportBuffer7.warning_capacity_limit = 50;
    reportBuffer7.remaining_time_limit = (uint8_t)lround(total);
    reportBuffer7.full_charge_capacity = 100;
    #endif

    reportBufferDebugOut.report_id = 0x20;
    reportBufferDebugOut.data[4] = 0;
    reportBufferDebugOut.data[5] = 0;
}

void ups_init(void)
{
    uint16_t i;

    #ifdef ENABLE_TEST_SHUTDOWN
    DDRB &= ~_BV(3);
    PORTB |= _BV(3);
    _delay_ms(10);
    #endif

    adc_voltage_scale = ups_load_scaler();

    adc_init();
    for (i = 0; i < 200 || status_flags == 0; i++) {
        poll_batt();
        poll_status();
        report_fill();
        _delay_ms(1);
    }
}

double ups_load_scaler(void)
{
    uint8_t tmpbuf[sizeof(double)];
    double* scaler;
    char allzeros = 1, allones = 1;
    int i;
    eeprom_read_block((void*)tmpbuf, (void*)8, sizeof(double));

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
        if ((*scaler) > 0.5 && (*scaler) < 5.0) {
            return (*scaler);
        }
    }
    return 1.0; // invalid, so return default
}

void ups_save_scaler(double x)
{
    eeprom_write_block((const void*)&x, (void*)8, sizeof(double));
}

void ups_force_scale(void)
{
    adc_voltage_scale = -1; // triggers calculation on next poll
}

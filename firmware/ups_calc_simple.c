#include "ups.h"
#include <stdlib.h>
#include <math.h>

// these values are fake placeholders
#define BATT_MIN (65535 / 4)
#define BATT_MAX 65535

double adc_to_percent(double x)
{
    // straight up linear voltage estimation
    x -= BATT_MIN;
    x /= (BATT_MAX - BATT_MIN);
    return x > 1.0 ? 1.0 : (x < 0.0 ? 0.0 : x);
}
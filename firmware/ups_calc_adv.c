#include "ups.h"
#include <stdlib.h>
#include <math.h>

/*
param v: voltage expressed in volts, range is the typical range for a lead acid battery

param load: load is expressed in C, for example, using 1 amp with a 1Ah battery is 1C, 0.5 amp with 1Ah battery is 0.5C
            positive means discharge, negative means recharge

return percent, range 0.0 to 100.0

*/
double calc_remaining_percent(double v, double load)
{
    double x;

    if (load >= 0) // discharging
    {
        double t1, t2, t3, t3o;
        /*
            The constants below are obtained from approximating
            the curves given from the document titled:
                Lead-Acid Battery State of Charge vs. Voltage
            by author:
                Richard Perez
            published:
                Home Power #36 • August / September 1993
        */
        if (load > 0) // under load
        {
            t1 = (-0.0009875627907627 * load * load) + (-0.0000333799157361 * load) + (-0.0001026050825665);
            t2 = 0.0736670600990142 * load + 0.0186826198807222;
            t3 = -6.8273790276026600 * load + 11.7603312904290000;
        }
        else // no load
        {
            t1 = -0.0001264568764569;
            t2 = 0.0235093240093240;
            t3 = 11.4989510489510000;
        }

        t3o = t3 - v;

        double sqinner = (t2 * t2) - (4 * t1 * t3o);
        if (sqinner < 0.0) {
            return 100.0;
        }
        x = (-t2 + sqrt(sqinner)) / (2 * t1);
    }
    else {
        // TODO
        // voltage curve for recharging is not implemented
    }

    return x;
}

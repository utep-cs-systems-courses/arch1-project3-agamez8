#include <msp430.h>
#include "buzzer.h"
#include "libTimer.h"

void buzzer_set_period(short cycles) // buzzer clock = 2MHz.  (period of 1k results in 2kHz tone)
{
  CCR0 = cycles;
  CCR1 = cycles >> 1; /* one half cycle */
}

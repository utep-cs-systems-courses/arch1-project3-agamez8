#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "switches.h"
#include "lcddraw.h"

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

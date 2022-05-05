#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "switches.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0;

/*
void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}
*/

short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
    secCount = 0;
    redrawScreen = 1;
  }
}
  
void update_shape();

void main()
{
  
  P1DIR |= LED;		/* Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();                 /** enable display lcd */
  switch_init();              /** enable switches */
  buzzer_init();              /** enable buzzer */
  enableWDTInterrupts();      /** enable periodic interrupt */
  or_sr(0x8);	              /** GIE (enable interrupts) */

  u_char width = screenWidth, height = screenHeight;
  
  clearScreen(COLOR_BLUE);
  while (1) {			/* forever */
    if (redrawScreen) {
      drawString5x7(42, 5, "PAC-MAN", COLOR_YELLOW, COLOR_BLUE);
      drawString5x7(12, 115, ". . . * . . . . .", COLOR_WHITE, COLOR_BLUE);
      
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/* CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

// color of the shape
unsigned int SHAPE_COLOR = COLOR_YELLOW;

void
update_shape()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  
  static int colStep = 2;
  static int rowStep = 2;

  static unsigned char step = 0;

  if (step <= 10)
  {
    int startCol = col - step;
    int endCol = col + step;
    int width = 22; // endCol - startCol

    fillRectangle(endCol-step, row-step, width, 1, SHAPE_COLOR); // fill shape with color
    fillRectangle(endCol-step, row+step, width, 1, SHAPE_COLOR);

    // button s1
    if (switches & SW1)
    {
      SHAPE_COLOR = COLOR_BLACK; /* change shape color to black */
      buzzer_off();
    }

    // button s2
    if (switches & SW2)
    {
      SHAPE_COLOR = COLOR_RED;    /* change shape color to red */
      buzzer_off();
    }

    // button s3
    if (switches & SW3)
    {
      SHAPE_COLOR = COLOR_GREEN;  /* change shape color to green */
      buzzer_off();
    }

    // button s4
    if (switches & SW4)
    {
      starwars_song();            /* play a note of the song every time you click the button */
    }

    step ++;
    
  } else {
    col -= colStep*2; // shift shape left
    clearScreen(COLOR_BLUE); // keep screen blue
    step = 0; // reset step
  }
}

/* Switch on S2 */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

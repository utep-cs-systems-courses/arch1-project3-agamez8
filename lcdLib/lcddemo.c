/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  drawString5x7(20,20, "hello :)", COLOR_GREEN, COLOR_RED);

  fillRectangle(30,30, 60, 60, COLOR_YELLOW);

  // draw triangle
  int a, length = 20;
  int fCol = 50, fRow = 50;

  for (a = 0; a < length; a++)
  {
    drawPixel(fCol + a, fRow + a, COLOR_PURPLE);
    drawPixel(fCol + a, fRow, COLOR_PURPLE);
    drawPixel(fCol + length, fRow + a, COLOR_PURPLE);

    if (a%2 == 1)
    {
      for (int v = 0; v < a; v++)
      {
	drawPixel(fCol + a, fRow + v, COLOR_BLACK);
      }
    }
  }

  /*
  // draw asterisk
  int sCol = 50, sRow = 50;
  int s;

  for (s = 0; s < length; s++)
  {
    drawPixel(sCol + s, sRow + s, COLOR_BLACK);
    drawPixel(sCol + (length/2), sRow + s, COLOR_BLACK);
    drawPixel(sCol + s, sRow + (length/2), COLOR_BLACK);
    drawPixel(sCol + s, sRow - (s - length), COLOR_BLACK);
  }
  */
  
}

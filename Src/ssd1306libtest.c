#include "stm32f1xx_hal.h"
#include "ssd1306.h"
#include "stdlib.h"
#include <math.h>


int  dfs;
char str[30];


void SSD1306LibTest(void)
{
	int i,j;
	
    ssd1306ClearScreen(LAYER0|LAYER1);
    for(i = 000; i < 400; i++)
    {
      ssd1306ClearScreen(LAYER1);
      //if (i >= 0 && i < 100)
      {
        for(j = 0; j < 1; j++)
        {
          ssd1306DrawPixel( (rand() % 127), (rand() % 5), WHITE, LAYER0);
        }
        ssd1306DrawString( 10,  0, "  HAPPY   ",  2, WHITE, LAYER1);
        ssd1306DrawString( 10, 16, "   NEW    ",  2, WHITE, LAYER1);
        ssd1306DrawString( 14, 32, "  YEAR    ",  2, WHITE, LAYER1);
        ssd1306ShiftFrameBuffer( 1, 1);
      }
      ssd1306Refresh();
      SSD1306MSDELAY(40);
    }
   
}

#include "stm32f1xx_hal.h"
#include "ssd1306.h"
#include "stdlib.h"
#include <math.h>

uint16_t c = 0;  // cnt
char s[64]={0}, st[64]={0};  // str

uint8_t key[4]={0,0,0,0};  // 4 keys state
const static uint16_t key_pin[4] = {GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_7, GPIO_PIN_6};

extern RTC_HandleTypeDef hrtc;

extern uint16_t uu;


void Draw(void)
{
	int i;
    ssd1306ClearScreen(LAYER0|LAYER1);
	
	//  draw  ----
	/*for(j = 0; j < 2; j++)
    {
	  ssd1306DrawPixel( rand() % 127, rand() % 63, WHITE, LAYER0);
	}*/
	//ssd1306DrawPixel( c % 128, 56, WHITE, LAYER0);
	
	//ssd1306DrawString( 0,  0, "Crystal",  1, WHITE, LAYER0);
	//ssd1306DrawString( 0,  8, "Hammer",   1, WHITE, LAYER0);
	sprintf(s,"%d %d", c, uu);
	ssd1306DrawString( 0, 20, s,  1, WHITE, LAYER0);

	//ssd1306DrawCircle( rand() % 107 +10, rand() % 43 +10, 10, WHITE);

	sprintf(s,"%d %d %d %d  e%d", key[0], key[1], key[2], key[3], hrtc.State);
	ssd1306DrawString( 0, 30, s,  1, WHITE, LAYER0);                     

	ssd1306DrawString( 0, 40, st,  1, WHITE, LAYER0);

      ssd1306Refresh();
	SSD1306MSDELAY(10);
	++c;
   
	//  read keys  --
	//HAL_GPIO_WritePin( GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	for (i=0; i < 4; ++i)
		key[i] = HAL_GPIO_ReadPin(GPIOB, key_pin[i]) == GPIO_PIN_SET ? 0 : 1;

	RTC_TimeTypeDef sTime;
	if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK)
		sprintf(st,"%2d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	
	//while(HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BCD)!= HAL_OK)
	//, sDate.Date,sDate.Month,sDate.Year);
}

#include "stm32f1xx_hal.h"
#include "def.h"
#include "ssd1306.h"


//  var
CRC_HandleTypeDef crc;
SPI_HandleTypeDef spi2;
DMA_HandleTypeDef hdma_spi2_tx;

RTC_HandleTypeDef hrtc;


uint16_t uu = 0;
extern void Draw();


//  RTC init
void RTC_Init()
{
	RCC_OscInitTypeDef osc;
	//uint32_t rtc_freq = 0;
	hrtc.Instance = RTC;

	//__PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_BACKUPRESET_FORCE(); 
	__HAL_RCC_BACKUPRESET_RELEASE();

	// Enable LSE Oscillator
	osc.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc.PLL.PLLState = RCC_PLL_NONE;  // otherwise PLL reconfigured
	osc.LSEState = RCC_LSE_ON;  // external 32.768 kHz clock on OSC
	if (HAL_RCC_OscConfig(&osc) == HAL_OK)
	{	// Connect LSE to RTC
		//__HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSE);
		__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
		//rtc_freq = LSE_VALUE;
	}else
	{	// Enable LSI clock
		osc.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
		osc.PLL.PLLState = RCC_PLL_NONE;
		osc.LSEState = RCC_LSE_OFF;
		osc.LSIState = RCC_LSI_ON;
		if (HAL_RCC_OscConfig(&osc) != HAL_OK) {  }
		// Connect LSI to RTC
		//__HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSI);
		__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSI);
		//rtc_freq = 32000;
	}
	__HAL_RCC_RTC_ENABLE();

	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {  }
}


//  System Clock Configuration
void SystemClock_Config()
{
	RCC_OscInitTypeDef osc;
	RCC_ClkInitTypeDef clk;

	osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc.HSEState = RCC_HSE_ON;
	osc.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	osc.PLL.PLLState = RCC_PLL_ON;
	osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	osc.PLL.PLLMUL = RCC_PLL_MUL9;
	HAL_RCC_OscConfig(&osc);

	clk.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
	clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk.APB1CLKDivider = RCC_HCLK_DIV2;
	clk.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

//  CRC init
void CRC_Init()
{
	crc.Instance = CRC;
	HAL_CRC_Init(&crc);
}

//  SPI2 init
void SPI2_Init()
{
	spi2.Instance = SPI2;
	spi2.Init.Mode = SPI_MODE_MASTER;
	spi2.Init.Direction = SPI_DIRECTION_2LINES;
	spi2.Init.DataSize = SPI_DATASIZE_8BIT;
	spi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi2.Init.NSS = SPI_NSS_SOFT;
	spi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	spi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi2.Init.TIMode = SPI_TIMODE_DISABLE;
	spi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi2.Init.CRCPolynomial = 10;
	HAL_SPI_Init(&spi2);
}

//  Enable DMA
void DMA_Init()
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}


//  Configure pins  ------------
void GPIO_Init()
{
	GPIO_InitTypeDef gpio;

	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOB, CS_Pin|DC_Pin, GPIO_PIN_RESET);

	gpio.Pin = CS_Pin|DC_Pin;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio);
}


//------------------------
int main()
{
	HAL_Init();
	SystemClock_Config();
	
	GPIO_Init();
	DMA_Init();
	CRC_Init();
	SPI2_Init();
	RTC_Init();
	
	//  Display
	uint8_t vcc = SSD1306_CHARGEPUMP;
	ssd1306Init(vcc);
	
	ssd1306ClearScreen(LAYER0 | LAYER1);
	ssd1306Refresh();
	HAL_Delay(100);
	ssd1306Dim(vcc, 1);

	//  Loop
	while (1)
	{
		Draw();
	}
}

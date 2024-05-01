#include "ws2812.h"

static TIM_HandleTypeDef *ws2812_tim_handle;				// WS2812 Timer Handle
static uint32_t ws2812_tim_channel;							// WS2812 Timer PWM Channel

uint8_t ws2812_high = 0;									// ARR * 2/3 (For WS2812 High:	0.84us)
uint8_t ws2812_low 	= 0;									// ARR * 1/3 (For WS2812 Low: 	0.4us)

char ws2812_data_sent_flag = 0;								// Check WS2812 Data Is Sent Complete.
ws2812_color_t color[WS2812_MAX_LED];						// WS2812 RGB Color Structure.
uint16_t payload[(24 * WS2812_MAX_LED) + 50];				// WS2812 Data Payload.

void WS2812_Init(TIM_HandleTypeDef *htim, uint32_t ch)
{
	size_t led = 0;
	
	/* Set The Timer Handle & Channel For WS2812 */
	ws2812_tim_handle	= htim;
	ws2812_tim_channel 	= ch;
	
	/* Calculate High & Low Value */
	ws2812_high = ws2812_tim_handle->Init.Period * 0.7;
	ws2812_low 	= ws2812_tim_handle->Init.Period * 0.35;
	
	/* Reset RGB Value For Any LED */
	for(led = 0; led < WS2812_MAX_LED; led++)
	{
		color[led].r = color[led].g = color[led].b = 0;
	}
}

char WS2812_SetColorHex(uint16_t idx, uint32_t color_hex)
{
	if (idx >= WS2812_MAX_LED)	// if led index is bigger than max led count, return false.
		return 0;
	
	/* Format: 0x00RRGGBB */
	color[idx].r = (color_hex >> 16) & 0xFF;
	color[idx].g = (color_hex >> 8) & 0xFF;
	color[idx].b = (color_hex >> 0) & 0xFF;
	
	return 1;
}

char WS2812_SetColorRgb(uint16_t idx, uint8_t r, uint8_t g, uint8_t b)
{
	if (idx >= WS2812_MAX_LED)	// if led index is bigger than max led count, return false.
		return 0;
	
	color[idx].r = r;
	color[idx].g = g;
	color[idx].b = b;
	
	return 1;
}

void WS2812_Write(void)
{
	size_t led, j;
	uint32_t _color = 0, _payload_cnt = 0;
	
	for(led = 0; led < WS2812_MAX_LED; led++)
	{
		_color = (color[led].g << 16) | (color[led].r << 8) | (color[led].b << 0);	// Package RGB Structure To 0x00GGRRBB
		
		for(j = 0; j < 24; j++)
		{
			if ((_color << 8) & 0x80000000)
				payload[_payload_cnt] = ws2812_high; //	To WS2812 = 1
			else
				payload[_payload_cnt] = ws2812_low;	 // To WS2812 = 0
			
			_color <<= 1; 
			_payload_cnt++;
		}
	}

	// At least 50us to next command
	for(j = 0; j < 50; j++)
	{
		payload[_payload_cnt] = 0;
		_payload_cnt++;
	}
		
	HAL_TIM_PWM_Start_DMA(ws2812_tim_handle, ws2812_tim_channel, (uint32_t *)payload, sizeof(payload) / sizeof(payload[0]));
	
	while (!ws2812_data_sent_flag) {}
	ws2812_data_sent_flag = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(ws2812_tim_handle, ws2812_tim_channel);
	ws2812_data_sent_flag = 1;
}

/**
 * @file ws2812.h
 * @brief WS2812B LED Matrix Driver Library With STM32HAL
 * @author blackcat (minexo79@gmail.com)
 * @version 0.1
 * @date 2024.05.01
 * ============================================================
 * 2024.05.01 - First Release.
*/

#ifndef _WS2812_H
#define _WS2812_H

/* Private includes */
#include "main.h"

/* Private defines */
#ifndef WS2812_MAX_LED
#define WS2812_MAX_LED 12						// WS2812 LED Count
#endif

/* Private typedef */
typedef struct WS2812_Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ws2812_color_t;

/* Private variables */
extern TIM_HandleTypeDef *ws2812_tim_handle;	// WS2812 Timer Handle
extern uint32_t ws2812_tim_channel;				// WS2812 Timer PWM Channel

/**
 * @brief 
 * Initialize WS2812B Driver
 * @param htim WS2812B Timer Handle
 * @param ch WS2812B Timer PWM Channel
 */
void WS2812_Init(TIM_HandleTypeDef *htim, uint32_t ch);

/**
 * @brief 
 * Set WS2812B Color To Specific LED With Hex Value
 * @param idx WS2812B LED Index
 * @param color_hex Hex Value
 * @return char if led index is bigger than max led count, return false.
 */
char WS2812_SetColorHex(uint16_t idx, uint32_t color_hex);

/**
 * @brief 
 * Set WS2812B Color To Specific LED With RGB Value
 * @param idx WS2812B LED Index
 * @param r Red Value
 * @param g Green Value
 * @param b Blue Value
 * @return char if led index is bigger than max led count, return false.
 */
char WS2812_SetColorRgb(uint16_t idx, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief 
 * Write All Colors To WS2812B LED
 */
void WS2812_Write(void);

#endif

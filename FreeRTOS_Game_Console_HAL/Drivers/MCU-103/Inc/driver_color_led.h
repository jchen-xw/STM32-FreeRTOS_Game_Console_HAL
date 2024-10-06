/*
 * @Author: Jchen
 * @Date: 2024-09-26 21:55:08
 * @LastEditors: Jchen
 * @LastEditTime: 2024-09-26 22:06:25
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Drivers\MCU-103\Inc\driver_color_led.h
 */
#ifndef _DRIVER_COLOR_LED_H
#define _DRIVER_COLOR_LED_H

#include <stdint.h>

/**********************************************************************
 * 函数名称： ColorLED_Init
 * 功能描述： 全彩LED的初始化函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void ColorLED_Init(void);

/**********************************************************************
 * 函数名称： ColorLED_Set
 * 功能描述： 全彩LED设置颜色函数
 * 输入参数： color - 24bit颜色,格式为0x00RRGGBB
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void ColorLED_Set(uint32_t color);

/**********************************************************************
 * 函数名称： ColorLED_Test
 * 功能描述： 全彩LED测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 ***********************************************************************/
void ColorLED_Test( void *params );


#endif /* _DRIVER_COLOR_LED_H */


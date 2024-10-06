/*
 * @Author: Jchen
 * @Date: 2024-09-26 13:33:52
 * @LastEditors: Jchen
 * @LastEditTime: 2024-09-26 22:01:03
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Drivers\MCU-103\Inc\driver_led.h
 */
#ifndef _DRIVER_LED_H
#define _DRIVER_LED_H

#define LED_GREEN 1  /* 哪个LED */

/**********************************************************************
 * 函数名称： Led_Init
 * 功能描述： LED初始化函数, 在HAL的初始化代码里已经配置好了GPIO引脚, 
 *            所以本函数可以写为空
 *            但是为了不依赖于stm32cubemx, 此函数也实现了GPIO的配置
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 ***********************************************************************/
int Led_Init(void);

/**********************************************************************
 * 函数名称： Led_Control
 * 功能描述： LED控制函数
 * 输入参数： which-哪个LED, 在driver_led.h里定义, 比如LED_GREEN
 *            on-状态, 1-亮, 0-灭
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 ***********************************************************************/
int Led_Control(int which, int on);

/**********************************************************************
 * 函数名称： Led_Test
 * 功能描述： Led测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 ***********************************************************************/
void Led_Test( void *params );

#endif /* _DRIVER_LED_H */

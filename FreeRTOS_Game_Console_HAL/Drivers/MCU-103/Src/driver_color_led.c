#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core

#include "driver_color_led.h"
#include "OLED.h"
#include "driver_timer.h"
#include "stm32f1xx_hal.h"
#include "tim.h"

#define CHANNEL_RED   TIM_CHANNEL_3
#define CHANNEL_GREEN TIM_CHANNEL_1
#define CHANNEL_BLUE  TIM_CHANNEL_2

extern TIM_HandleTypeDef htim2;

static TIM_HandleTypeDef *g_HPWM_ColorLED = &htim2;

/**********************************************************************
 * 函数名称： ColorLED_Init
 * 功能描述： 全彩LED的初始化函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void ColorLED_Init(void)
{
    MX_TIM2_Init();
}

/**********************************************************************
 * 函数名称： ColorLED_Set
 * 功能描述： 全彩LED设置颜色函数
 * 输入参数： color - 24bit颜色,格式为0x00RRGGBB
 * 输出参数： 无
 * 返 回 值： 无
 ***********************************************************************/
void ColorLED_Set(uint32_t color)
{
    TIM_OC_InitTypeDef sConfigR = {0};
    TIM_OC_InitTypeDef sConfigG = {0};
    TIM_OC_InitTypeDef sConfigB = {0};

    int r,g,b;

    r = (color >> 16) & 0xff;
    g = (color >> 8) & 0xff;
    b = (color >> 0) & 0xff;

    sConfigR.OCMode = TIM_OCMODE_PWM1;        // PWM 输出的两种模式:PWM1 当极性为低,CCR<CNT,输出低电平,反之高电平
    sConfigR.OCPolarity = TIM_OCPOLARITY_LOW; // 设置极性为低(硬件上低电平亮灯)
    sConfigR.OCFastMode = TIM_OCFAST_DISABLE; // 输出比较快速使能禁止(仅在 PWM1 和 PWM2 可设置)
    sConfigR.Pulse = r*2000/255;              // 在 PWM1 模式下,通道 3(RLED)占空比

    sConfigG.OCMode = TIM_OCMODE_PWM1;        // PWM 输出的两种模式:PWM1 当极性为低,CCR<CNT,输出低电平,反之高电平
    sConfigG.OCPolarity = TIM_OCPOLARITY_LOW; // 设置极性为低(硬件上低电平亮灯)
    sConfigG.OCFastMode = TIM_OCFAST_DISABLE; // 输出比较快速使能禁止(仅在 PWM1 和 PWM2 可设置)
    sConfigG.Pulse = g*2000/255;              // 在 PWM1 模式下,通道 3(RLED)占空比

    sConfigB.OCMode = TIM_OCMODE_PWM1;        // PWM 输出的两种模式:PWM1 当极性为低,CCR<CNT,输出低电平,反之高电平
    sConfigB.OCPolarity = TIM_OCPOLARITY_LOW; // 设置极性为低(硬件上低电平亮灯)
    sConfigB.OCFastMode = TIM_OCFAST_DISABLE; // 输出比较快速使能禁止(仅在 PWM1 和 PWM2 可设置)
    sConfigB.Pulse = b*2000/255;              // 在 PWM1 模式下,通道 3(RLED)占空比

    HAL_TIM_PWM_Stop(g_HPWM_ColorLED, CHANNEL_RED);
    HAL_TIM_PWM_Stop(g_HPWM_ColorLED, CHANNEL_GREEN);
    HAL_TIM_PWM_Stop(g_HPWM_ColorLED, CHANNEL_BLUE);
    
    HAL_TIM_PWM_ConfigChannel(g_HPWM_ColorLED, &sConfigR, CHANNEL_RED);
    HAL_TIM_PWM_ConfigChannel(g_HPWM_ColorLED, &sConfigG, CHANNEL_GREEN);
    HAL_TIM_PWM_ConfigChannel(g_HPWM_ColorLED, &sConfigB, CHANNEL_BLUE);

    HAL_TIM_PWM_Start(g_HPWM_ColorLED, CHANNEL_RED);
    HAL_TIM_PWM_Start(g_HPWM_ColorLED, CHANNEL_GREEN);
    HAL_TIM_PWM_Start(g_HPWM_ColorLED, CHANNEL_BLUE);    
}

/**********************************************************************
 * 函数名称： ColorLED_Test
 * 功能描述： 全彩LED测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 ***********************************************************************/
void ColorLED_Test( void *params )
{
    uint32_t color = 0;

    ColorLED_Init();

    while (1)
    {
        ColorLED_Set(color);

        color += 200000;
        color &= 0x00ffffff;
		vTaskDelay(1000);
    }    
}


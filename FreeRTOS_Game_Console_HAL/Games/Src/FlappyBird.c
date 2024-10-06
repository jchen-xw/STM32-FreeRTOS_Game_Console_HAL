/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 * 
 * 
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 *            佛祖保佑     永不宕机     永无BUG
 */

/*
 * @Author: Jchen
 * @Date: 2024-09-27 22:58:41
 * @LastEditors: Jchen
 * @LastEditTime: 2024-09-27 22:59:53
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Games\Src\FlappyBird.c
 */

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "homepage.h"
#include <stdlib.h>
#include "OLED.h"
#include "driver_Key.h"
#include "FlappyBird.h"

extern TaskHandle_t xHomePageTaskHandle;

static uint8_t FlappyBird_key = 0x00;
static uint8_t FlappyBird_Last_Key = 0x00;
static uint16_t Highest_Score = 0;
static uint16_t Score = 0;
static uint8_t Death_State = 0;
int16_t Up_Piping_X1 = 128;
uint8_t Up_Piping_Y1 = 0;
int16_t Up_Piping_X2 = 192;
uint8_t Up_Piping_Y2 = 0;
int8_t Bird_Y = 10;
int8_t Bird_Jump = 30;

/**
 * @description: 参数初始化
 * @return {*}
 * @Date: 2024-10-03 12:42:25
 * @Author: Jchen
 */
static void FlappyBird_Parameter_Reset( void )
{
    Score = 0;
    Death_State = 0;
    Up_Piping_X1 = 128;
    Up_Piping_Y1 = rand() % 36 + 1;
    Up_Piping_X2 = 192;
    Up_Piping_Y2 = rand() % 36 + 1;
    Bird_Y = 10;
    Bird_Jump = 30;
}

/**
 * @description: 欢迎界面
 * @return {*}
 * @Date: 2024-10-04 15:30:10
 * @Author: Jchen
 */
static void FplappyBird_Start_Screen( void )
{
    OLED_Clear();
	OLED_ShowImage( 0, 0, 128, 64, FLAPPYBIRD_COVER );
	OLED_Update();
}

/**
 * @description: 结束画面显示
 * @return {*}
 * @Date: 2024-10-03 12:42:35
 * @Author: Jchen
 */
static void FplappyBird_End_Screen( void )
{
    OLED_Clear();
    OLED_ShowImage(0, 0, 128, 32, FLAPPYBIRD_GAMEOVER);
    OLED_ShowString( 31, 32, "High Score:", OLED_6X8 );
    OLED_ShowNum( 52, 40, Highest_Score, 4, OLED_6X8 );
    OLED_ShowString( 34, 48, "Now Score:", OLED_6X8 );
    OLED_ShowNum( 52, 56, Score, 4, OLED_6X8 );
    OLED_Update();
}

/**
 * @description: 游戏画面显示
 * @return {*}
 * @Date: 2024-10-03 12:42:46
 * @Author: Jchen
 */
static void FplappyBird_GameScreen_Display( void )
{
    OLED_Clear();
    OLED_ShowString( 50, 0, "Hi:", OLED_6X8 );
    OLED_ShowNum(68, 0, Highest_Score, 4, OLED_6X8 );
    OLED_ShowNum(104, 0, Score, 4, OLED_6X8 );

    OLED_DrawRectangle_int16( Up_Piping_X1 - 0, 0, PIPING_WIDTH, Up_Piping_Y1, OLED_FILLED );
    OLED_DrawRectangle_int16( Up_Piping_X1 - 2, Up_Piping_Y1 - PIPING_MOUTH_HEIGHT, PIPING_MOUTH_WIDTH, PIPING_MOUTH_HEIGHT, OLED_FILLED );
    OLED_DrawRectangle_int16( Up_Piping_X1 - 0, Up_Piping_Y1 + PASS_HEIGHT, PIPING_WIDTH, 64 - Up_Piping_Y1, OLED_FILLED );
    OLED_DrawRectangle_int16( Up_Piping_X1 - 2, Up_Piping_Y1 + PASS_HEIGHT, PIPING_MOUTH_WIDTH, PIPING_MOUTH_HEIGHT, OLED_FILLED );
    OLED_DrawRectangle_int16( Up_Piping_X2 - 0, 0, PIPING_WIDTH, Up_Piping_Y2, OLED_FILLED );
    OLED_DrawRectangle_int16( Up_Piping_X2 - 2, Up_Piping_Y2 - PIPING_MOUTH_HEIGHT, PIPING_MOUTH_WIDTH, PIPING_MOUTH_HEIGHT, OLED_FILLED );
    OLED_DrawRectangle_int16( Up_Piping_X2 - 0, Up_Piping_Y2 + PASS_HEIGHT, PIPING_WIDTH, 64 - Up_Piping_Y2, OLED_FILLED );
    OLED_DrawRectangle_int16( Up_Piping_X2 - 2, Up_Piping_Y2 + PASS_HEIGHT, PIPING_MOUTH_WIDTH, PIPING_MOUTH_HEIGHT, OLED_FILLED );
    
    OLED_ShowImage( BIRD_DISPLAY_X, Bird_Y, 11, 8, FLAPPYBIRD_BIRD );
    OLED_Update();
}

/**
 * @description: 游戏主体
 * @param {void} *params
 * @return {*}
 * @Date: 2024-10-03 13:18:05
 * @Author: Jchen
 */
void FlappyBird_Task( void *params )
{
    vTaskSuspend( xHomePageTaskHandle );

    FlappyBird_Parameter_Reset();

    FplappyBird_Start_Screen();

    // 防止进入游戏连续读取按键A跳过封面显示
    FlappyBird_Last_Key = KEY_LEFT_A_NUM;
    FlappyBird_key = Get_KeyNum();

    while ( ( FlappyBird_key != KEY_LEFT_A_NUM ) || ( FlappyBird_Last_Key == FlappyBird_key ) )
    {
        FlappyBird_Last_Key = FlappyBird_key;
        FlappyBird_key = Get_KeyNum();

        
        if ( FlappyBird_key == KEY_DOWN_START_NUM )
        {
            vTaskResume( xHomePageTaskHandle );             // 恢复菜单任务线程
            vTaskSuspend( NULL );                           // 暂停游戏任务线程
            FlappyBird_key = KEY_LEFT_A_NUM;
            FplappyBird_Start_Screen();
        }
        else if ( FlappyBird_key == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xHomePageTaskHandle );             // 恢复菜单任务线程
            vTaskDelete( NULL );                            // 删除游戏任务线程
        }

        vTaskDelay( 50 );
    }
    

	while ( 1 )
	{
        
        // 结算画面
        if ( Death_State == 1 )
		{
            FplappyBird_End_Screen();
            
            FlappyBird_key = Get_KeyNum();
            if (FlappyBird_key == KEY_LEFT_A_NUM)
			{
				if( Score > Highest_Score )
                {
                    Highest_Score = Score;
                }

                FlappyBird_Parameter_Reset();

				OLED_Clear();
                OLED_Update();
			}
            else if ( FlappyBird_key == KEY_DOWN_START_NUM )
            {
                vTaskResume( xHomePageTaskHandle );         // 恢复菜单任务线程
                vTaskSuspend( NULL );                       // 暂停游戏任务线程
                FlappyBird_key = KEY_LEFT_A_NUM;
                FplappyBird_End_Screen();
            }
            else if ( FlappyBird_key == KEY_DOWN_SET_NUM )
            {
                vTaskResume( xHomePageTaskHandle );         // 恢复菜单任务线程
                vTaskDelete( NULL );                        // 删除游戏任务线程
            }
            vTaskDelay( 50 );
            continue;
		}

        OLED_Clear();
        
        FlappyBird_Last_Key = FlappyBird_key;
        FlappyBird_key = Get_KeyNum();
        if ( ( FlappyBird_key == KEY_LEFT_A_NUM ) && ( FlappyBird_Last_Key != FlappyBird_key ) )
        {
            Bird_Jump = 30;
        }

        Up_Piping_X1 = Up_Piping_X1 - 2;
        Up_Piping_X2 = Up_Piping_X2 - 2;
        Bird_Jump = Bird_Jump - 5;
        Bird_Y = Bird_Y - Bird_Jump / 10;

        FplappyBird_GameScreen_Display();

        if ( Bird_Y < 0 )
        {
            Bird_Y = 0;
        }
        if ( Bird_Y > 64 )
        {
            Death_State = 1;
        }

        if ( Up_Piping_X1 == -10 )
        {
            Up_Piping_X1 = 128;
            Up_Piping_Y1 = rand() % 42 + 1;
        }
        if ( Up_Piping_X2 == -10 )
        {
            Up_Piping_X2 = 128;
            Up_Piping_Y2 = rand() % 42 + 1;
        }
        /* 撞管子 */
        if ( ( ( Up_Piping_X1 >= BIRD_DISPLAY_X ) ) && ( Up_Piping_X1 <= ( BIRD_DISPLAY_X + 11 ) ) )
        {
            if ( ( Bird_Y < Up_Piping_Y1 ) || ( ( Bird_Y + 8 ) > ( Up_Piping_Y1 + PIPING_MOUTH_HEIGHT + PASS_HEIGHT ) ) )
            {
                Death_State = 1;
            }
        }
        if ( ( Up_Piping_X2 >= BIRD_DISPLAY_X ) && ( Up_Piping_X2 <= BIRD_DISPLAY_X + 11 ) )
        {
            if ( ( Bird_Y < Up_Piping_Y2 ) || ( Bird_Y + 8 >( Up_Piping_Y2 + PIPING_MOUTH_HEIGHT + PASS_HEIGHT ) ) )
            {
                Death_State = 1;
            }
        }

        if ( ( Up_Piping_X1 == 16 ) || ( Up_Piping_X2 == 16 ) )
        {
            Score ++;
        }

        vTaskDelay( 30 );
    
        // 嗯
        if ( FlappyBird_key == KEY_DOWN_START_NUM )
        {
            vTaskResume( xHomePageTaskHandle );                 // 恢复菜单任务线程
            vTaskSuspend( NULL );                               // 暂停游戏任务线程
            FlappyBird_key = KEY_LEFT_A_NUM;

        }
        else if ( FlappyBird_key == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xHomePageTaskHandle );                 // 恢复菜单任务线程
            vTaskDelete( NULL );                                // 删除游戏任务线程
        }
	}
}

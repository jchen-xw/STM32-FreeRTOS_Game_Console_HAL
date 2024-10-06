/*
 * @Author: Jchen
 * @Date: 2024-10-03 15:27:42
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-05 22:45:48
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Games\Src\GreedySnake.c
 */
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
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "homepage.h"
#include <stdlib.h>
#include "OLED.h"
#include "driver_Key.h"
#include "GreedySnake.h"

extern TaskHandle_t xHomePageTaskHandle;

static uint8_t GreedySnake_Last_Key = 0x00;
static uint8_t GreedySnake_Key = 0x00;

static uint8_t NewSnake_state = SNAKE_INIT_LENGTH;
static uint8_t Death_State = 0;
static uint16_t Highest_Score = 0;
static uint16_t Score = 0;

static uint8_t Snake_Length = SNAKE_INIT_LENGTH;
static uint8_t Snake_Point_X = SNAKE_INIT_POINT_X;
static uint8_t Snake_Point_Y = SNAKE_INIT_POINT_Y;

static uint8_t Food_Point_X = 86;
static uint8_t Food_Point_Y = 35;

/* 蛇全身坐标保存 */
SnakeBody_PointStructure SnakeBody_Point[ SNAKE_MAX_LENGTH ] = { 
    { SNAKE_INIT_POINT_X - 0, SNAKE_INIT_POINT_Y },
    { SNAKE_INIT_POINT_X - 2, SNAKE_INIT_POINT_Y },
    { SNAKE_INIT_POINT_X - 4, SNAKE_INIT_POINT_Y },
    { SNAKE_INIT_POINT_X - 6, SNAKE_INIT_POINT_Y },
    { SNAKE_INIT_POINT_X - 6, SNAKE_INIT_POINT_Y }
};

/**
 * @description: 参数初始化
 * @return {*}
 * @Date: 2024-10-03 19:51:12
 * @Author: Jchen
 */
void GreedySnake_Parameter_Reset( void )
{
    NewSnake_state = SNAKE_INIT_LENGTH;
    Death_State = 0;
    Score = 0;
    Snake_Length = SNAKE_INIT_LENGTH;
    Snake_Point_X = SNAKE_INIT_POINT_X;
    Snake_Point_Y = SNAKE_INIT_POINT_Y;
    Food_Point_X = 86;
    Food_Point_Y = 35;
}

/**
 * @description: 绘制新生小蛇蛇，并将初始地址记录到数组中；
 * @param {uint8_t} NewSnake_state
 * @return {*}
 * @Date: 2024-10-03 19:58:25
 * @Author: Jchen
 */
void NewSnake_Draw( void )
{
    uint8_t i;

    for ( i = 0; i < ( 5 - NewSnake_state ); i++ )
    {
        OLED_DrawPoint( Snake_Point_X - ( NewSnake_state - 1 + i ) * 2 - 0, Snake_Point_Y + 0 );
        OLED_DrawPoint( Snake_Point_X - ( NewSnake_state - 1 + i ) * 2 - 0, Snake_Point_Y + 1 );
        OLED_DrawPoint( Snake_Point_X - ( NewSnake_state - 1 + i ) * 2 - 1, Snake_Point_Y + 0 );
        OLED_DrawPoint( Snake_Point_X - ( NewSnake_state - 1 + i ) * 2 - 1, Snake_Point_Y + 1 );
    }
    NewSnake_state --;
}

/**
 * @description: 蛇身标志位存储
 * @return {*}
 * @Date: 2024-10-04 14:20:11
 * @Author: Jchen
 */
void SnakeBody_Store_Point( void )
{
	uint8_t i;
	for ( i = Snake_Length; i > 0; i-- )
	{
		SnakeBody_Point[ i ].x = SnakeBody_Point[ i - 1 ].x;
		SnakeBody_Point[ i ].y = SnakeBody_Point[ i - 1 ].y;
	}
	
	if ( Snake_Point_X > 126 )
	{
		Snake_Point_X = 2;
	}
    else if ( Snake_Point_X < 2 )
    {
		Snake_Point_X = 126;
	}
	if ( Snake_Point_Y > 61 )
	{
		Snake_Point_Y = 11;
	}
	else if ( Snake_Point_Y < 11 )
	{
		Snake_Point_Y = 61;
	}
	SnakeBody_Point[ 0 ].x = Snake_Point_X;
	SnakeBody_Point[ 0 ].y = Snake_Point_Y;
}

/**
 * @description: 蛇自动移动
 * @return {*}
 * @Date: 2024-10-04 14:10:44
 * @Author: Jchen
 */
void Snake_AutoMove( void )
{
    if ( ( SnakeBody_Point[0].x == SnakeBody_Point[1].x + 2 ) || ( ( SnakeBody_Point[0].x == 2 ) && ( SnakeBody_Point[1].x == 126 ) ) )
    {
        Snake_Point_X += 2;
    }
	else if ( ( SnakeBody_Point[0].x == SnakeBody_Point[1].x - 2 )  || ( ( SnakeBody_Point[0].x == 126 ) && ( SnakeBody_Point[1].x == 2 ) ) )
	{
		Snake_Point_X -= 2;
	}
    else if ( ( SnakeBody_Point[0].y == SnakeBody_Point[1].y + 2 )  || ( ( SnakeBody_Point[0].y == 11 ) && ( SnakeBody_Point[1].y == 61 ) ) )
	{
		Snake_Point_Y += 2;
	}
    else if ( ( SnakeBody_Point[0].y == SnakeBody_Point[1].y - 2 )  || ( ( SnakeBody_Point[0].y == 61 ) && ( SnakeBody_Point[1].y == 11 ) ) )
	{
		Snake_Point_Y -= 2;
	}
}

/**
 * @description: 蛇 按键转弯
 * @return {*}
 * @Date: 2024-10-04 14:10:59
 * @Author: Jchen
 */
void Snake_PassiveMove( void )
{
    if ( ( GreedySnake_Key == KEY_RIGHT_UP_NUM ) && ( ( SnakeBody_Point[0].y != SnakeBody_Point[1].y + 2 ) ) )
    {
        Snake_Point_Y -= 2;
    }
	else if ( ( GreedySnake_Key == KEY_RIGHT_DOWN_NUM )  && ( ( SnakeBody_Point[0].y != SnakeBody_Point[1].y - 2 ) ) )
	{
		Snake_Point_Y += 2;
	}
    else if ( ( GreedySnake_Key == KEY_RIGHT_RIGHT_NUM )  && ( ( SnakeBody_Point[0].x != SnakeBody_Point[1].x - 2 ) ) )
	{
		Snake_Point_X += 2;
	}
    else if ( ( GreedySnake_Key == KEY_RIGHT_LEFT_NUM )  && ( ( SnakeBody_Point[0].x != SnakeBody_Point[1].x + 2 ) ) )
	{
		Snake_Point_X -= 2;
	}
    else
    {
        Snake_AutoMove();
    }
}

/**
 * @description: 绘制蛇蛇
 * @return {*}
 * @Date: 2024-10-04 15:10:22
 * @Author: Jchen
 */
void Snake_Draw( void )
{
    uint8_t i;
    for ( i = 0; i < Snake_Length; i++ )
    {
        OLED_DrawPoint( SnakeBody_Point[ i ].x - 0, SnakeBody_Point[ i ].y + 0 );
        OLED_DrawPoint( SnakeBody_Point[ i ].x - 0, SnakeBody_Point[ i ].y + 1 );
        OLED_DrawPoint( SnakeBody_Point[ i ].x - 1, SnakeBody_Point[ i ].y + 0 );
        OLED_DrawPoint( SnakeBody_Point[ i ].x - 1, SnakeBody_Point[ i ].y + 1 );
    }
}

/**
 * @description: 食物绘制
 * @return {*}
 * @Date: 2024-10-04 14:19:44
 * @Author: Jchen
 */
void Food_Draw( void )
{
    OLED_DrawPoint( Food_Point_X - 0, Food_Point_Y + 0 );
    OLED_DrawPoint( Food_Point_X - 0, Food_Point_Y + 1 );
    OLED_DrawPoint( Food_Point_X - 1, Food_Point_Y + 0 );
    OLED_DrawPoint( Food_Point_X - 1, Food_Point_Y + 1 );
}

/**
 * @description: 判断蛇是否吃上实物
 * @return {*}
 * @Date: 2024-10-04 14:42:23
 * @Author: Jchen
 */
void Judging_Snake_Eat( void )
{
    if ( ( Snake_Point_X == Food_Point_X ) && ( Snake_Point_Y == Food_Point_Y ) )
    {
        Snake_Length ++;
        Score ++;
        Food_Point_X = ( rand() % 63 ) * 2 + 2;
        Food_Point_Y = ( rand() % 25 ) * 2 + 11;
    }
}

/**
 * @description: 是否撞上自己
 * @return {*}
 * @Date: 2024-10-04 15:28:59
 * @Author: Jchen
 */
void isGameOver( void )
{
    uint8_t i;
    for ( i = 3; i < Snake_Length; i++ )
    {
        if ( Snake_Point_X == SnakeBody_Point[ i ].x )
        {
            if ( Snake_Point_Y == SnakeBody_Point[ i ].y )
            {
                Death_State = 1;
            }
        } 
    }
}

/**
 * @description: 游戏欢迎页面
 * @return {*}
 * @Date: 2024-10-04 15:48:42
 * @Author: Jchen
 */
void GreedySnake_Start_Screen( void )
{
    OLED_Clear();
    OLED_ShowImage( 0, 0, 68, 64, GREEDYSNAKE_COVER );
	OLED_ShowString( 68, 10, "Greedy", OLED_8X16 );
	OLED_ShowString( 88, 26, "Snake", OLED_8X16 );
	OLED_ShowImage( 109, 57, 19, 5, ZSJTF );
	OLED_ShowString( 104, 42, "v1.0", OLED_6X8 );
    OLED_Update();
}

/**
 * @description: 结束画面
 * @return {*}
 * @Date: 2024-10-04 15:30:53
 * @Author: Jchen
 */
 void GreedySnake_End_Screen( void )
{
    OLED_Clear();
	OLED_ShowImage( 0, 0, 64, 57, GREEDYSNAKE_GAMEOVER );
    OLED_ShowString( 62, 12, "High Score:", OLED_6X8 );
    OLED_ShowNum( 104, 20, Highest_Score, 4, OLED_6X8 );
    OLED_ShowString( 62, 33, "Score:", OLED_6X8 );
    OLED_ShowNum( 104, 41, Score, 4, OLED_6X8 );
	OLED_Update();
}

/**
 * @description: 游戏主体
 * @param {void} *params
 * @return {*}
 * @Date: 2024-10-03 23:14:20
 * @Author: Jchen
 */
void GreedySnake_Task( void *params )
{
    vTaskSuspend( xHomePageTaskHandle );
    GreedySnake_Parameter_Reset();

    /* 欢迎页面 */
    GreedySnake_Start_Screen();

#if 1  // 防止进入游戏连续读取按键A跳过封面显示
    GreedySnake_Last_Key = KEY_LEFT_A_NUM;
    GreedySnake_Key = Get_KeyNum();

    while ( ( GreedySnake_Key != KEY_LEFT_A_NUM ) || ( GreedySnake_Last_Key == GreedySnake_Key ) )
    {
        GreedySnake_Last_Key = GreedySnake_Key;
        GreedySnake_Key = Get_KeyNum();
        
        if ( GreedySnake_Key == KEY_DOWN_START_NUM )
        {
            vTaskResume( xHomePageTaskHandle );             // 恢复菜单任务线程
            vTaskSuspend( NULL );                           // 暂停游戏任务线程
            GreedySnake_Key = KEY_LEFT_A_NUM;
            GreedySnake_Start_Screen();
        }
        else if ( GreedySnake_Key == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xHomePageTaskHandle );             // 恢复菜单任务线程
            vTaskDelete( NULL );                            // 删除游戏任务线程
        }
        vTaskDelay( 50 );
    }
#endif

    while ( 1 )
    {
        OLED_Clear();

        // 结算画面
        if ( Death_State == 1 )
		{
            GreedySnake_End_Screen();
            
            GreedySnake_Key = Get_KeyNum();
            if (GreedySnake_Key == KEY_LEFT_A_NUM)
			{
				if( Score > Highest_Score )
                {
                    Highest_Score = Score;
                }

                GreedySnake_Parameter_Reset();

				OLED_Clear();
                OLED_Update();
			}
            else if ( GreedySnake_Key == KEY_DOWN_START_NUM )
            {
                vTaskResume( xHomePageTaskHandle );         // 恢复菜单任务线程
                vTaskSuspend( NULL );                       // 暂停游戏任务线程
                GreedySnake_Key = KEY_LEFT_A_NUM;
                // FplappyBird_End_Screen();
            }
            else if ( GreedySnake_Key == KEY_DOWN_SET_NUM )
            {
                vTaskResume( xHomePageTaskHandle );         // 恢复菜单任务线程
                vTaskDelete( NULL );                        // 删除游戏任务线程
            }
            vTaskDelay( 50 );
            continue;
		}

        OLED_DrawRectangle( MAP_DRAW_X, MAP_DRAW_Y, MAP_LENGTH, MAP_HEIGTH, OLED_UNFILLED );

        // 初生小蛇蛇
        if ( NewSnake_state > 0 )
        {
            NewSnake_Draw();
        }
        else
        {
            GreedySnake_Last_Key = GreedySnake_Key;
            GreedySnake_Key = Get_KeyNum();

            if ( ( GreedySnake_Key != GreedySnake_Last_Key ) && 
                ( ( GreedySnake_Key == KEY_RIGHT_UP_NUM ) || ( GreedySnake_Key == KEY_RIGHT_DOWN_NUM ) ||
                ( GreedySnake_Key == KEY_RIGHT_RIGHT_NUM ) || ( GreedySnake_Key == KEY_RIGHT_LEFT_NUM ) ) )
            {
                Snake_PassiveMove();
            }
            else
            {
                Snake_AutoMove();
            }
            Judging_Snake_Eat();

            SnakeBody_Store_Point();

            Snake_Draw();
            Food_Draw();
        }

        OLED_ShowString(35, 0, "HI:", OLED_6X8);
		OLED_ShowNum(58, 0, Highest_Score, 4, OLED_6X8);
		OLED_ShowNum(98, 0, Score, 4, OLED_6X8);
        
        isGameOver();
        
        OLED_Update();

        vTaskDelay( 150 );
#if 1   // 嗯
        if ( GreedySnake_Key == KEY_DOWN_START_NUM )
        {
            vTaskResume( xHomePageTaskHandle );             // 恢复菜单任务线程
            vTaskSuspend( NULL );                           // 暂停游戏任务线程
            GreedySnake_Key = KEY_LEFT_A_NUM;
        }
        else if ( GreedySnake_Key == KEY_DOWN_SET_NUM )
        {
            vTaskResume( xHomePageTaskHandle );             // 恢复菜单任务线程
            vTaskDelete( NULL );                            // 删除游戏任务线程
        }
#endif
    }
}



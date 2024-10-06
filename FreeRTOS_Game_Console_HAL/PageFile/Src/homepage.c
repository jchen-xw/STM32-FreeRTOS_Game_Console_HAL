/*
 * @Author: Jchen
 * @Date: 2024-09-26 15:13:44
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-06 22:58:52
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\PageFile\Src\homepage.c
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
#include "cmsis_os.h"
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core

#include "driver_Key.h"
#include "driver_passive_buzzer.h"
#include "OLED.h"

#include "DinoGame.h"
#include "StarWar.h"
#include "FlappyBird.h"
#include "GreedySnake.h"
#include "music.h"
#include "homepage.h"
#include "InfoPage.h"

static TaskHandle_t xInfoPage_Handle;

extern TaskHandle_t xDinoGame_Handle;
extern TaskHandle_t xStarWar_Handler;
extern TaskHandle_t xFappyBird_Handler;
extern TaskHandle_t xGreedySnake_Handler;
extern TaskHandle_t xMusicTaskHandle;

extern TaskStatus_t xDinoGame_State;
extern TaskStatus_t xStarWar_State;
extern TaskStatus_t xFappyBird_State;
extern TaskStatus_t xGreedySnake_State;
extern TaskStatus_t xMusic_State;

static uint8_t Point = 0;
static uint8_t Last_Point = 0;

static uint8_t Home_Key_Num = POINT_MOVE_NONE;
static uint8_t Home_Key_Last_Num = POINT_MOVE_NONE;

void HomePage_Init( void )
{
    OLED_Init();
}

/**
 * @description: 默认显示
 * @return {*}
 * @Date: 2024-10-04 18:19:45
 * @Author: Jchen
 */
void Default_Information( void )
{
    /* 游戏默认欢迎语句显示 */
    OLED_ShowString( DEFAULT_TEXT_X, GAME_TEXT_POSITION_Y, DEFAULT_DISPLAY, OLED_8X16 );

    /* 游戏图标显示 */
    OLED_ShowImage( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 0 ), GAME1_ICON_POSITION_Y, GAME_ICON_LENGTH, GAME_ICON_LENGTH, GAME1_ICON );
    OLED_ShowImage( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 1 ), GAME1_ICON_POSITION_Y, GAME_ICON_LENGTH, GAME_ICON_LENGTH, GAME2_ICON );
    OLED_ShowImage( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 2 ), GAME1_ICON_POSITION_Y, GAME_ICON_LENGTH, GAME_ICON_LENGTH, GAME3_ICON );
    OLED_ShowImage( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 3 ), GAME1_ICON_POSITION_Y, GAME_ICON_LENGTH, GAME_ICON_LENGTH, GAME4_ICON );

    /* 图标框显示 */
    OLED_DrawRectangle( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 0 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH, OLED_UNFILLED );
    OLED_DrawRectangle( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 1 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH, OLED_UNFILLED );
    OLED_DrawRectangle( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 2 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH, OLED_UNFILLED );
    OLED_DrawRectangle( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 3 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH, OLED_UNFILLED );

    /* 底部方框内容显示 */
    OLED_ShowString( RETANGLE_TEST_POSITION_X, RETANGLE_TEST_POSITION_Y, RETANGLE_TEST_DISPLAY, OLED_6X8 );

    /* 底部方框显示 */
    OLED_DrawRectangle( RETANGLE_POSITION_X, RETANGLE_POSITION_Y, RETANGLE_X, RETANGLE_Y, OLED_UNFILLED );

    /* 全屏方框显示 */
    OLED_DrawRectangle( 0, 0, 128, 64, OLED_UNFILLED );
}

/**
 * @description: 游戏运行状态显示
 * @return {*}
 * @Date: 2024-10-06 13:44:51
 * @Author: Jchen
 */
void GameRunState( void )
{
    /* 获取任务状态 */
    vTaskGetInfo( xDinoGame_Handle, &xDinoGame_State, pdFALSE, eInvalid );
    vTaskGetInfo( xStarWar_Handler, &xStarWar_State, pdFALSE, eInvalid );
    vTaskGetInfo( xFappyBird_Handler, &xFappyBird_State, pdFALSE, eInvalid );
    vTaskGetInfo( xGreedySnake_Handler, &xGreedySnake_State, pdFALSE, eInvalid );
    vTaskGetInfo( xMusicTaskHandle, &xMusic_State, pdFALSE, eInvalid );

    /* 游戏运行状态显示 */
    if ( ( xDinoGame_State.eCurrentState == eSuspended ) || ( xDinoGame_State.eCurrentState == eBlocked ) )
    {
        OLED_DrawRectangle( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 0 ) - 3, GAME1_ICON_POSITION_Y - 3, GAME_ICON_LENGTH + 6, GAME_ICON_LENGTH + 6, OLED_UNFILLED );

        if (Point == 1)
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }
    if ( ( xStarWar_State.eCurrentState == eSuspended ) || ( xStarWar_State.eCurrentState == eBlocked ) )
    {
        OLED_DrawRectangle( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 1 ) - 3, GAME1_ICON_POSITION_Y - 3, GAME_ICON_LENGTH + 6, GAME_ICON_LENGTH + 6, OLED_UNFILLED );

        if (Point == 2)
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }
    if ( ( xFappyBird_State.eCurrentState == eSuspended ) || ( xFappyBird_State.eCurrentState == eBlocked ) )
    {
        OLED_DrawRectangle( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 2 ) - 3, GAME1_ICON_POSITION_Y - 3, GAME_ICON_LENGTH + 6, GAME_ICON_LENGTH + 6, OLED_UNFILLED );

        if (Point == 3)
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }
    if ( ( xGreedySnake_State.eCurrentState == eSuspended ) || ( xGreedySnake_State.eCurrentState == eBlocked ) )
    {
        OLED_DrawRectangle( ( GAME1_ICON_POSITION_X + GAME_ICON_DISTANCE * 3 ) - 3, GAME1_ICON_POSITION_Y - 3, GAME_ICON_LENGTH + 6, GAME_ICON_LENGTH + 6, OLED_UNFILLED );

        if (Point == 4)
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }
    if ( xMusic_State.eCurrentState == eBlocked )
    {
        OLED_DrawCircle( RUNNING_STATE_CIRCLE_X, RUNNING_STATE_CIRCLE_Y, RUNNING_STATE_CIRCLE_RADIUS, OLED_FILLED );

        if (Point == 5)
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }
    if ( xMusic_State.eCurrentState == eSuspended )
    {
        OLED_DrawCircle( RUNNING_STATE_CIRCLE_X, RUNNING_STATE_CIRCLE_Y, RUNNING_STATE_CIRCLE_RADIUS, OLED_UNFILLED );

        if (Point == 5)
        {
            OLED_ShowString( RUNNING_STATE_DISPLAY_X, RUNNING_STATE_DISPLAY_Y, "run", OLED_6X8 );
        }
    }
}

/**
 * @description: 选定任务显示
 * @param {uint8_t} Point
 * @return {*}
 * @Date: 2024-10-06 13:47:07
 * @Author: Jchen
 */
void SelectTask( uint8_t Point )
{
    if ( Point == 1 )
    {
        OLED_ShowString( GAME_TEXT_POSITION_X, GAME_TEXT_POSITION_Y, GAME1_NAME, OLED_8X16 );
        OLED_ReverseArea( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 0 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH );
    }
    else if ( Point == 2 )
    {
        OLED_ShowString( GAME_TEXT_POSITION_X, GAME_TEXT_POSITION_Y, GAME2_NAME, OLED_8X16 );
        OLED_ReverseArea( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 1 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH );
    }
    else if ( Point == 3 )
    {
        OLED_ShowString( GAME_TEXT_POSITION_X, GAME_TEXT_POSITION_Y, GAME3_NAME, OLED_8X16 );
        OLED_ReverseArea( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 2 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH );
    }
    else if ( Point == 4 )
    {
        OLED_ShowString( GAME_TEXT_POSITION_X, GAME_TEXT_POSITION_Y, GAME4_NAME, OLED_8X16 );
        OLED_ReverseArea( ( SQUARE_FIRST_POSITION_X + SQUARE_DISTANCE * 3 ), SQUARE_FIRST_POSITION_Y, SQUARE_LENGTH, SQUARE_LENGTH );
    }
    else if ( Point == 5 )
    {
        OLED_ShowString( GAME_TEXT_POSITION_X, GAME_TEXT_POSITION_Y, MUSIC_NAME, OLED_8X16 );
        OLED_ReverseArea( RETANGLE_POSITION_X, RETANGLE_POSITION_Y, RETANGLE_X, RETANGLE_Y );
    }
}

/**
 * @description: 选择开启游戏任务
 * @return {*}
 * @Date: 2024-10-06 13:57:01
 * @Author: Jchen
 */
void OpenTask( void )
{
    if ( Home_Key_Num == KEY_LEFT_A_NUM )
    {   
        if ( Point == 1 )
        {
            vTaskGetInfo( xDinoGame_Handle, &xDinoGame_State, pdFALSE, eInvalid );
            
            if ( xDinoGame_State.eCurrentState == eSuspended )
            {
                OLED_Clear();
                OLED_Update();
                vTaskResume( xDinoGame_Handle );
                vTaskSuspend( NULL );
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
            else
            {
                xTaskCreate( DinoGame_Task, "DinoGameTask", DINOGAME_STK_SIZE, NULL, osPriorityNormal, &xDinoGame_Handle); 
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
            
        }
        else if ( Point == 2 )
        {
            vTaskGetInfo( xStarWar_Handler, &xStarWar_State, pdFALSE, eInvalid );

            if ( xStarWar_State.eCurrentState == eSuspended )
            {
                OLED_Clear();
                OLED_Update();
                vTaskResume( xStarWar_Handler );
                vTaskSuspend( NULL );
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
            else
            {												 
                xTaskCreate( StarWar_Task, "StarWarTask", STARWAR_STK_SIZE, NULL, osPriorityNormal, &xStarWar_Handler); 
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
        }
        else if ( Point == 3 )
        {
            vTaskGetInfo( xFappyBird_Handler, &xFappyBird_State, pdFALSE, eInvalid );

            if ( xFappyBird_State.eCurrentState == eSuspended )
            {
                OLED_Clear();
                OLED_Update();
                vTaskResume( xFappyBird_Handler );
                vTaskSuspend( NULL );
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
            else
            {												 
                xTaskCreate( FlappyBird_Task, "FlappyBirdTask", FLAPPYBIRD_STK_SIZE, NULL, osPriorityNormal, &xFappyBird_Handler); 
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
        }
        else if ( Point == 4 )
        {
            vTaskGetInfo( xGreedySnake_Handler, &xGreedySnake_State, pdFALSE, eInvalid );

            if ( xGreedySnake_State.eCurrentState == eSuspended )
            {
                OLED_Clear();
                OLED_Update();
                vTaskResume( xGreedySnake_Handler );
                vTaskSuspend( NULL );
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
            else
            {												 
                xTaskCreate( GreedySnake_Task, "GreedySnakeTask", GreedySnake_STK_SIZE, NULL, osPriorityNormal, &xGreedySnake_Handler); 
                Home_Key_Last_Num = KEY_DOWN_START_NUM;
                Home_Key_Num = KEY_DOWN_START_NUM;
            }
        }
        else if ( Point == 5 )
        {
            vTaskGetInfo( xMusicTaskHandle, &xMusic_State, pdFALSE, eInvalid );

            if ( xMusic_State.eCurrentState == eSuspended )
            {
                vTaskResume( xMusicTaskHandle );
            }
            else if ( xMusic_State.eCurrentState == eBlocked )
            {
                
            }
            else 
            {												 
                xTaskCreate( PlayMusic, "PlayMusicTask", MUSIC_STK_SIZE, NULL, osPriorityNormal, &xMusicTaskHandle ); 
            }
        }
    }
    if ( Home_Key_Num == KEY_LEFT_B_NUM )
    {
        if ( Point == 5 )
        {
            vTaskGetInfo( xMusicTaskHandle, &xMusic_State, pdFALSE, eInvalid );

            if ( xMusic_State.eCurrentState == eBlocked )
            {												 
                vTaskSuspend( xMusicTaskHandle );
                PassiveBuzzer_Control(0);
            }
        }
    }
}

/**
 * @description: 关闭游戏
 * @return {*}
 * @Date: 2024-10-06 13:59:16
 * @Author: Jchen
 */
void CloseTask( void )
{
    if ( Home_Key_Num == KEY_DOWN_SET_NUM )
    {   
        if ( Point == 1 )
        {
            vTaskGetInfo( xDinoGame_Handle, &xDinoGame_State, pdFALSE, eInvalid );
            
            if ( ( xDinoGame_State.eCurrentState == eSuspended ) || ( xDinoGame_State.eCurrentState == eBlocked ) )
            {
                vTaskDelete( xDinoGame_Handle );
                xDinoGame_Handle = NULL;
            }
        }
        else if ( Point == 2 )
        {
            vTaskGetInfo( xStarWar_Handler, &xStarWar_State, pdFALSE, eInvalid );

            if ( ( xStarWar_State.eCurrentState == eSuspended ) || ( xStarWar_State.eCurrentState == eBlocked ) )
            {
                vTaskDelete( xStarWar_Handler );
                xStarWar_Handler = NULL;
            }
        }
        else if ( Point == 3 )
        {
            vTaskGetInfo( xFappyBird_Handler, &xFappyBird_State, pdFALSE, eInvalid );

            if ( ( xFappyBird_State.eCurrentState == eSuspended ) || ( xFappyBird_State.eCurrentState == eBlocked ) )
            {
                vTaskDelete( xFappyBird_Handler );
                xFappyBird_Handler = NULL;
            }
        }
        else if ( Point == 4 )
        {
            vTaskGetInfo( xGreedySnake_Handler, &xGreedySnake_State, pdFALSE, eInvalid );

            if ( ( xGreedySnake_State.eCurrentState == eSuspended) || ( xGreedySnake_State.eCurrentState == eBlocked ) )
            {
                vTaskDelete( xGreedySnake_Handler );
                xGreedySnake_Handler = NULL;
            }
        }
        else if ( Point == 5 )
        {
            vTaskGetInfo( xMusicTaskHandle, &xMusic_State, pdFALSE, eInvalid );

            if ( ( xMusic_State.eCurrentState == eSuspended)  || ( xMusic_State.eCurrentState == eBlocked ) )
            {
                vTaskDelete( xMusicTaskHandle );
                xMusicTaskHandle = NULL;
                PassiveBuzzer_Control( 0 );
            }
        }
    }
}

/**
 * @description: 选择展示任务信息
 * @return {*}
 * @Date: 2024-10-06 14:01:40
 * @Author: Jchen
 */
void SelectTaskDisplayInfo( void )
{
    if ( Home_Key_Last_Num != Home_Key_Num )
    {
        /* 显示游戏信息 */
        if ( Home_Key_Num == KEY_DOWN_START_NUM )
        {   
            if ( Point == 1 )
            {
                xTaskCreate( Info_Task, "InfoTask", INFO_STK_SIZE, (void *)&Point, osPriorityNormal, &xInfoPage_Handle );
                vTaskSuspend( NULL );
                xInfoPage_Handle = NULL;
                Home_Key_Num = KEY_LEFT_B_NUM;
            }
            else if ( Point == 2 )
            {
                xTaskCreate( Info_Task, "InfoTask", INFO_STK_SIZE, (void *)&Point, osPriorityNormal, &xInfoPage_Handle );
                vTaskSuspend( NULL );
                xInfoPage_Handle = NULL;
                Home_Key_Num = KEY_LEFT_B_NUM;
            }
            else if ( Point == 3 )
            {
                xTaskCreate( Info_Task, "InfoTask", INFO_STK_SIZE, (void *)&Point, osPriorityNormal, &xInfoPage_Handle );
                vTaskSuspend( NULL );
                xInfoPage_Handle = NULL;
                Home_Key_Num = KEY_LEFT_B_NUM;
            }
            else if ( Point == 4 )
            {
                xTaskCreate( Info_Task, "InfoTask", INFO_STK_SIZE, (void *)&Point, osPriorityNormal, &xInfoPage_Handle );
                vTaskSuspend( NULL );
                xInfoPage_Handle = NULL;
                Home_Key_Num = KEY_LEFT_B_NUM;
            }
            else if ( Point == 5 )
            {
                xTaskCreate( Info_Task, "InfoTask", INFO_STK_SIZE, (void *)&Point, osPriorityNormal, &xInfoPage_Handle );
                vTaskSuspend( NULL );
                xInfoPage_Handle = NULL;
                Home_Key_Num = KEY_LEFT_B_NUM;
            }
        }
    }
}

/**
 * @description: 光标位置显示
 * @return {*}
 * @Date: 2024-10-06 14:03:47
 * @Author: Jchen
 */
void PointPositionDisplay( void )
{
    if ( ( Home_Key_Num == KEY_RIGHT_RIGHT_NUM ) && ( Point < 4 ) )
    {
        Last_Point = Point;
        Point ++;
    }
    else if ( ( Home_Key_Num == KEY_RIGHT_LEFT_NUM ) && ( Point > 1 ) )
    {
        Last_Point = Point;
        Point --;
    }
    else if ( ( Home_Key_Num == KEY_RIGHT_RIGHT_NUM ) && ( Point == 4 ) )
    {
        Last_Point = Point;
        Point = 1;
    }
    else if ( ( Home_Key_Num == KEY_RIGHT_LEFT_NUM ) && ( Point <= 1 ) )
    {
        Last_Point = 1;
        Point = 4;
    }
    else if ( ( Home_Key_Num == KEY_RIGHT_RIGHT_NUM ) && ( Point == 4 ) )
    {
        Last_Point = 4;
        Point = 1;
    }
    else if ( ( Home_Key_Num == KEY_RIGHT_DOWN_NUM ) && ( Point < 5 ) )
    {
        Last_Point = Point;
        Point = 5;
    }
    else if ( ( Home_Key_Num == KEY_RIGHT_UP_NUM ) && ( Point == 5 ) )
    {
        Point = Last_Point;
        Last_Point = 5;
    }
    if ( Point > 5 )
    {
        Point = 5;
    }
}

/**
 * @description: 主页程序本体
 * @param {void} *params
 * @return {*}
 * @Date: 2024-10-06 13:56:41
 * @Author: Jchen
 */
void HomePage_Task( void *params )
{
    while ( 1 )
    {
        OLED_Clear();

        /* HomePage默认显示内容 */
        Default_Information();

        /* 游戏运行状态显示 */
        GameRunState();

        /* 选定任务显示 */
        SelectTask( Point );
        
        OLED_Update();

        Home_Key_Last_Num = Home_Key_Num;
        Home_Key_Num = Get_KeyNum();

        /* 是否重复按键判断 */
        if ( Home_Key_Last_Num != Home_Key_Num )
        {
            if ( ( Home_Key_Num != POINT_MOVE_NONE ) && ( Home_Key_Num != Home_Key_Last_Num ) )
            {
                /* 选择开启游戏任务 */
                OpenTask();

                /* 选择关闭游戏 */
                CloseTask();

                /* 选择展示任务信息 */
                SelectTaskDisplayInfo();

                /* 光标位置显示 */
                PointPositionDisplay();
            }
        }
        vTaskDelay(50);
    }
}


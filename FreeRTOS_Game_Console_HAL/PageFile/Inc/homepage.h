/*
 * @Author: Jchen
 * @Date: 2024-09-26 15:13:38
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-06 22:57:08
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\PageFile\Inc\homepage.h
 */
#ifndef __HOMEPAGE_H_
#define __HOMEPAGE_H_

#define DEFAULT_TEXT_X                      32

#define GAME_TEXT_POSITION_X                8
#define GAME_TEXT_POSITION_Y                2
#define DEFAULT_DISPLAY                     "Welcome!"
#define GAME1_NAME                          "Dino Game  "
#define GAME2_NAME                          "Star War   "
#define GAME3_NAME                          "Bird!      "
#define GAME4_NAME                          "GreedySnake"
#define MUSIC_NAME                          "Music      "

#define RUNNING_STATE_DISPLAY_X             108
#define RUNNING_STATE_DISPLAY_Y             1

#define GAME1_ICON                          Dino_Game
#define GAME2_ICON                          Star_War
#define GAME3_ICON                          FlayyyBird
#define GAME4_ICON                          GreedySnake

#define GAME1_ICON_POSITION_X               10
#define GAME1_ICON_POSITION_Y               21
#define GAME_ICON_DISTANCE                  30
#define GAME_ICON_LENGTH                    18

#define SQUARE_FIRST_POSITION_X             9    
#define SQUARE_FIRST_POSITION_Y             20 
#define SQUARE_DISTANCE                     30
#define SQUARE_LENGTH                       20

#define RETANGLE_TEST_POSITION_X            16
#define RETANGLE_TEST_POSITION_Y            50
#define RETANGLE_TEST_DISPLAY               "JTFs GameConsole"

#define RETANGLE_POSITION_X                 14
#define RETANGLE_POSITION_Y                 44
#define RETANGLE_X                          100
#define RETANGLE_Y                          18

#define RUNNING_STATE_CIRCLE_X              108
#define RUNNING_STATE_CIRCLE_Y              48
#define RUNNING_STATE_CIRCLE_RADIUS         2

#define DINOGAME_STK_SIZE                   60
#define STARWAR_STK_SIZE                    60
#define FLAPPYBIRD_STK_SIZE                 50
#define GreedySnake_STK_SIZE                48
#define MUSIC_STK_SIZE                      50
#define INFO_STK_SIZE                       50

#define POINT_MOVE_NONE	                    0x00


void HomePage_Init( void );
void HomePage_Task( void *params );

#endif

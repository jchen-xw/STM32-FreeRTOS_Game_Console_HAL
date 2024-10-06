/*
 * @Author: Jchen
 * @Date: 2024-10-03 15:27:18
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-04 16:41:22
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Games\Inc\GreedySnake.h
 */
#ifndef __GREEDYSNAKE_H_
#define __GREEDYSNAKE_H_

#define SNAKE_INIT_LENGTH               4
#define SNAKE_MAX_LENGTH                60
#define SNAKE_INIT_POINT_X              34
#define SNAKE_INIT_POINT_Y              35

#define MAP_DRAW_X                      0
#define MAP_DRAW_Y                      10
#define MAP_LENGTH                      128
#define MAP_HEIGTH                      54

typedef struct SnakeBody_PointStructure
{
    uint8_t x;
    uint8_t y;
}SnakeBody_PointStructure;



void GreedySnake_Task( void *params );


#endif /* __GREEDYSNAKE_H_ */

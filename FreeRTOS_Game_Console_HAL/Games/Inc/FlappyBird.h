/*
 * @Author: Jchen
 * @Date: 2024-09-27 22:58:44
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-03 14:54:04
 * @Description: 
 * @FilePath: \FreeRTOS_Game_Console_HAL\Games\Inc\FlappyBird.h
 */
#ifndef __FLAPPYBIRD_H_
#define __FLAPPYBIRD_H_

void FlappyBird_Task( void *params );

#define BIRD_DISPLAY_X              20
#define PIPING_WIDTH                10
#define PIPING_MOUTH_WIDTH          14
#define PIPING_MOUTH_HEIGHT         6
#define PASS_HEIGHT                 30


#endif

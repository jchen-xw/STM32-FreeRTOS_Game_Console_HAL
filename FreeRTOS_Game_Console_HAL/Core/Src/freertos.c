/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "semphr.h"                     // ARM.FreeRTOS::RTOS:Core
#include "driver_led.h"
#include "driver_Key.h"
#include "driver_timer.h"
#include "OLED.h"
#include "driver_color_led.h"
#include "music.h"
#include "homepage.h"
#include "GreedySnake.h"
#include "driver_uart.h"
#include "usart.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static StackType_t g_pucStackOfLightTask[ 30 ];
static StaticTask_t g_TCBofLightTask;

static StackType_t g_pucStackOfColorTask[ 50 ];
static StaticTask_t g_TCBofColorTask;

static StackType_t g_pucStackOfHomePageTask[ 50 ];
static StaticTask_t g_TCBofHomePageTask;

TaskHandle_t xLightTaskHandle;
TaskHandle_t xColorTaskHandle;
TaskHandle_t xHomePageTaskHandle;

TaskHandle_t xDinoGame_Handle;
TaskHandle_t xStarWar_Handler;
TaskHandle_t xFappyBird_Handler;
TaskHandle_t xGreedySnake_Handler;
TaskHandle_t xMusicTaskHandle;

TaskStatus_t xDinoGame_State;
TaskStatus_t xStarWar_State;
TaskStatus_t xFappyBird_State;
TaskStatus_t xGreedySnake_State;
TaskStatus_t xMusic_State;

//static char pcWriteBuffer[100];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
//    uint8_t i;
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
    task. It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()). If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
//    vTaskList( pcWriteBuffer );
//    for ( i = 0; i < 16; i++ )
//    {
//        printf( "-" );
//    }
//    printf( "\r\n" );
//    printf( "%s\r\n", pcWriteBuffer );
}

/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    HomePage_Init();
	
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* 创建任务 */

    /* 测试创建游戏任务 */
	// xTaskCreate( GreedySnake_Task, "GreedySnakeTask", GreedySnake_STK_SIZE, NULL, osPriorityNormal, &xGreedySnake_Handler); 

    /* LED闪烁 */
    xLightTaskHandle = xTaskCreateStatic( Led_Test, "LightTest", 30, NULL, osPriorityNormal, g_pucStackOfLightTask, &g_TCBofLightTask );

    /* 彩色LED */
    xColorTaskHandle = xTaskCreateStatic( ColorLED_Test, "ColorTest", 50, NULL, osPriorityNormal, g_pucStackOfColorTask, &g_TCBofColorTask );

    /* 主页显示 */
    xHomePageTaskHandle = xTaskCreateStatic( HomePage_Task, "HomePageMenu", 50, NULL, osPriorityNormal, g_pucStackOfHomePageTask, &g_TCBofHomePageTask );

    
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    
    /* Infinite loop */
    for(;;)
    {
        // osDelay(1);
        // Led_Test();
        // OLED_Test();
        // PassiveBuzzer_Test();
        // UART_Test();
        // Key_Test();
        vTaskDelay( 50 );
    }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


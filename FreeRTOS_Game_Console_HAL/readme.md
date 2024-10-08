<!--
 * @Author: Jchen
 * @Date: 2024-10-04 19:35:52
 * @LastEditors: Jchen
 * @LastEditTime: 2024-10-07 13:03:33
 * @Description: 
 * @FilePath: \undefinedd:\Embedded\Git\FreeRTOS_Game_Console_HAL\redme.md
-->
#       * 照烧鸡腿饭 开发的 单片机游戏机项目 *
#       * stm32 - FreeRTOS *
##      * 简介 *

适用于STM32单片机，本项目在STM32C8T6单片机上测试通过；
在其他STM32单片机上只需修改少量代码也可使用；
游戏可以关注：B站（ 账号名：照烧鸡腿饭饭 ）有录制视频效果。

STM32F103开发板实现了游戏、无源蜂鸣器播放、OKLED显示盒按键切换任务功能；使用FreeRTOS实时操作系统完成多任务调度。

    使用软件模拟IIC通信编写OLED模块；
    移植FreeRTOS到STM32，使用heap_4内存管理算法，动态创建任务并以时间片轮转方式完成任务调度；
    使用TIM时钟以50毫秒间隔中断扫描按键状态并写入缓冲区，供各任务读取；
    在钩子函数中获取各任务内存占用使用USART输出，并由此优化任务的内存占用率（方法已注释，需要可以取消注释查看效果）；

# * 游戏包含：*
    1. 谷歌小恐龙；
    2. 星球大战；
    3. 飞翔小鸟；
    4. 贪吃蛇；

#  * 任务包含： *
    1. LED闪烁；
    2. RGB彩灯；
    3. 4个游戏；
    4. 音乐播放（无源蜂鸣器播放《 孤勇者 》）
    4. 主页;
    5. 信息显示页面；

# * 硬件材料 *
    1. STM32C8T6 最小系统板                                 *1
    2. SSD1315 OLED 128*64 4针脚显示屏                      *1
    3. 5向导航按键模块（包含一个5向摇杆和2个按键）             *1
            或者使用6个按键，效果、接线一样
    4. 按键                                                 *2
    5. 无源蜂鸣器                                           *1
    6. RGB全彩LED                                           *1

# * IO连接说明 *
    OLED SCK            PB8
    OLED SDA            PB9
    按键：
        方向上          PA4
        方向下          PA5
        方向右          PA6
        方向左          PA7

        STAR           PB0
        SET            PB1

        A              PB12
        B              PB13
    无源蜂鸣器I\O       PA8
    RGBLED
        R              PA2
        B              PB3
        G              PA15
    LED                PC13( STM32板上有个连接PC的LED灯，所以不用外接 )


        
    
        
    

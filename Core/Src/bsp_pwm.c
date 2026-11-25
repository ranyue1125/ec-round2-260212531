#include "main.h"
#include "stm32f1xx_hal_tim.h"
#include "bsp_pwm.h"
extern TIM_HandleTypeDef htim2;
#define SERVO_MIN 500
#define SERVO_MAX 2500
void Set_Servo_Angle(uint8_t channel,float angle)
{
    if(angle<0.0f) angle=0.0f;
    if(angle>180.0f) angle=180.0f;
    uint16_t pulse=SERVO_MIN+(uint16_t)((angle/180.0f)*(SERVO_MAX-SERVO_MIN));
    switch (channel) {
        case 1:
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pulse);
        break;
        case 2:
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pulse);
        break;
        case 3:
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,pulse);
        break;
        case 4:
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,pulse);
        break;
        default:
        break;
    
    }
}
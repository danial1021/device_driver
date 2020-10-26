#ifndef __TIM3_RGB_LED_H__
#define	__TIM3_RGB_LED_H__

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim3;

void User_PWM_SetValue(TIM_HandleTypeDef *htimx, uint16_t Value, uint16_t TIM_CHANNEL);
 
void User_PWM_RGB_Test(TIM_HandleTypeDef *htimx);


#endif /* __TIM3_RGB_LED_H__ */
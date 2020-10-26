

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "main.h"
#include "TIM3_RGB_LED.h"


void User_PWM_SetValue(TIM_HandleTypeDef *htimx, uint16_t Value, uint16_t TIM_CHANNEL){
	
	TIM_OC_InitTypeDef sConfigOC;
	 
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = Value;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	 
	HAL_TIM_PWM_ConfigChannel(htimx, &sConfigOC, TIM_CHANNEL);
	HAL_TIM_PWM_Start(htimx, TIM_CHANNEL);
}
 
void User_PWM_RGB_Test(TIM_HandleTypeDef *htimx){
	for(int i = 0; i <= 999; i += 9){
		User_PWM_SetValue(htimx, i, TIM_CHANNEL_1);
		HAL_Delay(10);
	}
	User_PWM_SetValue(htimx, 0, TIM_CHANNEL_1);
	
	for(int j = 0; j <= 999; j += 9){
		User_PWM_SetValue(htimx, j, TIM_CHANNEL_2);
		HAL_Delay(10);
	}
	User_PWM_SetValue(htimx, 0, TIM_CHANNEL_2);
	
	for(int k = 0; k <= 999; k += 9){
		User_PWM_SetValue(htimx, k, TIM_CHANNEL_3);
		HAL_Delay(10);
	}
	User_PWM_SetValue(htimx, 0, TIM_CHANNEL_3);
	
	for(int l = 0; l <= 999; l += 9){
		User_PWM_SetValue(htimx, l, TIM_CHANNEL_1);
		User_PWM_SetValue(htimx, l, TIM_CHANNEL_2);
		User_PWM_SetValue(htimx, l, TIM_CHANNEL_3);
		HAL_Delay(10);
	}
	User_PWM_SetValue(htimx, 127, TIM_CHANNEL_1);
	User_PWM_SetValue(htimx, 127, TIM_CHANNEL_2);
	User_PWM_SetValue(htimx, 127, TIM_CHANNEL_3);
}
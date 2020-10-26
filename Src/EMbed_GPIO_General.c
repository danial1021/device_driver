#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "main.h"
#include "EMbed_GPIO_General.h"
#include "I2C1_PCA9535A_FND.h"
#include "TIM3_RGB_LED.h"

unsigned char mode, state = 0;
int led_cnt = -1;
int r=0,b=0,g=0,rgb=0;
int fnd_cnt = -1; // not auto
int auto_fnd_cnt = -1; // auto
BUTTON_TypeDef hButton;

void GPIO_BT_READ(BUTTON_TypeDef *hBT){
	hBT->ucState = 	(HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT1) << 0) | (HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT2) << 1)
								| (HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT3) << 2) | (HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT4) << 3)
								| (HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT5) << 4);
	hBT->ucState &= 0xFF;
	
	
	if(hBT->ucState != 0x1F){			//first botton click	
		if(!hBT->ucPressed){
			hBT->ucPressed = HIGH;			
			hBT->uiDelaycount = 0;
			hBT->ucValue = 0;
			hBT->ucValue = hBT->ucState;
			BT_SHORT_Event(hBT->ucValue);
		}	
	}
	else{
		hBT->uiDelaycount = 0;
		hBT->ucPressed = LOW;
	}
}

		
void BT_SHORT_Event(unsigned char bt_state){
   switch(bt_state){
      case 0x1E :	//0b xxx1 1110
				mode = mode + 1;
				if(mode > 3){
				 WrFND(319);
				 mode = 0;
				}else{
					Set_Mode(mode);
					if(mode == 3){
						hPtF.FND[0] = FND_DISPLAY_0;
						hPtF.FND[1] = FND_DISPLAY_1;
						hPtF.FND[2] = FND_DISPLAY_2;
						hPtF.FND[3] = FND_DISPLAY_3;
						hPtF.FND[4] = FND_DISPLAY_4;
						hPtF.FND[5] = FND_DISPLAY_5;
						hPtF.FND[6] = FND_DISPLAY_6;
						hPtF.FND[7] = FND_DISPLAY_7;
						hPtF.FND[8] = FND_DISPLAY_8;
						hPtF.FND[9] = FND_DISPLAY_9;
					}
				}
      break;
			
      case 0x1D :	//0b xxx1 1101
				if(mode == 2){
					g = b = rgb = 0;
					User_PWM_SetValue(&htim3, b, TIM_CHANNEL_1); // blue
					User_PWM_SetValue(&htim3, g, TIM_CHANNEL_3); // green
					while(HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT2) == 0){
						User_PWM_SetValue(&htim3, r, TIM_CHANNEL_2); // red
						r += 8;
						if(r > 255) r = 0;
						HAL_Delay(100);
					}
				}else if(mode == 3){
					state ^= 1;
					while(state){
						if(++auto_fnd_cnt > 9) auto_fnd_cnt = 0;
					
						hPtF.data = 0x00;
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
						
						hPtF.data = FND_DISPLAY_OFF;
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
						
						hPtF.data = hPtF.fndClkEnable[ONE_PLACE];
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
						
						hPtF.data = hPtF.FND[auto_fnd_cnt];
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
						
						hPtF.data = hPtF.fndClkEnable[TEN_PLACE];
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
						
						HAL_Delay(500);
						if(HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT2) == 0){
							state ^= 1;
						}
					}
				}
      break;
			
      case 0x1B :	//0b xxx1 1011
				if(mode == 1){
					if(++led_cnt > 4) led_cnt = 0;
					HAL_GPIO_WritePin(GPIO_LED_PORT, GPIO_LED_1 | GPIO_LED_2 | GPIO_LED_3 | GPIO_LED_4 | GPIO_LED_5, 0);
					HAL_GPIO_WritePin(GPIO_LED_PORT, GPIO_LED_1<<led_cnt, 1);
				}
				else if(mode == 2){
					r = b = rgb = 0;
					
					User_PWM_SetValue(&htim3, r, TIM_CHANNEL_2);
					User_PWM_SetValue(&htim3, b, TIM_CHANNEL_1);
					while(HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT3) == 0){
						User_PWM_SetValue(&htim3, g, TIM_CHANNEL_3);
						g += 8;
						if(g > 255) g = 0;
						HAL_Delay(100);
					}
				}else if(mode == 3){
					state ^= 1;
					while(state){
						if(--auto_fnd_cnt < 0) auto_fnd_cnt = 9;
					
						hPtF.data = 0x00;
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
						
						hPtF.data = FND_DISPLAY_OFF;
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
						
						hPtF.data = hPtF.fndClkEnable[ONE_PLACE];
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
						
						hPtF.data = hPtF.FND[auto_fnd_cnt];
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
						
						hPtF.data = hPtF.fndClkEnable[TEN_PLACE];
						HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
						
						HAL_Delay(500);
						if(HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT3) == 0){
							state ^= 1;
						}
					}
				}
      break;
				 
      case 0x17 :	//0b xxx1 0111
				if(mode == 1){
					if(--led_cnt < 0) led_cnt = 4;
					HAL_GPIO_WritePin(GPIO_LED_PORT, GPIO_LED_1 | GPIO_LED_2 | GPIO_LED_3 | GPIO_LED_4 | GPIO_LED_5, 0);
					HAL_GPIO_WritePin(GPIO_LED_PORT, GPIO_LED_1<<led_cnt, 1);
				}else if(mode == 2){
					g = r = rgb = 0;
					
					User_PWM_SetValue(&htim3, r, TIM_CHANNEL_2);
					User_PWM_SetValue(&htim3, g, TIM_CHANNEL_3);
					while(HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT4) == 0){
						User_PWM_SetValue(&htim3, b, TIM_CHANNEL_1);
						b += 8;
						if(b > 255) b = 0;
						HAL_Delay(100);
					}
				}else if(mode == 3){
					fnd_cnt += 1;
					if(fnd_cnt > 9) fnd_cnt = 0;
					
					hPtF.data = 0x00;
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);

					hPtF.data = hPtF.FND[mode];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.fndClkEnable[HUND_PLACE];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
					
					hPtF.data = FND_DISPLAY_OFF;
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.fndClkEnable[TEN_PLACE];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.FND[fnd_cnt];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.fndClkEnable[ONE_PLACE];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
				}
      break;
				 
      case 0x0F :	//0b xxx0 1111
				if(mode == 1){
					HAL_GPIO_WritePin(GPIO_LED_PORT, GPIO_LED_1 | GPIO_LED_3 | GPIO_LED_5, 1);
					HAL_GPIO_WritePin(GPIO_LED_PORT, GPIO_LED_2 | GPIO_LED_4 , 0);
					HAL_Delay(500);
						
					for(int i=0; i<9; i++){
						HAL_GPIO_TogglePin(GPIO_LED_PORT, GPIO_LED_1 | GPIO_LED_3 | GPIO_LED_5);
						HAL_GPIO_TogglePin(GPIO_LED_PORT, GPIO_LED_2 | GPIO_LED_4);
						HAL_Delay(500);
					}
					HAL_GPIO_WritePin(GPIO_LED_PORT, GPIO_LED_1 | GPIO_LED_2 | GPIO_LED_3 | GPIO_LED_4 | GPIO_LED_5, 0);
				}else if(mode == 2){
					r = b = g = rgb = 0;
					while(HAL_GPIO_ReadPin(GPIO_SW_PORT, GPIO_SW_BT5) == 0){
						User_PWM_SetValue(&htim3, rgb, TIM_CHANNEL_3);
						User_PWM_SetValue(&htim3, rgb, TIM_CHANNEL_2);
						User_PWM_SetValue(&htim3, rgb, TIM_CHANNEL_1);
						rgb += 8;
						if(rgb > 255) rgb = 0;
						HAL_Delay(100);
					}
				}else if(mode == 3){
					fnd_cnt -= 1;
					if(fnd_cnt < 0) fnd_cnt = 9;
					
					hPtF.data = 0x00;
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);

					hPtF.data = hPtF.FND[mode];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.fndClkEnable[HUND_PLACE];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
					
					hPtF.data = FND_DISPLAY_OFF;
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.fndClkEnable[TEN_PLACE];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.FND[fnd_cnt];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
					
					hPtF.data = hPtF.fndClkEnable[ONE_PLACE];
					HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
				}
			break;
   }
 }

void BT_LONG_Event_test(){
	for(int i=0; i<=10; i++){
		HAL_GPIO_TogglePin(GPIO_LED_PORT, GPIO_LED_1 | GPIO_LED_2 | GPIO_LED_3 | GPIO_LED_4 | GPIO_LED_5);
		HAL_Delay(GPIO_DLY);
	}
}

//bt TO timer
void BT_TICK_FUNC(BUTTON_TypeDef *hBT){
	if(hBT->ucPressed == HIGH){
		hBT->uiDelaycount++;
	}
	else{
		hBT->uiDelaycount = 0;
	}
}


#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "main.h"
#include "I2C1_PCA9535A_FND.h"


PCA9535AtoFND_TypeDef hPtF;

void I2C1_PCA9535A_Config()
{
	hPtF.data = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, DevAddr0_W, MemAddr_Configuration_port_0, 1, &hPtF.data, 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, DevAddr0_W, MemAddr_Configuration_port_1, 1, &hPtF.data, 1, 100);
	hPtF.fndClkEnable[ONE_PLACE]	= FND3_CLK;
	hPtF.fndClkEnable[TEN_PLACE]	= FND2_CLK;
	hPtF.fndClkEnable[HUND_PLACE]	= FND1_CLK;	
	hPtF.fndClkEnableAll = FND3_CLK | FND2_CLK | FND1_CLK;
}

void I2C1_FND_TEST()
{
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
	hPtF.FND[10] = FND_DISPLAY_ON;
	
	for (int i=0; i<11; i++) {
		hPtF.data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);

		hPtF.data = hPtF.FND[i];
		HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
		
		hPtF.data = hPtF.fndClkEnableAll;
		HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
		HAL_Delay(200);
	}
	hPtF.data = 0x00;		//hPtF.data Init
}

uint16_t WordToFND(uint16_t Dec)	
{	
	uint16_t Bcd = 0;
    
	Dec = Dec % 10000;
	
	Bcd = (Dec / 1000) << 12;									//0b xxxx 0000 0000 0000 
	Dec = Dec % 1000;
    
	Bcd = Bcd | (Dec / 100) << 8;							//0b xxxx 0000 0000 0000  | 0b 0000 xxxx 0000 0000
	Dec = Dec % 100;
	
	Bcd = Bcd | (((Dec/10)<<4) + (Dec%10));		//		(0b xxxx 0000 0000 0000  | 0b 0000 xxxx 0000 0000) 
																						//	| (0b 0000 0000 xxxx 0000  + 0b 0000 0000 0000 xxxx)
	return Bcd;
}

uint8_t BcdToFND(uint8_t Bcd)			
{
	uint8_t BcdToBit = 0;
	
	switch(Bcd)
	{
		case 0 :
		BcdToBit = FND_DISPLAY_0;
		break;
		case 1 :
		BcdToBit = FND_DISPLAY_1;
		break;
		case 2 :
		BcdToBit = FND_DISPLAY_2;
		break;
		case 3 :
		BcdToBit = FND_DISPLAY_3;
		break;
		case 4 :
		BcdToBit = FND_DISPLAY_4;
		break;
		case 5 :
		BcdToBit = FND_DISPLAY_5;
		break;
		case 6 :
		BcdToBit = FND_DISPLAY_6;
		break;
		case 7 :
		BcdToBit = FND_DISPLAY_7;
		break;
		case 8 :
		BcdToBit = FND_DISPLAY_8;
		break;
		case 9 :
		BcdToBit = FND_DISPLAY_9;
		break;
		case 10 :
		BcdToBit = FND_DISPLAY_E;
		break;
		case 11 :
		BcdToBit = FND_DISPLAY_n;
		break;
		case 12 :
		BcdToBit = FND_DISPLAY_d;
		break;
		case 13 :
		BcdToBit = FND_DISPLAY_S;
		break;
		case 14 :
		BcdToBit = FND_DISPLAY_r;
		break;
		case 15 :
		BcdToBit = FND_DISPLAY_OFF;
		break;
		case 16 :
		BcdToBit = FND_DISPLAY_t;
		break;
		case 17 :
		BcdToBit = FND_DISPLAY_b_DP;
		break;
		case 18 :
		BcdToBit = FND_DISPLAY_P;
		break;
		case 19 :
		BcdToBit = FND_DISPLAY_o_DP;
		break;
		case 20 :
		BcdToBit = FND_DISPLAY_r_DP;
		break;	
		case 21 :
		BcdToBit = FND_DISPLAY_C;
		break;
		case 22 :
		BcdToBit = FND_DISPLAY_A;
		break;
		case 23 :
		BcdToBit = FND_DISPLAY_L_DP;
		break;
		case 24 :
		BcdToBit = FND_DISPLAY_y_DP;
		break;
	}
	return BcdToBit;
} 

void WrFND(uint16_t Dec){

	uint16_t Bcd = 0;
	uint8_t BcdToDec[3] = {0,};
	
	if(Dec==0x0fff){
		Bcd = 0x0fff;
	} 
	else{
		Bcd = WordToFND(Dec);			
	}
	
	Bcd = (Bcd & 0x0fff);
	
	BcdToDec[HUND_PLACE]	= ( (Bcd & 0x0f00) >> 8 );
	BcdToDec[HUND_PLACE]	= BcdToFND(BcdToDec[HUND_PLACE]);
	
	BcdToDec[TEN_PLACE]		= ( (Bcd & 0x00f0) >> 4 );
	BcdToDec[TEN_PLACE]		= BcdToFND(BcdToDec[TEN_PLACE]);
	
	BcdToDec[ONE_PLACE]		= ( Bcd & 0x000f ); 
	BcdToDec[ONE_PLACE]		= BcdToFND(BcdToDec[ONE_PLACE]);
	
	
	
	for(int i=0; i<3; i++){
		hPtF.data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
		
		hPtF.data = BcdToDec[i];
		HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
		
		hPtF.data = hPtF.fndClkEnable[i];
		HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
	}
}

void Set_Mode(unsigned char mode){
	hPtF.FND[1] = FND_DISPLAY_1;
	hPtF.FND[2] = FND_DISPLAY_2;
	hPtF.FND[3] = FND_DISPLAY_3;
	
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
	
	hPtF.data = FND_DISPLAY_OFF;
	HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x02, 1, &hPtF.data, 1, 100);
	
	hPtF.data = hPtF.fndClkEnable[ONE_PLACE];
	HAL_I2C_Mem_Write(&hi2c1, 0x42, 0x03, 1, &hPtF.data, 1, 100);
}










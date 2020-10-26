#ifndef __I2C_PCA9535A_FND_H__
#define	__I2C_PCA9535A_FND_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"


//---------[PCA9535A Define START]------------------------------------
//								7bit	6bit	5bit	4bit	3bit	2bit	1bit	0bit
// DevAddr =>				0			1			0			0		 A2		 A1		 A0		R/W 
#define DevAddr0_W		0x42
#define DevAddr0_R		0x43
#define DevAddr1_W		0x44
#define DevAddr1_R		0x45
#define DevAddr2_W		0x48
#define DevAddr2_R		0x49

#define MemAddr_Input_port_0								0x00
#define MemAddr_Input_port_1								0x01
#define MemAddr_Output_port_0								0x02
#define MemAddr_Output_port_1								0x03
#define MemAddr_Polarity_Inversion_port_0		0x04
#define MemAddr_Polarity_Inversion_port_1		0x05
#define MemAddr_Configuration_port_0				0x06
#define MemAddr_Configuration_port_1				0x07

#define FND1_CLK		0x02		//0b 0000 00x0
#define FND2_CLK		0x08		//0b 0000 x000
#define FND3_CLK		0x20		//0b 00x0 0000

//---------[PCA9535A Define END]--------------------------------------


//---------[FND Define START]-----------------------------------------

#define FND_DISPLAY_ON		0x00
#define FND_DISPLAY_0			0xC0
#define FND_DISPLAY_1			0xF9
#define FND_DISPLAY_2			0xA4
#define FND_DISPLAY_3			0xB0
#define FND_DISPLAY_4			0x99
#define FND_DISPLAY_5			0x92
#define FND_DISPLAY_6			0x82
#define FND_DISPLAY_7			0xD8
#define FND_DISPLAY_8			0x80
#define FND_DISPLAY_9			0x90
#define FND_DISPLAY_OFF		0xFF
#define FND_DISPLAY_E			0x86
#define FND_DISPLAY_n			0xAB
#define FND_DISPLAY_d			0xA1
#define FND_DISPLAY_S			0x92
#define FND_DISPLAY_r			0xAF
#define FND_DISPLAY_t			0x87
#define FND_DISPLAY_b_DP	0x03
#define FND_DISPLAY_P			0x8C
#define FND_DISPLAY_o_DP	0x23
#define FND_DISPLAY_r_DP	0x2F
#define FND_DISPLAY_C			0xC6
#define FND_DISPLAY_A			0x88
#define FND_DISPLAY_L_DP	0x47
#define FND_DISPLAY_y_DP	0x11

#define FND_E			10
#define	FND_n			11
#define	FND_d			12
#define FND_S			13
#define FND_r			14
#define FND_t			16
#define FND_b_DP	17
#define FND_P			18
#define FND_o_DP	19
#define FND_r_DP	20
#define FND_C			21
#define FND_A			22
#define FND_L_DP	23
#define FND_y_DP	24

#define HUND_PLACE	2
#define	TEN_PLACE		1
#define ONE_PLACE		0

//---------[FND Define END]-------------------------------------------

extern I2C_HandleTypeDef hi2c1;


typedef struct
{
	uint8_t data;
	uint8_t fndClkEnable[3];
	uint8_t FND[11];
	uint8_t fndClkEnableAll;
}PCA9535AtoFND_TypeDef;

extern PCA9535AtoFND_TypeDef hPtF;

void I2C1_PCA9535A_Config(void);

void I2C1_FND_TEST(void);

uint16_t WordToFND(uint16_t Dec);

uint8_t BcdToFND(uint8_t Bcd);

void WrFND(uint16_t Dec);

void Set_Mode(unsigned char mode);


#endif /* __I2C_PCA9535A_FND_H__ */

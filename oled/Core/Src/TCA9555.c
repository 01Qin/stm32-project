/*
 * TCA9555.c
 *
 *  Created on: 2026. 05. 22.
 *      Author: P
 */

#include "TCA9555.h"
#include "stm32g4xx_hal.h"

//#define TCA9555_ADD_0	0x20		// 0x20 - 0x27
//#define TCA9555_ADD_1	0x21
//#define TCA9555_ADD_2	0x22
//#define TCA9555_ADD_3	0x23
//#define TCA9555_ADD_4	0x24
//#define TCA9555_ADD_5	0x25
//#define TCA9555_ADD_6	0x26
//#define TCA9555_ADD_7	0x27
//
//#define TCA9555_INPUT_PORT_0	0x00
//#define TCA9555_INPUT_PORT_1	0x01
//#define TCA9555_OUTPUT_PORT_0	0x02
//#define TCA9555_OUTPUT_PORT_1	0x03
//#define TCA9555_POL_INV_0		0x04
//#define TCA9555_POL_INV_1		0x05
//#define TCA9555_CONFIG_PORT_0	0x06
//#define TCA9555_CONFIG_PORT_1	0x07
//
//#define TCA9555_ALL_IN	0xff
//#define TCA9555_ALL_OUT	0x00


void TCA9555_All_Output_Init(I2C_HandleTypeDef *I2CHandler, uint8_t address){
	uint8_t buf[2] = {0};
	buf[0] = TCA9555_CONFIG_PORT_0;
	buf[1] = TCA9555_ALL_OUT;
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
	buf[0] = TCA9555_CONFIG_PORT_1;
	buf[1] = TCA9555_ALL_OUT;
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);

	// Set all 0
	buf[0] = TCA9555_OUTPUT_PORT_0;
	buf[0] = 0;
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
	buf[0] = TCA9555_OUTPUT_PORT_1;
	buf[0] = 0;
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
}

void TCA9555_All_Input_Init(I2C_HandleTypeDef *I2CHandler, uint8_t address){
	uint8_t buf[2] = {0};
	buf[0] = TCA9555_CONFIG_PORT_0;
	buf[1] = TCA9555_ALL_IN;
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
	buf[0] = TCA9555_CONFIG_PORT_1;
	buf[1] = TCA9555_ALL_IN;
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
}

void TCA9555_Write_AllOutput(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t digitOutValues[16]){
	uint8_t buf[2] = {0};
	buf[0] = TCA9555_OUTPUT_PORT_1;
	buf[1] = buf[1]<<1 | digitOutValues[15];
	buf[1] = buf[1]<<1 | digitOutValues[14];
	buf[1] = buf[1]<<1 | digitOutValues[13];
	buf[1] = buf[1]<<1 | digitOutValues[12];
	buf[1] = buf[1]<<1 | digitOutValues[11];
	buf[1] = buf[1]<<1 | digitOutValues[10];
	buf[1] = buf[1]<<1 | digitOutValues[9];
	buf[1] = buf[1]<<1 | digitOutValues[8];
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
	buf[0] = TCA9555_OUTPUT_PORT_0;
	buf[1] = 0;
	buf[1] = buf[1]<<1 | digitOutValues[7];
	buf[1] = buf[1]<<1 | digitOutValues[6];
	buf[1] = buf[1]<<1 | digitOutValues[5];
	buf[1] = buf[1]<<1 | digitOutValues[4];
	buf[1] = buf[1]<<1 | digitOutValues[3];
	buf[1] = buf[1]<<1 | digitOutValues[2];
	buf[1] = buf[1]<<1 | digitOutValues[1];
	buf[1] = buf[1]<<1 | digitOutValues[0];
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
}

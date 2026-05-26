/*
 * DAC7578.c
 *
 *  Created on: 2026. 05. 22.
 *      Author: P
 */

#include "DAC7578.h"
#include "stm32g4xx_hal.h"

#define DAC7578_ADD_X			0x47	//Tests Address. Not official
#define DAC7578_ADD_0			0x48	//ADDR0 pin: 0
#define DAC7578_ADD_1			0x4A	//ADDR0 pin: 1
#define DAC7578_ADD_2			0x4C	//ADDR0 pin: Float
//Write Sequences
#define DAC7578_WRITE_DAC_N				0x00	//Write to DAC input register channel n
#define DAC7578_SEL_UPD_DAC_N			0x10	//Select to update DAC register channel n
#define DAC7578_WRITE_DAC_N_UPD_ALL		0x20	//Write to DAC input register channel n, and update all DAC registers (global software LDAC)
#define DAC7578_WRITE_DAC_N_UPD_N		0x30	//Write to DAC input register channel n, and update DAC register channel n
//Read Sequences
#define DAC7578_PWR_DWN_ON_DAC			0x40	//Power down/on DAC
#define DAC7578_WRITE_TO_CLR_CODE		0x50	//Write to clear code register
#define DAC7578_WRITE_TO_LDAC			0x60	//Write to LDAC register
#define DAC7578_SW_RESET				0x70	//Software reset
//Access Sequences
#define DAC7578_CH_A				0x00	//DAC ch. A
#define DAC7578_CH_B				0x01	//DAC ch. B
#define DAC7578_CH_C				0x02	//DAC ch. C
#define DAC7578_CH_D				0x03	//DAC ch. D
#define DAC7578_CH_E				0x04	//DAC ch. E
#define DAC7578_CH_F				0x05	//DAC ch. F
#define DAC7578_CH_G				0x06	//DAC ch. G
#define DAC7578_CH_H				0x07	//DAC ch. H
#define DAC7578_CH_ALL				0x0F	//All DAC channels, broadcast update

void DAC7578_Write_SingleChannel(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t commandAndChannel, uint16_t output)
{
	output = output<<4;
	uint8_t buf[3];
	uint8_t lowByte = (uint8_t)(output & 0x00ff);
	uint8_t highByte = (uint8_t)(output >> 8);
	buf[0] = commandAndChannel;
	buf[1] = highByte;
	buf[2] = lowByte;
	HAL_I2C_Master_Transmit(I2CHandler, address<<1, buf, sizeof(buf), HAL_MAX_DELAY);
}

void DAC7578_Write_AllChannel(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint16_t dacChannelValues[8])
{
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_A, dacChannelValues[0]);
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_B, dacChannelValues[1]);
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_C, dacChannelValues[2]);
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_D, dacChannelValues[3]);
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_E, dacChannelValues[4]);
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_F, dacChannelValues[5]);
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_G, dacChannelValues[6]);
	DAC7578_Write_SingleChannel(I2CHandler, address, DAC7578_WRITE_DAC_N_UPD_N | DAC7578_CH_H, dacChannelValues[7]);
}

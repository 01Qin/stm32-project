/*
 * TCA9555.h
 *
 *  Created on: 2026. 05. 22.
 *      Author: P
 */

#ifndef INC_TCA9555_H_
#define INC_TCA9555_H_

#include "stm32g4xx_hal.h"

#define TCA9555_ADD_0	0x20		// 0x20 - 0x27
#define TCA9555_ADD_1	0x21
#define TCA9555_ADD_2	0x22
#define TCA9555_ADD_3	0x23
#define TCA9555_ADD_4	0x24
#define TCA9555_ADD_5	0x25
#define TCA9555_ADD_6	0x26
#define TCA9555_ADD_7	0x27

#define TCA9555_INPUT_PORT_0	0x00
#define TCA9555_INPUT_PORT_1	0x01
#define TCA9555_OUTPUT_PORT_0	0x02
#define TCA9555_OUTPUT_PORT_1	0x03
#define TCA9555_POL_INV_0		0x04
#define TCA9555_POL_INV_1		0x05
#define TCA9555_CONFIG_PORT_0	0x06
#define TCA9555_CONFIG_PORT_1	0x07

#define TCA9555_ALL_IN	0xff
#define TCA9555_ALL_OUT	0x00

void TCA9555_All_Output_Init(I2C_HandleTypeDef *I2CHandler, uint8_t address);

void TCA9555_All_Input_Init(I2C_HandleTypeDef *I2CHandler, uint8_t address);

void TCA9555_Write_AllOutput(I2C_HandleTypeDef *I2CHandler, uint8_t address, uint8_t digitOutValues[16]);

#endif /* INC_TCA9555_H_ */

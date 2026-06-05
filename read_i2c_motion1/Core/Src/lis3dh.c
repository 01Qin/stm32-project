/*
 * lis3dh.c
 *
 *  Created on: Mar 4, 2023
 *      Author: jed
 */

#include "lis3dh.h"

//#define I2C_READ_BIT   1
#define I2C_WRITE_BIT  0
#define TIMEOUT_MS     50

HAL_StatusTypeDef lis3dh_init(lis3dh_t *lis3dh, I2C_HandleTypeDef *i2c, uint8_t *buf, uint16_t bufsize) {
	HAL_StatusTypeDef status;

	lis3dh->i2c = i2c;
	lis3dh->i2c_addr = LIS3DH_ADDR;
	lis3dh->buf = buf;
	lis3dh->bufsize = bufsize;

	/* Let device wake up. */
	HAL_Delay(LID3DH_POWER_UP_MS);

	status = HAL_I2C_IsDeviceReady(lis3dh->i2c, lis3dh->i2c_addr, 1, TIMEOUT_MS);
	if (status != HAL_OK) return status;


    /* Confirm the device identifies itself as expected. */
	status = lis3dh_read(lis3dh, REG_WHO_AM_I, 1);
	if (status != HAL_OK) return status;
	if (lis3dh->buf[0] != LIS3DH_DEVICE_ID) return HAL_ERROR;

	// Set power mode to operational; Enable all axes; Normal operation.
	status = lis3dh_write(lis3dh, REG_CTRL_REG1, DATA_RATE_NORM_1kHz344 | 0x07);
	if (status != HAL_OK) return status;

	// High resolution; BDU enabled.
	status = lis3dh_write(lis3dh, REG_CTRL_REG4, 0x88);
	if (status != HAL_OK) return status;

//	status = lis3dh_enable_freefall(lis3dh);
//	if (status != HAL_OK) return status;

//	status = lis3dh_enable_tap(lis3dh);
//	if (status != HAL_OK) return status;

	status = lis3dh_tap(lis3dh);

}

//HAL_StatusTypeDef lis3dh_enable_freefall(lis3dh_t *lis3dh){
//
//	HAL_StatusTypeDef status;
//	uint8_t ret;
//
////	INT1_CFG: AOI=1, XLIE=1, YLIE=1, ZLIE=1
//	status = lis3dh_write(lis3dh, REG_INT1_CFG, 0x95);
//	if (status != HAL_OK) return status;
//
//	// INT1_THS: threshold ~350 mg (0x16)
//	status = lis3dh_write(lis3dh, REG_INT1_THS, 0x16);
//	if (status != HAL_OK) return status;
//
//	// INT1_DURATION: ~30 ms (0x02)
//	status = lis3dh_write(lis3dh, REG_INT1_DURATION, 0x02);
//	if (status != HAL_OK) return status;
//
//	// enable ia1 interrrupt on INT1 pin
//	status = lis3dh_read(lis3dh, REG_CTRL_REG3, 1);
//	if (status != HAL_OK) return status;
//
//	ret = lis3dh->buf[0];
//	ret |= 0x40;
//	status = lis3dh_write(lis3dh, REG_CTRL_REG3, ret);
//	if (status != HAL_OK) return status;
//
//	// Latch interrupt
//	status = lis3dh_read(lis3dh, REG_CTRL_REG5, 1);
//	if (status != HAL_OK) return status;
//
//	ret = lis3dh->buf[0];
//	ret |= 0x08;
//	status = lis3dh_write(lis3dh, REG_CTRL_REG5, ret);
//	if (status != HAL_OK) return status;
//
//	return HAL_OK;
//}

//HAL_StatusTypeDef lis3dh_enable_tap(lis3dh_t *lis3dh){
//
//	HAL_StatusTypeDef status;
//
////	enable tap on z (CLICK_CFG 0x15)
//	status = lis3dh_write(lis3dh, REG_CLICK_CFG, 0x15);
//	if (status != HAL_OK) return status;
//
//	// CLICK_THS: threshold ~0.28 mg (0x12)
//	status = lis3dh_write(lis3dh, REG_CLICK_THS, 0x10);
//	if (status != HAL_OK) return status;
//
//	// TIME_LIMIT: ~120 ms (0x33)
//	status = lis3dh_write(lis3dh, REG_TIME_LIMIT, 0x20);
//	if (status != HAL_OK) return status;
//
//	lis3dh_write(lis3dh, REG_TIME_LATENCY, 0x20);
//
//	// enable CLICK interrupt on INT1 pin
//	lis3dh_read(lis3dh, REG_CTRL_REG3, 1);
//	uint8_t val = lis3dh->buf[0] | 0x80;
//	status = lis3dh_write(lis3dh, REG_CTRL_REG3, val);
//
//	return status;
//}

HAL_StatusTypeDef lis3dh_hit(lis3dh_t *lis3dh){

	HAL_StatusTypeDef status;

	//	INT1_CFG: AOI=1, XLIE=1, YLIE=1, ZLIE=1
	status = lis3dh_write(lis3dh, REG_INT1_CFG, 0x0A);
	if (status != HAL_OK) return status;

	// INT1_THS: threshold ~0.28 mg (0x12)
	status = lis3dh_write(lis3dh, REG_INT1_THS, 0x40);
	if (status != HAL_OK) return status;

	// TIME_LIMIT: ~120 ms (0x33)

	status = lis3dh_write(lis3dh, REG_INT1_DURATION, 0x05);
	if (status != HAL_OK) return status;

	lis3dh_write(lis3dh, REG_TIME_LATENCY, 0x20);


//
//	uint8_t val = lis3dh->buf[0] | 0x40;
//	status = lis3dh_write(lis3dh, REG_CTRL_REG3, val);

	// latch interrupt
	lis3dh_read(lis3dh, REG_CTRL_REG5, 0xF1);
//
//	val = lis3dh->buf[0] | 0x80;
//	status = lis3dh_write(lis3dh, REG_CTRL_REG5, val);

	// enable CLICK interrupt on INT1 pin
	lis3dh_read(lis3dh, REG_CTRL_REG3, 0x60);

	return status;
}


bool lis3dh_xyz_available(lis3dh_t *lis3dh) {
	/*
	 * Read STATUS_REG bit 2 (ZYXDA): New X, Y, Z data available.
	 */
	HAL_StatusTypeDef status;
	status = lis3dh_read(lis3dh, REG_STATUS_REG, 1);
	if (status != HAL_OK) return false;

	return (lis3dh->buf[0] & 0x08) > 0;
}

//bool lis3dh_freefall_detected(lis3dh_t *lis3dh){
//
//	HAL_StatusTypeDef status;
//
//	status = lis3dh_read(lis3dh, REG_INT1_SRC, 1);
//	if (status != HAL_OK) return false;
//
//	uint8_t src = lis3dh->buf[0];
//
//	if ((src & 0x40) == 0){
//		return false;
//	}
//	return (src & 0x3F) == 0;
//
//
//}

bool lis3dh_tap_detected(lis3dh_t *lis3dh){

	HAL_StatusTypeDef status;

	status = lis3dh_read(lis3dh, REG_INT1_SRC, 1);
	if (status != HAL_OK) return false;

	uint8_t src = lis3dh->buf[0];

	if (!(src & 0x40)){
	return false;
	}

	return (src & 0x3F) != 0;
}


HAL_StatusTypeDef lis3dh_read(lis3dh_t* lis3dh, uint16_t reg, uint16_t bufsize) {
	if (bufsize > lis3dh->bufsize) return HAL_ERROR;

	return HAL_I2C_Mem_Read(lis3dh->i2c, lis3dh->i2c_addr, reg, 1, lis3dh->buf, bufsize, TIMEOUT_MS);
}

HAL_StatusTypeDef lis3dh_write(lis3dh_t* lis3dh, uint16_t reg, uint8_t data) {
	return HAL_I2C_Mem_Write(lis3dh->i2c, lis3dh->i2c_addr, reg, 1, &data, 1, TIMEOUT_MS);
}

HAL_StatusTypeDef lis3dh_get_xyz(lis3dh_t* lis3dh) {
	if (lis3dh->bufsize < 6) return HAL_ERROR;
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
			lis3dh->i2c,
			lis3dh->i2c_addr,
			REG_OUT_XYZ_BASE | 0x80,          // Progressively read 6 buffers.
			1,
			lis3dh->buf,
			6,
			TIMEOUT_MS);

	if (status != HAL_OK) {
		lis3dh->x = -1;
		lis3dh->y = -1;
		lis3dh->z = -1;
		return status;
	}

	lis3dh->x = (int) (((int8_t) lis3dh->buf[1]) << 8) | lis3dh->buf[0];
	lis3dh->y = (int) (((int8_t) lis3dh->buf[3]) << 8) | lis3dh->buf[2];
	lis3dh->z = (int) (((int8_t) lis3dh->buf[5]) << 8) | lis3dh->buf[4];

	return HAL_OK;
}

/* To-do */
HAL_StatusTypeDef lis3dh_get_temp(lis3dh_t* lis3dh) {
	return HAL_ERROR;
}

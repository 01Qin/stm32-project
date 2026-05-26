/*
 * NTC.c
 *
 *  Created on: 2026. ápr. 20.
 *      Author: P
 */

#include <math.h>
#include <stdint.h>
#include "NTC.h"

uint16_t ntc_to_adc(int16_t temp_celsius)
{
    const float VREF = 3.3f;
    const float R_FIXED = 10000.0f;
    const float B = 3453.0f;
    const float T0 = 298.15f; // 25°C °K

    // C → K
    float T = temp_celsius + 273.65f;	// + 273.15f + 0.5f

    // NTC resistance
    float exponent = B * ((1.0f / T) - (1.0f / T0));
    float R_ntc = R_FIXED * expf(exponent);

    // Voltage divider
    float Vout = VREF * R_ntc / (R_FIXED + R_ntc);

    // ADC (12 bit)
    float adc = Vout / (VREF / 4096.0f);

    return (uint16_t)(adc + 0.5f); // round
}

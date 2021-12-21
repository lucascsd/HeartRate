/*
 * max30102_CIAA_port.h
 *
 *  Created on: 11 oct. 2021
 *      Author: lucascsd
 */

#ifndef MAX30102_CIAA_PORT_H_
#define MAX30102_CIAA_PORT_H_

#include "sapi_max30102.h"

/*==================[inclusions]=============================================*/

bool_t i2cInit_CIAA_port ( i2cMap_t i2cNumber, uint32_t clockRateHz );
bool_t max30102Write_CIAA_port ( i2cMap_t  i2cNumber, uint8_t  i2cSlaveAddress, uint8_t registerAddr, uint8_t data );
bool_t max30102Read_CIAA_port ( i2cMap_t  i2cNumber, uint8_t  i2cSlaveAddress, uint8_t registerAddr, uint8_t* receiveDataBuffer, uint16_t receiveDataBufferSize );
uint32_t delay_CIAA_port ( void );

#endif /* MAX30102_CIAA_PORT_H_ */

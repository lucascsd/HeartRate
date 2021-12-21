/*
 * max30102_CIAA_port.c
 *
 *  Created on: 11 oct. 2021
 *      Author: lucascsd
 */

#include "max30102_CIAA_port.h"

/*************************************************************************************************
 *  @brief Funcion para set/reset del chip enable para MAX6675
 *
 *  @details
 *   	Esta funcion es especifica para el hardware utilizado. Notar que se basa en una capa
 *   	HAL existente (LPC Open)
 *
 *  @param		estado	Determina la accion a ser tomada con el pin CS.
 *  @return     None.
 ***************************************************************************************************/

bool_t i2cInit_CIAA_port ( i2cMap_t i2cNumber, uint32_t clockRateHz )
{

	/* Selecciono puerto I2C e inicializo */
	if ( !i2cInit ( i2cNumber, clockRateHz) )
		return FALSE;
	else
		return TRUE;

}

/* Escritura en el registro definido por registerAddr */
bool_t max30102Write_CIAA_port ( i2cMap_t  i2cNumber, uint8_t  i2cSlaveAddress, uint8_t registerAddr, uint8_t data )

{

	uint8_t transmitDataBuffer[2];
	transmitDataBuffer[0] = registerAddr;
	transmitDataBuffer[1] = data;

	return i2cWrite(i2cNumber, i2cSlaveAddress, transmitDataBuffer, 2, TRUE);

}

/* Lectura en el registro definido por registerAddr */
bool_t max30102Read_CIAA_port ( i2cMap_t  i2cNumber, uint8_t  i2cSlaveAddress, uint8_t registerAddr,
			uint8_t* receiveDataBuffer, uint16_t receiveDataBufferSize )
{
	return i2cRead ( i2cNumber, i2cSlaveAddress, &registerAddr, 1, TRUE, receiveDataBuffer, receiveDataBufferSize, TRUE );
}

/**************************************************************************/
/*!
    @brief      Funcion para realizar un delay bloqueante.

    @param[in]  millisecs
				La cantidad de milisegundos del delay.
*/
/**************************************************************************/
uint32_t delay_CIAA_port(void)
{
	tick_t number_of_ticks = tickRead();
	return (uint32_t)number_of_ticks;
}

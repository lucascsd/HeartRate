/*
 * sapi_max30102.c
 *
 *  Created on: 2 oct. 2021
 *      Author: lucascsd
 */
#include "sapi_max30102.h"

/* Variables goblaes */

max30102_t _max30102;

max30102_config_t _config;

/* Mis funciones */

/* Init board MAX30102 */
bool_t max30102_Init( max30102_t driver_config )
{

	// _max30102._i2cPortFn 	= driver_config._i2cPortFn;
	// _max30102._i2cWriteFn 	= driver_config._i2cWriteFn;
	// _max30102._i2cReadFn 	= driver_config._i2cReadFn;
	// _max30102._delay		= driver_config._delay;

	// /* Selecciono puerto I2C e inicializo */
	// if ( !_max30102._i2cPortFn(I2C0, MAX30102_I2C_RATE_STD) )
	// 	return FALSE;

	/* Selecciono puerto I2C e inicializo */
	if ( !i2cInit_CIAA_port(I2C0, MAX30102_I2C_RATE_STD) )
		return FALSE;

	/* Verificar conexión de MAX30102 */
	if ( !max30102_readPartID( ) )
		return false;

	/* Lectura de Revision ID */
	if ( !max30102_readRevisionID ( ) )
		return false;

	/* POR register */
	if ( !max30102_reset() )
		return FALSE;

	/* Configuracion del device */
	if ( !max30102_setup ( _config ) )
		return FALSE;

	return TRUE;
}

/* Funcion para obtener PART ID */
uint8_t max30102_readPartID ( )
{
	uint8_t data;

	if ( _max30102._i2cReadFn ( I2C0, MAX_ADDRESS, PART_ID, &data, 1 ) )
		return data;
	else
		return 0;
}

/* Funcion para obtener REVISION ID */
uint8_t max30102_readRevisionID ( )
{
	uint8_t data;

	if ( _max30102._i2cReadFn ( I2C0, MAX_ADDRESS, REVISION_ID, &data, 1 ) )
		return data;
	else
		return 0;
}

/* Configuracion del device */
bool_t max30102_setup ( max30102_config_t _configDevice )
{
	/* Sample average */
	if ( !max30102_config ( FIFO_CONFIG, SMP_AVE_8, 0 ) )
		return FALSE;

	/* Roll Over */
	if ( !max30102_config ( FIFO_CONFIG, FIFO_ROLLOVER_EN, 0 ) )
		return FALSE;

	/* Set LED Mode */
	if ( !max30102_config ( MODE_CONFIG, MODE_MULTI, 0 ) )
		return FALSE;

	/* Set ADC Range Mode */
	if ( !max30102_config ( SPO2_CONFIG, SPO2_ADC_RGE_2048, 0 ) )
		return FALSE;

	/* Set Sample Rate Mode */
	if ( !max30102_config ( SPO2_CONFIG, SAMPLERATE_1000, 0 ) )
		return FALSE;

	/* Set Pulse width Mode */
	if ( !max30102_config ( SPO2_CONFIG, PULSEWIDTH_411, 0 ) )
		return FALSE;

	/* Set Led_1 Mode */
	if ( !max30102_config ( LED_PULSE_AMP_1, PULSEAMPLITUDE_12, 0 ) )
		return FALSE;

	/* Set Led_2 Mode */
	if ( !max30102_config ( LED_PULSE_AMP_2, PULSEAMPLITUDE_12, 0 ) )
		return FALSE;

	/* Set Led_1 Config */
	if ( !max30102_config ( LED_CTRL_REG_12, MULTILED_CONTROL_RED, 0 ) )
		return FALSE;

	/* Set Led_2 Config */
	if ( !max30102_config ( LED_CTRL_REG_12, MULTILED_CONTROL_IR, 4 ) )
		return FALSE;

	/* Reset the FIFO before we begin checking the sensor */
	if ( !max30102_clearFIFO() )
		return FALSE;

	return TRUE;
}

bool_t max30102_config ( uint8_t _register, uint8_t _param, uint8_t shitf )
{
	uint8_t datoActual;

	 _max30102._i2cReadFn ( I2C0, MAX_ADDRESS, _register, &_config._buffer, 1 );
	 datoActual = _config._buffer;

	/* Enable roll over if FIFO over flows */
	max30102_maskRegister ( _register, ~_param, _param << shitf );

	if ( _config._buffer == datoActual | _param )
		return TRUE;
	else
		return FALSE;

}

/* Reset all configuration, threshold, and data registers to POR values */
bool_t max30102_reset ()
{

	max30102_maskRegister ( MODE_CONFIG, ~MODE_RESET, MODE_RESET );

	if ( (_config._buffer & MODE_RESET) == 0 )
		return TRUE;
	else
		return FALSE;

}

bool_t max30102_clearFIFO ( )
{

	if ( !_max30102._i2cWriteFn ( I2C0, MAX_ADDRESS, FIFO_WRITE_POINTER, 0x00 ) )
		return FALSE;
	if ( !_max30102._i2cWriteFn ( I2C0, MAX_ADDRESS, OVERFLOW_COUNTER, 0x00 ) )
		return FALSE;
	if ( !_max30102._i2cWriteFn ( I2C0, MAX_ADDRESS, FIFO_READ_POINTER, 0x00 ) )
		return FALSE;

	return TRUE;
}

float_t max30102_readNewValue ( )
{
	return max30102_check();

}

/* Read the FIFO Write Pointer */
uint8_t max30102_getWritePointer ( )
{
	uint8_t datoActual;

	if ( _max30102._i2cReadFn ( I2C0, MAX_ADDRESS, FIFO_WRITE_POINTER, &datoActual, 1 ))
		return datoActual;

	return 0;
}

/* Read the FIFO Read Pointer */
uint8_t max30102_getReadPointer ( )
{
	uint8_t datoActual;

	if ( _max30102._i2cReadFn ( I2C0, MAX_ADDRESS, FIFO_READ_POINTER, &datoActual, 1 ))
		return datoActual;

	return 0;
}

int16_t max30102_check ( )
{

	uint8_t readPointer = max30102_getReadPointer();
	uint8_t writePointer = max30102_getWritePointer();
	float_t	spo2;

	uint8_t datoLeidoAux[6];
	uint8_t indexR = 0, indexIR = 0;

	if (readPointer != writePointer)
	{

		_config.numberSamplesAvailable = writePointer - readPointer;
		if (_config.numberSamplesAvailable < 0) _config.numberSamplesAvailable += 32;

		for (uint8_t index = 0; index < _config.numberSamplesAvailable; index++)
		{

			_max30102._i2cReadFn ( I2C0, MAX_ADDRESS, FIFO_DATA_REGISTER, datoLeidoAux, 6 );

			_config.datoLeidoRed[indexR] = ((uint32_t)datoLeidoAux[0] << 16) | ((uint32_t)datoLeidoAux[1] << 8) | (uint32_t)datoLeidoAux[2];
			_config.datoLeidoRed[indexR] = 0x3FFFF & _config.datoLeidoRed[indexR];
			indexR++;

			_config.datoLeidoIr[indexIR] = ((uint32_t)datoLeidoAux[3] << 16) | ((uint32_t)datoLeidoAux[4] << 8) | ((uint32_t)datoLeidoAux[5]);
			_config.datoLeidoIr[indexIR] = 0x3FFFF & _config.datoLeidoIr[indexIR];
			indexIR++;

		}

		spo2 = max30102_oxygenSaturation ( _config.datoLeidoIr, _config.datoLeidoRed, _config.numberSamplesAvailable );

	} //FIN readPtr != writePtr
	max30102_clearFIFO();
	return spo2;
}


float_t max30102_oxygenSaturation (uint32_t * ledIr, uint32_t * ledR, int32_t numSamples )
{
	float_t spo2 = 0;
	float_t avgRed = 0, avgIr = 0;
	float_t rmsRed = 0, rmsIr = 0;
	float_t R;

	for (uint16_t indexSPO2 = 0; indexSPO2 < numSamples; indexSPO2++)
	{

		avgRed = avgRed + ledR[indexSPO2] ;
		avgIr = avgIr + ledIr[indexSPO2] ;

		rmsRed = rmsRed + ( ( ledR[indexSPO2] ) * ( ledR[indexSPO2] ) );
		rmsIr = rmsIr + ( ( ledIr[indexSPO2]) * ( ledIr[indexSPO2]) );

	}

	/* means */
	avgRed = avgRed / numSamples;
	avgIr = avgIr / numSamples;

	/* RMS */
	rmsRed = rmsRed / numSamples;
	rmsIr = rmsIr / numSamples;

	R = ( sqrt( rmsRed ) / avgRed ) / ( sqrt( rmsIr ) / avgIr );

	spo2 = 110 - 17*R;

	if ( spo2 >= 100 ) spo2 = 100;

	return spo2;
}

uint32_t max30102_hearBeat ( )
{
	uint32_t datoHeartBeat, counter = 0 ;
	uint32_t 	BPM = 0;
	uint8_t datoLeidoAux[6];

	while ( !delayRead( &beatTime ) )
	{
		_max30102._i2cReadFn ( I2C0, MAX_ADDRESS, FIFO_DATA_REGISTER, datoLeidoAux, 6 );

		datoHeartBeat = ((uint32_t)datoLeidoAux[0] << 16) | ((uint32_t)datoLeidoAux[1] << 8) | (uint32_t)datoLeidoAux[2];
		datoHeartBeat = 0x3FFFF & datoHeartBeat;

		if ( 115000 < datoHeartBeat )
		{
			//delay(1);
			counter++;
		}

	}
	if ( counter > 100 && counter < 1500 )
	{
		BPM = counter * 12 / 100;
	}else{
		BPM = 0;
	}

	return BPM;
}

void max30102_maskRegister ( uint8_t _register, uint8_t _mask, uint8_t bitConfig )
{

	uint8_t registerActual = 0;

	_max30102._i2cReadFn ( I2C0, MAX_ADDRESS, _register, &registerActual, 1 );

	/* Enmascaro los bits del registro */
	registerActual = registerActual & _mask;

	/* Escribo el registro con las máscara más el bit o bits a cambiar */
	_max30102._i2cWriteFn ( I2C0, MAX_ADDRESS, _register, registerActual | bitConfig );

}

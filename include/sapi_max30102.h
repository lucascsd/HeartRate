/*
 * sapi_max30102.h
 *
 *  Created on: 2 oct. 2021
 *      Author: lucascsd
 */

#ifndef SAPI_MAX30102_H_
#define SAPI_MAX30102_H_

/*
 * Libreria para el manejo del Pulsioximetro MAX30102 para la plataforma EDU-CIAA-NXP
 */
#include "i2c.h"

/* REGISTER MAPS */

/* STATUS REGISTER */
#define INT_STATUS_1          	((uint8_t)0x00)	/* Read only */
#define INT_STATUS_2          	((uint8_t)0x01)	/* Read only */
#define INT_ENABLE_1          	((uint8_t)0x02)	/* Read / Write */
#define INT_ENABLE_2          	((uint8_t)0x03)	/* Read / Write */

/* FIFO */
#define FIFO_WRITE_POINTER		((uint8_t)0x04)	/* Read / Write */
#define OVERFLOW_COUNTER		((uint8_t)0x05)	/* Read / Write */
#define FIFO_READ_POINTER		((uint8_t)0x06)	/* Read / Write */
#define FIFO_DATA_REGISTER		((uint8_t)0x07)	/* Read / Write */

/* CONFIGURATION */
#define FIFO_CONFIG				((uint8_t)0x08)	/* Read / Write */
#define MODE_CONFIG				((uint8_t)0x09)	/* Read / Write */
#define SPO2_CONFIG				((uint8_t)0x0A)	/* Read / Write */
#define LED_PULSE_AMP_1			((uint8_t)0x0C)	/* Read / Write */
#define LED_PULSE_AMP_2			((uint8_t)0x0D)	/* Read / Write */
#define LED_CTRL_REG_12			((uint8_t)0x11)	/* Read / Write */
#define LED_CTRL_REG_34			((uint8_t)0x12)	/* Read / Write */

/* TEMPERATURE */
#define TEMP_INTEGER			((uint8_t)0x1F)	/* Read only */
#define TEMP_FRACTION			((uint8_t)0x20)	/* Read only */
#define TEMP_CONFIG				((uint8_t)0x21)	/* Read / Write */

/* PART ID */
#define REVISION_ID         	((uint8_t)0xFE)	/* Read only */
#define PART_ID             	((uint8_t)0xFF)	/* Read only */

/************************************** REGISTERS VALUE *******************************************/

/* I2C SLAVE ID */
#define PART_ID_POR				((uint8_t)0x15)

/* Enable interrupts */
#define INT_ENB_A_FULL			((uint8_t)0x80)
#define INT_ENB_PPG_RDY			((uint8_t)0x40)
#define INT_ENB_ALC_OVF			((uint8_t)0x20)
#define INT_ENB_DIE_TEMP_RDY	((uint8_t)0x02)

/* Mode configuration */
#define MODE_SHDN				((uint8_t)0x80)
#define MODE_RESET				((uint8_t)0x40)
#define MODE_TEMP_EN			((uint8_t)0x01)
#define MODE_HR					((uint8_t)0x02)
#define MODE_SPO2				((uint8_t)0x03)
#define MODE_MULTI				((uint8_t)0x07)

/* SpO2 ADC Range Control (18-Bit Resolution) */
#define SPO2_ADC_RGE_2048 		((uint8_t)0x00)		/* 00000000 */
#define SPO2_ADC_RGE_4096 		((uint8_t)0x20)		/* 00100000 */
#define SPO2_ADC_RGE_8192 		((uint8_t)0x40)		/* 01000000 */
#define SPO2_ADC_RGE_16384 		((uint8_t)0x60)		/* 01100000 */

/* FIFO Configuration SMP_AVE[2:0] */
#define SMP_AVE_0				((uint8_t)0x00)		/* 000XXXXX = No averaging */
#define SMP_AVE_2				((uint8_t)0x20)		/* 001XXXXX = 2  */
#define SMP_AVE_4				((uint8_t)0x40)		/* 010XXXXX = 4  */
#define SMP_AVE_8				((uint8_t)0x60)		/* 011XXXXX = 8  */
#define SMP_AVE_16				((uint8_t)0x80)		/* 101XXXXX = 16 */
#define SMP_AVE_32				((uint8_t)0xA0)		/* 101XXXXX = 32 */
#define SMP_AVE_32A				((uint8_t)0xC0)		/* 110XXXXX = 32 */
#define SMP_AVE_MASK			((uint8_t)0xE0)		/* 111XXXXX = 32 */

/* Sample Rate values */
#define SAMPLERATE_50			((uint8_t)0x00)
#define SAMPLERATE_100			((uint8_t)0x04)
#define SAMPLERATE_200 			((uint8_t)0x08)
#define SAMPLERATE_400 			((uint8_t)0x0C)
#define SAMPLERATE_800 			((uint8_t)0x10)
#define SAMPLERATE_1000			((uint8_t)0x14)
#define SAMPLERATE_1600			((uint8_t)0x18)
#define SAMPLERATE_3200			((uint8_t)0x1C)

/* Pulse Width values */
#define PULSEWIDTH_69			((uint8_t)0x00)
#define PULSEWIDTH_118			((uint8_t)0x01)
#define PULSEWIDTH_215			((uint8_t)0x02)
#define PULSEWIDTH_411			((uint8_t)0x03)

// NOTE: Amplitude values: 0x00 = 0mA, 0x7F = 25.4mA, 0xFF = 50mA (typical)
#define PULSEAMPLITUDE_00		((uint8_t)0x00)
#define PULSEAMPLITUDE_06		((uint8_t)0x1F)
#define PULSEAMPLITUDE_12		((uint8_t)0x3F)
#define PULSEAMPLITUDE_25		((uint8_t)0x7F)
#define PULSEAMPLITUDE_50		((uint8_t)0xFF)

/* Multi-led values */
#define MULTILED_CONTROL_OFF	((uint8_t)0x00)
#define MULTILED_CONTROL_RED	((uint8_t)0x01)
#define MULTILED_CONTROL_IR		((uint8_t)0x02)
#define MULTILED_CONTROL_OFF1	((uint8_t)0x03)
#define MULTILED_CONTROL_OFF2	((uint8_t)0x04)

/* FIFO Rolls on Full */
#define FIFO_ROLLOVER_EN		((uint8_t)0x10)		/* Bit 4 = 1 */
#define FIFO_ROLLOVER_DE		((uint8_t)0x00)		/* Bit 4 = 0 */

/* I2C baudrate */
#define MAX30102_I2C_RATE_STD	100000 // 400 kHz
#define MAX30102_I2C_RATE_MAX	400000 // 400 kHz

#define	MAX_ADDRESS				((uint8_t)0x57)

/* typedef for struct */

typedef uint8_t registerAddr_t;
typedef uint8_t slave_address_t;
typedef uint8_t avgsamples_t;
typedef uint8_t rollOver_t;
typedef uint8_t ledMode_t;
typedef uint8_t spo2Mode_t;
typedef uint8_t srMode_t;
typedef uint8_t pwMode_t;
typedef uint8_t registerLed_t;

typedef bool_t ( *i2c_port_t )( i2cMap_t, uint32_t );
typedef bool_t ( *i2c_write_t )( i2cMap_t  i2cNumber, uint8_t  i2cSlaveAddress, uint8_t registerAddr, uint8_t data );
typedef bool_t ( *i2c_Read_t )( i2cMap_t  i2cNumber, uint8_t  i2cSlaveAddress, uint8_t registerAddr, uint8_t* receiveDataBuffer, uint16_t receiveDataBufferSize );
typedef uint32_t ( *readTimeFnc_t )( void );

typedef struct
{

	i2c_port_t		_i2cPortFn;
	i2c_write_t		_i2cWriteFn;
	i2c_Read_t		_i2cReadFn;
	readTimeFnc_t		_delay;

} max30102_t;

typedef struct{
	/* Tipos de datos para configuracion */
	avgsamples_t	_avg;
	rollOver_t		_rollOver;
	pwMode_t 		_pW;
	spo2Mode_t		_spo2;
	srMode_t		_sr;
	ledMode_t 		_ledC;
	registerLed_t	_regLed;

	uint32_t 		datoLeidoRed[32];
	uint32_t 		datoLeidoIr[32];
	int32_t 		numberSamplesAvailable;
	
	/* buffer para lectura y escritura de registros */
	uint8_t 		_buffer;

}max30102_config_t;

/* Variables del tipo tick para retardos */
delay_t 	beatTime;

/* Mis funciones */
void 	initStructMax30102( void );

/* Inicializar device */
bool_t	max30102_Init( max30102_t driver_config );
bool_t  max30102_setup ( max30102_config_t _configDevice );
bool_t	max30102_config ( uint8_t _register, uint8_t _param, uint8_t shitf );
bool_t 	max30102_reset 	( void );

/* Lectura de Part ID y Revision ID */
uint8_t max30102_readPartID 		( void );
uint8_t max30102_readRevisionID 	( void );

bool_t max30102_clearFIFO 			( max30102_t driver_config );
float_t max30102_readNewValue 		( void );
int16_t max30102_check 				( void );
uint8_t max30102_getWritePointer 	( void );
uint8_t max30102_getReadPointer 	( void );

float_t max30102_oxygenSaturation	( uint32_t * ledIr, uint32_t * ledR, int32_t numSamples );
uint32_t max30102_hearBeat 			( void );

/* Funcion para enmascarar registros */
void 	max30102_maskRegister ( uint8_t _register, uint8_t _mask, uint8_t bitMask );

#endif /* SAPI_MAX30102_H_ */

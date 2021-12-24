#include "unity.h"

#include "sapi_max30102.h"
#include "max30102_CIAA_port.h"

#include "mock_i2c.h"

/* TP4: TDD */

/*
 * Trabajo práctico 4. Testing unitario.
 * Seleccionar una biblioteca (o componente) del software a desarrollar en el TP final de la carrera de especialización / maestría 
 * y escribir un test unitario con ceedling que incluya por lo menos 5 test cases. 
 * El archivo de test debe tener por lo menos 65 líneas de extensión.
 * Se debe mantener bajo control de versión el código generado.
 * Entregar:
 * Repositorio con el archivo con las pruebas unitarias y el código de producción bajo prueba (preferentemente en bitbucket o github).
*/

/* Test a realizar:
 *
 * 1.- Iniciar comunicacion con dispositivo I2C - Max30102  Ok
 * 2.- Reset de la memoria FIFO y punteros de lectura y escritura Ok
 * 2.- Configurar dispostivo 
 * 3.- Enmascarar registros
 * 4.- Escritura de registros
 * 5.- Consulta de estado de los registros
 * 6.- Calculo Heartbear
 * 7.- Calculo Saturacion de oxigeno en sangre
 * 8.- Limpieza Memoria FIFO
 * 
 */

#define INIT_I2C_OK TRUE
#define CLEAR_MEM_I2C_OK TRUE

#define MAX_ADDRESS_I2C ((uint8_t)0x57)

max30102_t driver_max30102;
float_t spo2Data;
uint32_t BPM;
bool_t max30102_status;

/* Bloque de Set up: Se fijan las condiciones iniciales  */
void setUp(void)
{

    /* Inicializo driver para lectura de MAX30102 */
    driver_max30102._i2cPortFn = i2cInit_CIAA_port;
    driver_max30102._i2cWriteFn = max30102Write_CIAA_port;
    driver_max30102._i2cReadFn = max30102Read_CIAA_port;
    driver_max30102._delay = delay_CIAA_port;
}

/*************************************************************/
/* Init I2C - Se inicializa comunicación con dispositivo I2C */
/* A la funcion i2cInit_CIAA_port se le pasa como argumento  */
/* puerto y velocidad de transmision.                        */
/* Return TRUE si se conecto correctamente el dispositivo    */
/*************************************************************/
void test_init_i2c(void)
{

    i2cInit_IgnoreAndReturn(TRUE);

    TEST_ASSERT_EQUAL(INIT_I2C_OK, driver_max30102._i2cPortFn(I2C0, MAX30102_I2C_RATE_STD));
}

void test_clear_fifo_i2c(void)
{

    bool_t stateReset = FALSE;
    bool_t statePointerFifo = FALSE;

    uint8_t transmitDataBufferToWritePointer[2];
    uint8_t transmitDataBufferToOverflowCounter[2];
    uint8_t transmitDataBufferToReadPointer[2];

	transmitDataBufferToWritePointer[0] = FIFO_WRITE_POINTER;
	transmitDataBufferToWritePointer[1] = 0x00;

    transmitDataBufferToOverflowCounter[0] = OVERFLOW_COUNTER;
	transmitDataBufferToOverflowCounter[1] = 0x00;

    transmitDataBufferToReadPointer[0] = FIFO_READ_POINTER;
	transmitDataBufferToReadPointer[1] = 0x00;

    i2cWrite_ExpectAndReturn ( I2C0, MAX_ADDRESS, transmitDataBufferToWritePointer, 2, TRUE, TRUE );

    i2cWrite_ExpectAndReturn ( I2C0, MAX_ADDRESS, transmitDataBufferToOverflowCounter, 2, TRUE, TRUE );

    i2cWrite_ExpectAndReturn ( I2C0, MAX_ADDRESS, transmitDataBufferToReadPointer, 2, TRUE, TRUE );

    stateReset = max30102_clearFIFO(driver_max30102);

    printf("Clear FIFO %d", stateReset);

    TEST_ASSERT_EQUAL(CLEAR_MEM_I2C_OK, stateReset);
}

// void test_mask_register ( void )
// {
//     uint8_t registerActual;
//     uint16_t receiveDataBufferSize = 1;
    
//     uint8_t _register = FIFO_CONFIG;
//     uint8_t _mask = SMP_AVE_8; 
//     uint8_t bitConfig = 0;

//     i2cRead_ExpectAndReturn ( I2C0, MAX_ADDRESS, &_register, 1, TRUE, &registerActual, receiveDataBufferSize, TRUE, TRUE );

//      uint8_t transmitDataRegister[2];

// 	transmitDataRegister[0] = FIFO_WRITE_POINTER;
// 	transmitDataRegister[1] = registerActual & _mask;

//     i2cWrite_ExpectAndReturn ( I2C0, MAX_ADDRESS, transmitDataRegister, 2, TRUE, TRUE );

//     max30102_maskRegister ( _register, _mask, bitConfig );
// }

void test_calculate_oxygen_saturation ( void ) 
{

    uint32_t ledIr[] = { 
                        0x00016F94, 0x0001737C, 0x00016FFF, 0x00017000, 
                        0x00016F94, 0x0001737C, 0x00016FFF, 0x00017000, 
                        0x00016F94, 0x0001737C }; 
    /* Suma total IR = 0x000CF92E */
    /* Promedio 0x000CF92E / 10 = 14C1E ( 85022 decimal) */

    uint32_t ledR[] = { 0x00014244, 0x00013F24, 0x00014000, 0x000135FA,
                        0x00014244, 0x00013F24, 0x00014000, 0x000135FA,
                        0x00014244, 0x00013F24 };
    /* Suma total IR = 0x000C702C */
    /* Promedio 0x000C702C / 10 = 13E6A ( 81514 decimal) */

    int32_t numSamples = 10;

    float_t tolerance = 0.1f;
    float_t expectedResult = 89.5;

    sqrt_IgnoreAndReturn( 85022 );
    sqrt_IgnoreAndReturn( 81514 );

    float_t Result = max30102_oxygenSaturation ( ledIr, ledR, numSamples );

    printf( "SO2 %f", Result );

    TEST_ASSERT_FLOAT_WITHIN ( tolerance, expectedResult, Result );

}

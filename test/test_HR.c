#include "unity.h"

#include "sapi_max30102.h"
#include "max30102_CIAA_port.h"

#include "mock_i2c.h"

/* TP4: TDD */

/* Test a realizar:
 *
 * 1.- Iniciar comunicacion con dispositivo I2C - Max30102          OK
 * 2.- Reset de la memoria FIFO y punteros de lectura y escritura   OK
 * 3.- Calculo Saturacion de oxigeno en sangre                      OK
 * 4.- Configuracion de algun parametro del dispositivo I2C         OK
 * 
 */

#define INIT_I2C_OK TRUE
#define CONFIG_I2C_OK TRUE
#define CLEAR_MEM_I2C_OK TRUE

#define MAX_PART_ID_I2C ((uint8_t)0x57)

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

/*************************************************************/
/* Confi I2C - Se configura el dispositivo I2C con la funcion*/
/* max30102_config se le pasa como argumento puerto,         */
/* direccion de registro, configuracion del registro y mask 0*/
/* Return TRUE si se configuro correctamente el dispositivo  */
/*************************************************************/
void test_config_sample_average_nbits(void)
{

    i2cRead_IgnoreAndReturn ( TRUE );

    i2cRead_IgnoreAndReturn ( TRUE );
    i2cWrite_IgnoreAndReturn ( TRUE );

    bool_t stateConfig = max30102_config ( driver_max30102, FIFO_CONFIG, SMP_AVE_8, 0 );

    TEST_ASSERT_EQUAL ( CONFIG_I2C_OK, stateConfig );

}

/***************************************************************/
/* Clear fifo I2C - Se limpia memoria fifo del dispositivo I2C */
/* A la funcion max30102_clearFIFO se le pasa como argumento   */
/* la instancia del puerto i2C                                 */
/* Return TRUE si se limpio correctamente la memoria FIFO      */
/***************************************************************/
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

    i2cWrite_ExpectAndReturn(I2C0, MAX_ADDRESS, transmitDataBufferToWritePointer, 2, TRUE, TRUE);

    i2cWrite_ExpectAndReturn(I2C0, MAX_ADDRESS, transmitDataBufferToOverflowCounter, 2, TRUE, TRUE);

    i2cWrite_ExpectAndReturn(I2C0, MAX_ADDRESS, transmitDataBufferToReadPointer, 2, TRUE, TRUE);

    stateReset = max30102_clearFIFO(driver_max30102);

    printf("Clear FIFO %d", stateReset);

    TEST_ASSERT_EQUAL(CLEAR_MEM_I2C_OK, stateReset);
}

/***************************************************************/
/* Calculo de Saturacion de oxigeno                            */
/* Con la funcion max30102_oxygenSaturation pasa como argumento*/
/* lecturas de Led IR, Red y cantidad de lecturas              */
/* Return El resultado del calculo de SO2                      */
/***************************************************************/
void test_calculate_oxygen_saturation(void)
{

    uint32_t ledIr[] = {
        0x00016F94, 0x0001737C, 0x00016FFF, 0x00017000,
        0x00016F94, 0x0001737C, 0x00016FFF, 0x00017000,
        0x00016F94, 0x0001737C};
    /* Suma total IR = 0x000CF92E */
    /* Promedio 0x000CF92E / 10 = 14C1E ( 85022 decimal) */

    uint32_t ledR[] = {0x00014244, 0x00013F24, 0x00014000, 0x000135FA,
                       0x00014244, 0x00013F24, 0x00014000, 0x000135FA,
                       0x00014244, 0x00013F24};
    /* Suma total IR = 0x000C702C */
    /* Promedio 0x000C702C / 10 = 13E6A ( 81514 decimal) */

    int32_t numSamples = 10;

    float_t tolerance = 0.1f;
    float_t expectedResult = 89.5;

    sqrt_IgnoreAndReturn(85022);
    sqrt_IgnoreAndReturn(81514);

    float_t Result = max30102_oxygenSaturation(ledIr, ledR, numSamples);

    printf("SO2 %f", Result);

    TEST_ASSERT_FLOAT_WITHIN(tolerance, expectedResult, Result);
}

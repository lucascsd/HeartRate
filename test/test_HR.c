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

#define INIT_I2C_OK 0xFF

max30102_t driver_max30102;
float_t spo2Data;
uint32_t BPM;
bool_t max30102_status;



/* Bloque de Set up: Se fijan las condiciones iniciales  */
void setUp(void)
{
    max30102_status = max30102_Init( driver_max30102 );
}

void test_init_i2c(void)
{
    
}


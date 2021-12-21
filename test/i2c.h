#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <stdbool.h>

//==================[macros]===================================================

// Functional states
#ifndef OFF
#define OFF    0
#endif
#ifndef ON
#define ON     (!OFF)
#endif

// Electrical states
#ifndef LOW
#define LOW    0
#endif
#ifndef HIGH
#define HIGH   (!LOW)
#endif

// Logical states

#ifndef FALSE
#define FALSE  0
#endif
#ifndef TRUE
#define TRUE   (!FALSE)
#endif

#ifndef false
#define false  0
#endif
#ifndef true
#define true   (!false)
#endif

//==================[typedef]==================================================

// Define Boolean Data Type
typedef uint8_t bool_t;

typedef float  float_t;

typedef uint8_t i2cMap_t;

/* Define Tick Data Type */
typedef uint64_t tick_t;

typedef struct{
   tick_t startTime;
   tick_t duration;
   bool_t running;
} delay_t;

/**
 * @brief	I2C Slave Identifiers
 */
typedef enum {
	I2C_SLAVE_GENERAL,	/**< Slave ID for general calls */
	I2C_SLAVE_0,		/**< Slave ID fo Slave Address 0 */
	I2C_SLAVE_1,		/**< Slave ID fo Slave Address 1 */
	I2C_SLAVE_2,		/**< Slave ID fo Slave Address 2 */
	I2C_SLAVE_3,		/**< Slave ID fo Slave Address 3 */
	I2C_SLAVE_NUM_INTERFACE	/**< Number of slave interfaces */
} I2C_SLAVE_ID;

/**
 * @brief	I2C transfer status
 */
typedef enum {
	I2C_STATUS_DONE,	/**< Transfer done successfully */
	I2C_STATUS_NAK,		/**< NAK received during transfer */
	I2C_STATUS_ARBLOST,	/**< Aribitration lost during transfer */
	I2C_STATUS_BUSERR,	/**< Bus error in I2C transfer */
	I2C_STATUS_BUSY,	/**< I2C is busy doing transfer */
	I2C_STATUS_SLAVENAK,/**< NAK received after SLA+W or SLA+R */
} I2C_STATUS_T;

/**
 * @brief Master transfer data structure definitions
 */
typedef struct {
	uint8_t slaveAddr;		/**< 7-bit I2C Slave address */
	const uint8_t *txBuff;	/**< Pointer to array of bytes to be transmitted */
	int     txSz;			/**< Number of bytes in transmit array,
							   if 0 only receive transfer will be carried on */
	uint8_t *rxBuff;		/**< Pointer memory where bytes received from I2C be stored */
	int     rxSz;			/**< Number of bytes to received,
							   if 0 only transmission we be carried on */
	I2C_STATUS_T status;	/**< Status of the current I2C transfer */
} I2C_XFER_T;

/**
 * @brief	I2C interface IDs
 * @note
 * All Chip functions will take this as the first parameter,
 * I2C_NUM_INTERFACE must never be used for calling any Chip
 * functions, it is only used to find the number of interfaces
 * available in the Chip.
 */
typedef enum I2C_ID {
	I2C0,				/**< ID I2C0 */
	I2C1,				/**< ID I2C1 */
	I2C_NUM_INTERFACE	/**< Number of I2C interfaces in the chip */
} I2C_ID_T;

/**
 * @brief	I2C master events
 */
typedef enum {
	I2C_EVENT_WAIT = 1,	/**< I2C Wait event */
	I2C_EVENT_DONE,		/**< Done event that wakes up Wait event */
	I2C_EVENT_LOCK,		/**< Re-entrency lock event for I2C transfer */
	I2C_EVENT_UNLOCK,	/**< Re-entrency unlock event for I2C transfer */
	I2C_EVENT_SLAVE_RX,	/**< Slave receive event */
	I2C_EVENT_SLAVE_TX,	/**< Slave transmit event */
} I2C_EVENT_T;

/*==================[external functions declaration]=========================*/

bool_t i2cInit( i2cMap_t i2cNumber, uint32_t clockRateHz );

bool_t i2cRead( i2cMap_t  i2cNumber,
                uint8_t  i2cSlaveAddress,
                uint8_t* dataToReadBuffer,
                uint16_t dataToReadBufferSize,
                bool_t   sendWriteStop,
                uint8_t* receiveDataBuffer,
                uint16_t receiveDataBufferSize,
                bool_t   sendReadStop );

bool_t i2cWrite( i2cMap_t i2cNumber,
                 uint8_t  i2cSlaveAddress,
                 uint8_t* transmitDataBuffer,
                 uint16_t transmitDataBufferSize,
                 bool_t   sendWriteStop );

bool_t i2cWriteRead( i2cMap_t i2cNumber,
                     uint8_t  i2cSlaveAddress,
                     uint8_t* dataToReadBuffer,
                     uint16_t dataToReadBufferSize,
                     bool_t   sendWriteStop,
                     uint8_t* receiveDataBuffer,
                     uint16_t receiveDataBufferSize,
                     bool_t   sendReadStop );

bool_t delayRead( delay_t * delay );

tick_t tickRead( void );

double sqrt (double num);

#endif /* I2C_H_ */
#ifndef _EEPROM_H
#define	_EEPROM_H
#include "main.h"
#include "i2c.h"
#include <stdio.h>
#define I2C_X           hi2c3
#define EEPROM_ADDRESS          0XA0

extern uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr,uint8_t NumByteToWrite);
extern uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
extern uint8_t I2C_Test(void);
extern uint8_t I2C_ReadData(void);
#endif
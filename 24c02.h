#ifndef __24C02_H
#define __24C02_H

#include "main.h"

uint8_t X24C02_ReadOneByte(uint8_t address);
void X24C02_WriteOneByte(uint8_t address,uint8_t info);
void X24C02_WriteLenByte(uint8_t WriteAddr,uint32_t DataToWrite,uint8_t Len);
uint32_t X24CXX_ReadLenByte(uint8_t ReadAddr,uint8_t Len);
void X24CXX_WriteArray(uint8_t WriteAddr,uint8_t *pBuffer,uint8_t NumToWrite);
void X24CXX_ReadArray(uint8_t ReadAddr,uint8_t *pBuffer,uint8_t NumToRead);

void X24C02_Test(void);

#endif

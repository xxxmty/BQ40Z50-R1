#ifndef __BQ4050_H
#define __BQ4050_H

#include "main.h"

void bq40z50_Init(void);
void bq40z50_Get_Info_Vol(void);
void bq40z50_Get_Info_Cap(void);
void bq40z50_Get_Info_Name(void);
uint8_t bq40z50_Get_Data(uint8_t address,char* buff);


extern int DianChi_DianYa;
extern int DianChi_DianLiang;

#endif

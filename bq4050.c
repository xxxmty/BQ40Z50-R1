#include "bq4050.h"
#include "myiic.h"

int DianChi_DianYa;
int DianChi_DianLiang;
 
 
//模拟IIC初始化
void bq40z50_Init(void)
{
     I2CInit();
}
//获取多个电池信息
void bq40z50_Get_Info_Vol(void)
{
    char DianChi_ReadData[2] = {0, 0};
    if(bq40z50_Get_Data(0x09,DianChi_ReadData) == 0)//读取电池电压
    {
        if((DianChi_ReadData[1]*256 + DianChi_ReadData[0]) < 15000)//有时第二个字节会读到FF，还没弄清原因
            DianChi_DianYa = DianChi_ReadData[1]*256 + DianChi_ReadData[0];//DianChi_DianYa为8150，说明电池电压为8.15V
    }
    HAL_Delay_us(100);
    /*if(bq40z50_Get_Data(0x0D,DianChi_ReadData) == 0)//读取电池电量
    {
        if(DianChi_ReadData[0] > 0 && DianChi_ReadData[0] < 101)
            DianChi_DianLiang = DianChi_ReadData[0];
    }*/
}

void bq40z50_Get_Info_Cap(void){
    char DianChi_ReadData[2] = {0, 0};
    if(bq40z50_Get_Data(0x0D,DianChi_ReadData) == 0)//读取电池电量
    {
        if(DianChi_ReadData[0] > 0 && DianChi_ReadData[0] < 101)
            DianChi_DianLiang = DianChi_ReadData[0];
    }
    HAL_Delay_us(100);
    
}

void bq40z50_Get_Info_Name(void){
    char Name_ReadData[20] = {0};
    I2CStart();  
    I2CSendByte(0x16);//发送地址
    if(I2CWaitAck() == 1)//若设备正常，BQ40Z50此时会将SDA拉低
    {
        //未应答
        return;
    }
    I2CSendByte(0x21);    //根据不同的address读取不同的数据，例如电压、电流、剩余电量
    HAL_Delay_us(100);
    if(I2CWaitAck() == 1)//若设备正常，BQ40Z50此时会将SDA拉低
    {
        //未应答
        return;
    }

    I2CStart();
    
    I2CSendByte(0x17);
    if(I2CWaitAck() == 1)//若设备正常，BQ40Z50此时会将SDA拉低
    {
        //未应答
        return;
    }
    HAL_Delay_us(100);
    
    for(int i = 0; i < 20; i++){
      Name_ReadData[i] = I2CReceiveByte();
      HAL_Delay_us(10);
    }
    
    HAL_Delay_us(10);
    
    I2CStop();
    return;
    
}
//获取某一个电池信息
//电压address = 0x09 电量address = 0x0D
uint8_t bq40z50_Get_Data(uint8_t address,char* buff)
{
    I2CStart();  
    I2CSendByte(0x16);//发送地址
    if(I2CWaitAck() == 1)//若设备正常，BQ40Z50此时会将SDA拉低
    {
        //未应答
        return 1;
    }
    I2CSendByte(address);    //根据不同的address读取不同的数据，例如电压、电流、剩余电量
    HAL_Delay_us(100);
    if(I2CWaitAck() == 1)//若设备正常，BQ40Z50此时会将SDA拉低
    {
        //未应答
        return 1;
    }
    I2CStart();
    
    I2CSendByte(0x17);
    if(I2CWaitAck() == 1)//若设备正常，BQ40Z50此时会将SDA拉低
    {
        //未应答
        return 1;
    }
    HAL_Delay_us(100);
    
    *(buff+0) = I2CReceiveByte();
    HAL_Delay_us(10);
    
    *(buff+1) = I2CReceiveByte();
    HAL_Delay_us(10);
    
    I2CStop();
    return 0;
}


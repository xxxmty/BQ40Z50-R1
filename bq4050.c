#include "bq4050.h"
#include "myiic.h"

int DianChi_DianYa;
int DianChi_DianLiang;
 
 
//ģ��IIC��ʼ��
void bq40z50_Init(void)
{
     I2CInit();
}
//��ȡ��������Ϣ
void bq40z50_Get_Info_Vol(void)
{
    char DianChi_ReadData[2] = {0, 0};
    if(bq40z50_Get_Data(0x09,DianChi_ReadData) == 0)//��ȡ��ص�ѹ
    {
        if((DianChi_ReadData[1]*256 + DianChi_ReadData[0]) < 15000)//��ʱ�ڶ����ֽڻ����FF����ûŪ��ԭ��
            DianChi_DianYa = DianChi_ReadData[1]*256 + DianChi_ReadData[0];//DianChi_DianYaΪ8150��˵����ص�ѹΪ8.15V
    }
    HAL_Delay_us(100);
    /*if(bq40z50_Get_Data(0x0D,DianChi_ReadData) == 0)//��ȡ��ص���
    {
        if(DianChi_ReadData[0] > 0 && DianChi_ReadData[0] < 101)
            DianChi_DianLiang = DianChi_ReadData[0];
    }*/
}

void bq40z50_Get_Info_Cap(void){
    char DianChi_ReadData[2] = {0, 0};
    if(bq40z50_Get_Data(0x0D,DianChi_ReadData) == 0)//��ȡ��ص���
    {
        if(DianChi_ReadData[0] > 0 && DianChi_ReadData[0] < 101)
            DianChi_DianLiang = DianChi_ReadData[0];
    }
    HAL_Delay_us(100);
    
}

void bq40z50_Get_Info_Name(void){
    char Name_ReadData[20] = {0};
    I2CStart();  
    I2CSendByte(0x16);//���͵�ַ
    if(I2CWaitAck() == 1)//���豸������BQ40Z50��ʱ�ὫSDA����
    {
        //δӦ��
        return;
    }
    I2CSendByte(0x21);    //���ݲ�ͬ��address��ȡ��ͬ�����ݣ������ѹ��������ʣ�����
    HAL_Delay_us(100);
    if(I2CWaitAck() == 1)//���豸������BQ40Z50��ʱ�ὫSDA����
    {
        //δӦ��
        return;
    }

    I2CStart();
    
    I2CSendByte(0x17);
    if(I2CWaitAck() == 1)//���豸������BQ40Z50��ʱ�ὫSDA����
    {
        //δӦ��
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
//��ȡĳһ�������Ϣ
//��ѹaddress = 0x09 ����address = 0x0D
uint8_t bq40z50_Get_Data(uint8_t address,char* buff)
{
    I2CStart();  
    I2CSendByte(0x16);//���͵�ַ
    if(I2CWaitAck() == 1)//���豸������BQ40Z50��ʱ�ὫSDA����
    {
        //δӦ��
        return 1;
    }
    I2CSendByte(address);    //���ݲ�ͬ��address��ȡ��ͬ�����ݣ������ѹ��������ʣ�����
    HAL_Delay_us(100);
    if(I2CWaitAck() == 1)//���豸������BQ40Z50��ʱ�ὫSDA����
    {
        //δӦ��
        return 1;
    }
    I2CStart();
    
    I2CSendByte(0x17);
    if(I2CWaitAck() == 1)//���豸������BQ40Z50��ʱ�ὫSDA����
    {
        //δӦ��
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


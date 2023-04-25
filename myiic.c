#include "myiic.h"

#define DELAY_TIME	118

void HAL_Delay_us(uint32_t us)
{       
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);
    HAL_Delay(us-1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
}


/**
  * @brief SDA线输入模式配置
  * @param None
  * @retval None
  */
void SDA_Input_Mode()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出模式配置
  * @param None
  * @retval None
  */
void SDA_Output_Mode_OD()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDA_Output_Mode_PP()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief SDA线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SDA_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_11;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);
    }
}

/**
  * @brief SCL线输出一个位
  * @param val 输出的数据
  * @retval None
  */
void SCL_Output( uint16_t val )
{
    if ( val )
    {
        GPIOB->BSRR |= GPIO_PIN_10;
        uint32_t timeout = 1000; 
        while(timeout && (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) != GPIO_PIN_SET)) timeout--;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
    }
}

/**
  * @brief SDA输入一位
  * @param None
  * @retval GPIO读入一位
  */
uint8_t SDA_Input(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET){
		return 1;
	}else{
		return 0;
	}
}


/**
  * @brief I2C的短暂延时
  * @param None
  * @retval None
  */
static void delay1(unsigned int n)
{
    uint32_t i;
    for ( i = 0; i < n; ++i);
}

/**
  * @brief I2C起始信号
  * @param None
  * @retval None
  */
void I2CStart(void)
{
    SDA_Output_Mode_PP();
    SDA_Output(1);
    SCL_Output(1);
    HAL_Delay_us(9);
    SDA_Output(0);
    HAL_Delay_us(9);
    SCL_Output(0);
    HAL_Delay_us(5);
}

/**
  * @brief I2C结束信号
  * @param None
  * @retval None
  */
void I2CStop(void)
{   
    SDA_Output_Mode_PP();
    SDA_Output(0);
    HAL_Delay_us(10);
    SCL_Output(1);
    HAL_Delay_us(10);
    SDA_Output(1);
    HAL_Delay_us(10);
}

/**
  * @brief I2C等待确认信号
  * @param None
  * @retval None
  */
uint8_t I2CWaitAck(void)
{
    uint16_t ucErrTime=0;
    SDA_Output_Mode_OD();
    SDA_Output(1);
    HAL_Delay_us(20);
    SDA_Input_Mode();
    SCL_Output(1);
    HAL_Delay_us(9);  
    while(SDA_Input())
    {
        ucErrTime++;
        if(ucErrTime>500)
        {
            I2CStop();
            return 1;
        }
    }
    SCL_Output(0);  
    return 0; 
}

/**
  * @brief I2C发送确认信号
  * @param None
  * @retval None
  */
void I2CSendAck(void)
{
    SDA_Output_Mode_PP();
    SDA_Output(0);
    HAL_Delay_us(19);
    SCL_Output(1);
    HAL_Delay_us(19);
    SCL_Output(0);
    HAL_Delay_us(3);
    SDA_Output(1);
}

/**
  * @brief I2C发送非确认信号
  * @param None
  * @retval None
  */
void I2CSendNotAck(void)
{
    SDA_Output_Mode_PP();
    SDA_Output(1);
    HAL_Delay_us(9);
    SCL_Output(1);
    HAL_Delay_us(9);
    SCL_Output(0);
}

/**
  * @brief I2C发送一个字节
  * @param cSendByte 需要发送的字节
  * @retval None
  */
void I2CSendByte(unsigned char cSendByte)
{
    SDA_Output_Mode_PP();
    unsigned char  i = 8;
    SCL_Output(0);
    while (i--)
    {
        SDA_Output(cSendByte & 0x80);
        cSendByte += cSendByte;
        HAL_Delay_us(20);
        SCL_Output(1);
        HAL_Delay_us(20);
        SCL_Output(0);
    }
    HAL_Delay_us(20);
}

/**
  * @brief I2C接收一个字节
  * @param None
  * @retval 接收到的字节
  */
unsigned char I2CReceiveByte(void)
{

    unsigned char i = 8;
    unsigned char cR_Byte = 0;
    SDA_Input_Mode();
    HAL_Delay_us(40);
    while (i > 0)
    {
        cR_Byte += cR_Byte;
        SCL_Output(0);
        HAL_Delay_us(40);
        SCL_Output(1); 
        cR_Byte |=  SDA_Input();
        HAL_Delay_us(40);
        i--;
    }
    SCL_Output(0);
    HAL_Delay_us(40);
    I2CSendAck();
    return cR_Byte;
}

/**
  * @brief I2C对应IO口初始化
  * @param None
  * @retval None
  */
void I2CInit(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = GPIO_PIN_11 | GPIO_PIN_10;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}


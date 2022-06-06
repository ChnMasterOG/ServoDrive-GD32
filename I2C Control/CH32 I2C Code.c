#include "stdio.h"
#include "ch32v10x.h"

//------------- I2C1 GPIO 配置 -------------
static void I2C1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruce;   					//定义GPIO初始化结构体变量
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	//使能GPIOB、重映射 RCC时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);		//使能I2C1的RCC时钟

    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);   		//使能I2C1的重映射功能

    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_AF_OD;    		//复用开漏输出
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_10MHz;			//IO速率选择
    GPIO_InitStruce.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;		//设置引脚8和引脚9
    GPIO_Init(GPIOB, &GPIO_InitStruce);
}

//------------- I2C1 模式 配置 -------------
static void I2C1_Mode_Config(void)
{
    I2C_InitTypeDef I2C_InitStruce;                 //定义IIC1初始化结构体变量

    I2C_InitStruce.I2C_Mode = I2C_Mode_I2C;         //I2C模式运行
    I2C_InitStruce.I2C_ClockSpeed = 100000;         //SCL时钟100KHz
    I2C_InitStruce.I2C_OwnAddress1 = 0x52;          //主机地址0x52，不与连接的设备冲突即可
    I2C_InitStruce.I2C_Ack = I2C_Ack_Enable;        //使能主机应答
    I2C_InitStruce.I2C_DutyCycle = I2C_DutyCycle_2; //时钟线高低电平占空比为2:1
    I2C_InitStruce.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //主机7位寻址
    I2C_Init(I2C1, &I2C_InitStruce);                //初始化I2C1的工作参数
}

//------------- 舵机驱动控制 -------------
//--------- txdata: 写的数据指针 ---------
//--------- rxdata: 读的数据指针 ---------
//-------- i2cAddr: i2c地址(7bit) --------
void ServoDrive_Control(u8* txdata, u8* rxdata, u8 i2cAddr)
{
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, i2cAddr, I2C_Direction_Transmitter );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    I2C_SendData( I2C1, txdata[0] );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) );
    I2C_SendData( I2C1, txdata[1] );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) );
    I2C_SendData( I2C1, txdata[2] );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) );
    I2C_SendData( I2C1, txdata[3] );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) );
    I2C_SendData( I2C1, txdata[4] );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
    I2C_GenerateSTOP( I2C1, ENABLE );

    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    I2C_GenerateSTART( I2C1, ENABLE );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, i2cAddr, I2C_Direction_Receiver );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_RXNE ) ==  RESET ) I2C_AcknowledgeConfig( I2C1, ENABLE );

    rxdata[0] = I2C_ReceiveData( I2C1 );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED ) );
    I2C_AcknowledgeConfig( I2C1, ENABLE);
    rxdata[1] = I2C_ReceiveData( I2C1 );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED ) );
    I2C_AcknowledgeConfig( I2C1, ENABLE);
    rxdata[2] = I2C_ReceiveData( I2C1 );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED ) );
    I2C_AcknowledgeConfig( I2C1, ENABLE);
    rxdata[3] = I2C_ReceiveData( I2C1 );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED ) );
    I2C_AcknowledgeConfig( I2C1, ENABLE);
    rxdata[4] = I2C_ReceiveData( I2C1 );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED ) );
    I2C_AcknowledgeConfig( I2C1, DISABLE);
    I2C_GenerateSTOP( I2C1, ENABLE );
}

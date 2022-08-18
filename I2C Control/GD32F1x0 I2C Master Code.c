#include "gd32f1x0.h"

#define I2C1_SPEED              				100000
#define I2C1_SLAVE_ADDRESS7     				0x52

//------------- I2C1 GPIO 配置 -------------
static void I2C1_GPIO_Config(void)
{
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);

    /* connect PA0 to I2C1_SCL */
    gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_0);
    /* connect PA1 to I2C1_SDA */
    gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_1);
    /* configure GPIO pins of I2C1 */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

//------------- I2C1 模式 配置 -------------
static void I2C1_Mode_Config(void)
{
    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);
    /* configure I2C1 clock */
    i2c_clock_config(I2C1, I2C1_SPEED, I2C_DTCY_2);
    /* configure I2C1 address */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_SLAVE_ADDRESS7);
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
}

//------------- 舵机驱动控制 -------------
//--------- txdata: 写的数据指针 ---------
//--------- rxdata: 读的数据指针 ---------
//-------- i2cAddr: i2c地址(7bit) --------
void ServoDrive_Control(uint8_t* txdata, uint8_t* rxdata, uint8_t i2cAddr)
{
    while( i2c_flag_get( I2C1, I2C_FLAG_I2CBSY ) != RESET );
    i2c_start_on_bus( I2C1 );
		while( !i2c_flag_get(I2C1, I2C_FLAG_SBSEND) );
    i2c_master_addressing( I2C1, i2cAddr, I2C_TRANSMITTER );
    while( !i2c_flag_get(I2C1, I2C_FLAG_ADDSEND) );
		i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);

    i2c_data_transmit( I2C1, txdata[0] );
		while( !i2c_flag_get(I2C1, I2C_FLAG_TBE) );
    i2c_data_transmit( I2C1, txdata[1] );
		while( !i2c_flag_get(I2C1, I2C_FLAG_TBE) );
    i2c_data_transmit( I2C1, txdata[2] );
		while( !i2c_flag_get(I2C1, I2C_FLAG_TBE) );
    i2c_data_transmit( I2C1, txdata[3] );
		while( !i2c_flag_get(I2C1, I2C_FLAG_TBE) );
    i2c_data_transmit( I2C1, txdata[4] );
		while( !i2c_flag_get(I2C1, I2C_FLAG_TBE) );
    i2c_stop_on_bus( I2C1 );

    while( i2c_flag_get( I2C1, I2C_FLAG_I2CBSY ) != RESET );
    i2c_start_on_bus( I2C1 );
		while( !i2c_flag_get(I2C1, I2C_FLAG_SBSEND) );
    i2c_master_addressing( I2C1, i2cAddr, I2C_RECEIVER );
    while( !i2c_flag_get(I2C1, I2C_FLAG_ADDSEND) );
		i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);
		while( !i2c_flag_get( I2C1, I2C_FLAG_RBNE ) ) {
			i2c_ack_config( I2C1, I2C_ACK_ENABLE );
    }
    rxdata[0] = i2c_data_receive( I2C1 );
    while( !i2c_flag_get( I2C1, I2C_FLAG_RBNE ) );
    rxdata[1] = i2c_data_receive( I2C1 );
    while( !i2c_flag_get( I2C1, I2C_FLAG_RBNE ) );
    rxdata[2] = i2c_data_receive( I2C1 );
    while( !i2c_flag_get( I2C1, I2C_FLAG_RBNE ) );
    rxdata[3] = i2c_data_receive( I2C1 );
    while( !i2c_flag_get( I2C1, I2C_FLAG_RBNE ) );
    rxdata[4] = i2c_data_receive( I2C1 );
    while( !i2c_flag_get( I2C1, I2C_FLAG_RBNE ) );
		i2c_ack_config( I2C1, I2C_ACK_DISABLE );
    i2c_stop_on_bus( I2C1 );
}


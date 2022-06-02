/*!
    \file    i2c.c
    \brief   GD32 ServoDrive I2C
*/

#include "i2c.h"

uint8_t i2cDataRx[8];
uint8_t i2cDataTx[8];

/*!
    \brief      configure the I2C1 interfaces
    \param[in]  _id: slave address 
    \param[out] none
    \retval     none
*/
void i2c_config(uint32_t _id)
{
	/* configure I2C clock */
	i2c_clock_config(I2C0, I2C_SPEED, I2C_DTCY_2);
	/* configure I2C address */
	i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, _id);
	/* enable I2C0 */
	i2c_enable(I2C0);
	/* enable acknowledge */
	i2c_ack_config(I2C0, I2C_ACK_ENABLE);
	
	/* I2C0 interrupt Init */
	nvic_irq_enable(I2C0_EV_IRQn, 0, 1);
	i2c_interrupt_enable(I2C0, I2C_INT_EV);
	i2c_interrupt_enable(I2C0, I2C_INT_ERR);
	i2c_interrupt_enable(I2C0, I2C_INT_BUF);
}

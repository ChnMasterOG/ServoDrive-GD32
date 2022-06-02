/*!
    \file    rcu.c
    \brief   GD32 ServoDrive RCU
*/

#include "rcu.h"

void rcu_config(void)
{
  /* GPIO clock enable */
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOF);
	
	/* TIMER clock enable */
  rcu_periph_clock_enable(RCU_TIMER2);
	rcu_periph_clock_enable(RCU_TIMER13);
	
	/* I2C1 clock enable */
	rcu_periph_clock_enable(RCU_I2C0);
	
	/* DMA1 clock enable */
	rcu_periph_clock_enable(RCU_DMA);

	/* ADC1 clock enable */
	rcu_periph_clock_enable(RCU_ADC);
	/* ADCCLK = PCLK2/6 */
	rcu_adc_clock_config(RCU_ADCCK_APB2_DIV6);
	
}

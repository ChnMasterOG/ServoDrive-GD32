/*!
    \file    tim.c
    \brief   GD32 ServoDrive TIM
*/

#include "tim.h"

void tim2_config(void)
{
	timer_oc_parameter_struct timer_ocintpara;
  timer_parameter_struct timer_initpara;

  timer_deinit(TIMER2);

	/* TIMER2 configuration */
	timer_initpara.prescaler         = 0;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = 999;	// 8khz
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER2, &timer_initpara);

	/* CH0 & CH1 configuration in PWM mode1 */
	timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	timer_channel_output_config(TIMER2, TIMER_CH_0, &timer_ocintpara);
	timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 0);
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 0);
	timer_channel_output_mode_config(TIMER2, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER2, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER2);
	
	timer_enable(TIMER2);
}

/* TIM13 init function */
void tim13_config(void)
{
	timer_parameter_struct timer_initpara;

  timer_deinit(TIMER13);

	/* TIMER13 configuration */
	timer_initpara.prescaler         = 7;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = 4999;	// 5ms
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER13, &timer_initpara);

	timer_interrupt_enable(TIMER13, TIMER_INT_UP);
	
	/* TIMER13 interrupt Init */
	nvic_irq_enable(TIMER13_IRQn, 3, 0);
	
	timer_enable(TIMER13);
}


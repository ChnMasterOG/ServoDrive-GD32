/*!
    \file    gpio.c
    \brief   GD32 ServoDrive GPIO
*/

#include "gpio.h"

void gpio_config(void)
{
	/* GPIOF0 & GPIOF1 */
	gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2);

  gpio_bit_reset(GPIOF, GPIO_PIN_0 | GPIO_PIN_1);
	
	/* GPIOA6 & GPIOA7 */
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
	
	gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_6 | GPIO_PIN_7);
		
	/* GPIOA9 & GPIOA10 */
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9 | GPIO_PIN_10);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_9 | GPIO_PIN_10);
	
	gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_9 | GPIO_PIN_10);
	
	/* GPIOA0 */
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	
	gpio_bit_set(GPIOA, GPIO_PIN_0);
}

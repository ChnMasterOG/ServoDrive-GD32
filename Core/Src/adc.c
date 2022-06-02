/*!
    \file    adc.c
    \brief   GD32 ServoDrive ADC
*/

#include "adc.h"

uint16_t adcData[8];
uint32_t adcAvg;

void adc_start(void)
{
	adc_enable();
	adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
}

void adc_config(void)
{
	/**ADC GPIO Configuration
	PA4     ------> ADC_IN4
	*/
	gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);

	/* ADC channel length config */
	adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);

	/* ADC regular channel config */
	adc_regular_channel_config(0, ADC_CHANNEL_4, ADC_SAMPLETIME_55POINT5);

	/* ADC external trigger enable */
	adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);
	/* ADC external trigger source config */
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
	/* ADC data alignment config */
	adc_data_alignment_config(ADC_DATAALIGN_RIGHT);
	/* ADC DMA function enable */
	adc_dma_mode_enable();
	/* enable ADC continuous mode */
	adc_special_function_config(ADC_CONTINUOUS_MODE, ENABLE);
	/* enable ADC interface */
	adc_enable();
	/* ADC calibration and reset calibration */
	adc_calibration_enable();
}

void dma_config(void)
{
	/* ADC DMA Init */
	dma_parameter_struct dma_init_struct;

	/* initialize DMA channel0 */
	dma_deinit(DMA_CH0);	//CH1
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_addr = (uint32_t)adcData;
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_init_struct.number = 8;
	dma_init_struct.periph_addr = (uint32_t)&(ADC_RDATA);
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_init_struct.priority = DMA_PRIORITY_LOW;
	dma_init(DMA_CH0,&dma_init_struct);

	/* configure DMA mode */
	dma_circulation_enable(DMA_CH0);
	dma_memory_to_memory_disable(DMA_CH0);

	dma_interrupt_enable(DMA_CH0, DMA_INT_FTF);

	/* DMA CH0 interrupt Init */
	nvic_irq_enable(DMA_Channel0_IRQn, 2, 0);
	
	/* enable DMA channel0 */
	dma_channel_enable(DMA_CH0);
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/*!
    \file    adc.h
    \brief   GD32 ServoDrive ADC
*/

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern uint16_t adcData[8];
extern uint32_t adcAvg;

void adc_start(void);
void adc_config(void);
void dma_config(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */


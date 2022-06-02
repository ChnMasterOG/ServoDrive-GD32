/*!
    \file    main.h
    \brief   GD32 ServoDrive Main
*/

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f1x0.h"
#include "systick.h"

typedef enum configStatus_t
{
    CONFIG_RESTORE = 0,
    CONFIG_OK,
    CONFIG_COMMIT
} configStatus_t;

typedef struct Config_t
{
    configStatus_t configStatus;
    uint8_t nodeId;
    float initPos;
    float toqueLimit;
    float velocityLimit;
    uint16_t adcValAtAngleMin;
    uint16_t adcValAtAngleMax;
    float mechanicalAngleMin;
    float mechanicalAngleMax;
    float dceKp;
    float dceKv;
    float dceKi;
    float dceKd;
    uint8_t enableMotorOnBoot;
} BoardConfig_t;

#define BoardConfig_SIZE	64

void dma_InterruptCallback(void);
void i2c_SlaveRxCpltCallback(void);
void tim13_PeriodElapsedCallback(void);
	
#endif /* __MAIN_H */

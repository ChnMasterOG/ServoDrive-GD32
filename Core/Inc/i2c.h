/*!
    \file    i2c.h
    \brief   GD32 ServoDrive RCU
*/

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define I2C_SPEED              400000

extern uint8_t i2cDataRx[8];
extern uint8_t i2cDataTx[8];

void i2c_config(uint32_t _id);

#ifdef __cplusplus
}
#endif
#endif /* __RCU_H__ */


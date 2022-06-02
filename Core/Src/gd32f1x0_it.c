/*!
    \file    gd32f1x0_it.c
    \brief   interrupt service routines

    \version 2016-01-15, V1.0.0, demo for GD32F1x0
    \version 2016-05-13, V2.0.0, demo for GD32F1x0
    \version 2019-11-20, V3.0.0, demo for GD32F1x0
    \version 2021-12-31, V3.1.0, demo for GD32F1x0
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f1x0_it.h"
#include "main.h"
#include "i2c.h"

static uint32_t I2cTxIndex = 0;
static uint32_t I2cRxIndex = 0;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
}

/*!
    \brief      this function handles i2c0 event interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void I2C0_EV_IRQHandler(void)
{
	/* receive address */
	if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_ADDSEND))
	{
		i2c_interrupt_flag_clear(I2C0, I2C_INT_FLAG_ADDSEND);
	}
	/* receive data */
	else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_RBNE))
	{
		i2cDataRx[I2cRxIndex++] = i2c_data_receive(I2C0);
		if (I2cRxIndex == 5)
		{
			I2cRxIndex = 0;
			i2c_SlaveRxCpltCallback();
		}
  }
	/* request data */
	else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_TBE))
	{
		i2c_data_transmit(I2C0, i2cDataTx[I2cTxIndex++]);
		if (I2cTxIndex == 5)
		{
			I2cTxIndex = 0;
		}
  }
	/* stop request */
	else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_STPDET))
	{
		i2c_stop_on_bus(I2C0);
		I2cTxIndex = I2cRxIndex = 0;
	}
	/* nack */
	else if (i2c_interrupt_flag_get(I2C0, I2C_INT_FLAG_AERR))
	{
		I2cTxIndex = I2cRxIndex = 0;
	}
}

/*!
    \brief      this function handles dma channel0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA_Channel0_IRQHandler(void)
{
    if(RESET != dma_interrupt_flag_get(DMA_CH0, DMA_INT_FLAG_FTF)){
        dma_interrupt_flag_clear(DMA_CH0, DMA_INT_FLAG_FTF);
				dma_InterruptCallback();
    }
}

/*!
    \brief      this function handles timer13 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER13_IRQHandler(void)
{
    if(RESET != timer_interrupt_flag_get(TIMER13, TIMER_INT_FLAG_UP)){
        timer_interrupt_flag_clear(TIMER13, TIMER_INT_FLAG_UP);
				tim13_PeriodElapsedCallback();
    }
}

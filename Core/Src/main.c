/*!
    \file    main.c
    \brief   GD32 ServoDrive Main
*/

#include "main.h"
#include "stdio.h"
#include "rcu.h"
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "i2c.h"
#include "flash.h"
#include "motor.h"

Motor motor;
BoardConfig_t boardConfig;

void gd_usart_init(void);
void user_main(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	/* hal main */
  systick_config();
	rcu_config();
	gpio_config();
//	gd_usart_init();
	dma_config();
	adc_config();
	tim2_config();
	tim13_config();
	/* user main */
	user_main();
	while (1);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
void user_main(void)
{
	fmc_read(0, &boardConfig, BoardConfig_SIZE);
	if (boardConfig.configStatus != CONFIG_OK) // use default settings
	{
		boardConfig.configStatus = CONFIG_OK;
		boardConfig.nodeId = 12; // 7bit address, has to be even number
		boardConfig.initPos = 90;
		boardConfig.toqueLimit = 0.5;
		boardConfig.velocityLimit = 0;
		boardConfig.adcValAtAngleMin = 0;
		boardConfig.adcValAtAngleMax = 4096;
		boardConfig.mechanicalAngleMin = 0;
		boardConfig.mechanicalAngleMax = 225;
		boardConfig.dceKp = 30;
		boardConfig.dceKv = 0.5;
		boardConfig.dceKi = 0.1;
		boardConfig.dceKd = 150;
		boardConfig.enableMotorOnBoot = 1;
		fmc_write(0, &boardConfig, BoardConfig_SIZE);
	}
	SetTorqueLimit(boardConfig.toqueLimit, &motor);
	motor.mechanicalAngleMin = boardConfig.mechanicalAngleMin;
	motor.mechanicalAngleMax = boardConfig.mechanicalAngleMax;
	motor.adcValAtAngleMin = boardConfig.adcValAtAngleMin;
	motor.adcValAtAngleMax = boardConfig.adcValAtAngleMax;
	motor.dce.kp = boardConfig.dceKp;
	motor.dce.ki = boardConfig.dceKi;
	motor.dce.kv = boardConfig.dceKv;
	motor.dce.kd = boardConfig.dceKd;
	motor.dce.setPointPos = boardConfig.initPos;
	SetEnable(boardConfig.enableMotorOnBoot, &motor);

	i2c_config( boardConfig.nodeId );
	
	while (1)
	{
		if (boardConfig.configStatus == CONFIG_COMMIT)
		{
			boardConfig.configStatus = CONFIG_OK;
			fmc_write(0, &boardConfig, BoardConfig_SIZE);
		}
	}
}

/**
  * @brief  DMA Interrupt CB
  * @retval void
  */
void dma_InterruptCallback(void)
{
		uint8_t i;
		adc_disable();
		// calculate adcAvg
		adcAvg = 0;
		for (i = 0; i < 8; i++)
			adcAvg += adcData[i];
		adcAvg /= 8;
}

// Command handler
void i2c_SlaveRxCpltCallback(void)
{
    float valF = *((float*) (i2cDataRx + 1));
		int i;
		unsigned char* b;

    i2cDataTx[0] = i2cDataRx[0];

    switch (i2cDataRx[0])
    {
        case 0x01:  // Set angle
        {
            motor.dce.setPointPos = valF;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x02: // Set velocity
        {
            motor.dce.setPointVel = valF;
            b = (unsigned char*) &(motor.velocity);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x03: // Set torque
        {
            SetTorqueLimit(valF, &motor);
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x11: // Get angle
        {
            unsigned char* b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x12: // Get velocity
        {
            unsigned char* b = (unsigned char*) &(motor.velocity);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x21: // Set id
        {
            boardConfig.nodeId = i2cDataRx[1];
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x22: // Set kp
        {
            motor.dce.kp = valF;
            boardConfig.dceKp = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x23: // Set ki
        {
            motor.dce.ki = valF;
            boardConfig.dceKi = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x24: // Set kv
        {
            motor.dce.kv = valF;
            boardConfig.dceKv = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x25: // Set kd
        {
            motor.dce.kd = valF;
            boardConfig.dceKd = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x26: // Set torque limit
        {
            SetTorqueLimit(valF, &motor);
            boardConfig.toqueLimit = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0x27: // Set init pos
        {
            boardConfig.initPos = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
				/* add functions */
        case 0x31: // Set adcVal min
        {
            boardConfig.adcValAtAngleMin = (uint16_t)valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
				case 0x32: // Set adcVal max
        {
            boardConfig.adcValAtAngleMax = (uint16_t)valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
				case 0x33: // Set mechanicalAngleMin
        {
            boardConfig.mechanicalAngleMin = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
				case 0x34: // Set mechanicalAngleMax
        {
            boardConfig.mechanicalAngleMax = valF;
            boardConfig.configStatus = CONFIG_COMMIT;
            b = (unsigned char*) &(motor.angle);
            for (i = 0; i < 4; i++)
                i2cDataTx[i + 1] = *(b + i);
            break;
        }
        case 0xff:
            SetEnable(i2cDataRx[1] != 0, &motor);
            break;
        default:
            break;
    }
}

/**
  * @brief  Control loop
  * @retval void
  */
void tim13_PeriodElapsedCallback(void)
{
		adc_start();
		motor.angle = motor.mechanicalAngleMin +
									(motor.mechanicalAngleMax - motor.mechanicalAngleMin) *
									((float) adcData[0] - (float) motor.adcValAtAngleMin) /
									((float) motor.adcValAtAngleMax - (float) motor.adcValAtAngleMin);
		// Calculate PID
		CalcDceOutput(motor.angle, 0, &motor);
		SetPwm((int16_t) motor.dce.output, &motor);
}

/**
  * @brief  usart for DEBUG
  * @retval void
  */
void gd_usart_init(void)
{
    rcu_periph_clock_enable( RCU_GPIOA );
    /* enable USART clock */
    rcu_periph_clock_enable( RCU_USART0 );

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_9);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0,115200U);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_enable(USART0);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TC));
    return ch;
}

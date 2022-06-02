#ifndef SERVODRIVE_FW_MOTOR_H
#define SERVODRIVE_FW_MOTOR_H

#include <stdint.h>

#define DCE_INTEGRAL_LIMIT	(500)

typedef struct
{
		float kp;
		float kv;
		float ki;
		float kd;
		float setPointPos;
		float setPointVel;
		float integralPos;
		float integralVel;
		float lastError;
		float output;
}DCE_t;

typedef struct
{
    DCE_t dce;

    uint16_t adcValAtAngleMin;
    uint16_t adcValAtAngleMax;
    float angle;
    float velocity;
    float mechanicalAngleMin;
    float mechanicalAngleMax;

    uint8_t isEnabled;
    float lastAngle;
    float limitAngleMin;
    float limitAngleMax;
    float limitVelocity;
    float limitTorque; // 0~0.1
}Motor;

void UpdateVelocity(Motor* _motor);
void SetEnable(uint8_t _enable, Motor* _motor);
void SetTorqueLimit(float _percent, Motor* _motor);
float CalcDceOutput(float _inputPos, float _inputVel, Motor* _motor);
void SetPwm(int16_t _pwm, Motor* _motor);

#endif

#include "motor.h"
#include "tim.h"


float CalcDceOutput(float _inputPos, float _inputVel, Motor* _motor)
{
    float errorPos = _inputPos - _motor->dce.setPointPos;
    float errorVel = _inputVel - _motor->dce.setPointVel;
    float deltaPos = errorPos - _motor->dce.lastError;
    _motor->dce.lastError = errorPos;
    _motor->dce.integralPos += errorPos;
    if (_motor->dce.integralPos > DCE_INTEGRAL_LIMIT) _motor->dce.integralPos = DCE_INTEGRAL_LIMIT;
    else if (_motor->dce.integralPos < -DCE_INTEGRAL_LIMIT) _motor->dce.integralPos = -DCE_INTEGRAL_LIMIT;
    _motor->dce.integralVel += errorVel;
    if (_motor->dce.integralVel > DCE_INTEGRAL_LIMIT) _motor->dce.integralVel = DCE_INTEGRAL_LIMIT;
    else if (_motor->dce.integralVel < -DCE_INTEGRAL_LIMIT) _motor->dce.integralVel = -DCE_INTEGRAL_LIMIT;

    _motor->dce.output = _motor->dce.kp * errorPos +
												 _motor->dce.ki * _motor->dce.integralPos + _motor->dce.kv * _motor->dce.integralVel +
												 _motor->dce.kd * deltaPos;

    if (_motor->dce.output > _motor->limitTorque) _motor->dce.output = _motor->limitTorque;
    else if (_motor->dce.output < -_motor->limitTorque) _motor->dce.output = -_motor->limitTorque;

    return _motor->dce.output;
}


void SetPwm(int16_t _pwm, Motor* _motor)
{
    if (_motor->isEnabled)
    {
        if (_pwm >= 0)
        {
						timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 0);
						timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, _pwm > 1000 ? 1000 : _pwm);
        } else
        {
						timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 0);
						timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, _pwm < -1000 ? 1000 : -_pwm);
        }
    } else
    {
        timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_0, 0);
        timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 0);
    }
}


void SetTorqueLimit(float _percent, Motor* _motor)
{
    if (_percent > 1)_percent = 1;
    else if (_percent < 0)_percent = 0;

    _motor->limitTorque = _percent * 1000;
}


void UpdateVelocity(Motor* _motor)
{
    _motor->velocity = _motor->angle - _motor->lastAngle;
    _motor->lastAngle = _motor->angle;
}


void SetEnable(uint8_t _enable, Motor* _motor)
{
    _motor->isEnabled = _enable;
}


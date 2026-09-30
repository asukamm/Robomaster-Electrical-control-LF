#ifndef DVC_MOTOR_H
#define DVC_MOTOR_H

#include "alg_pid.h"
#include "stm32f4xx_hal.h"

#define PI           3.14159265358979f
#define RPM_TO_RADPS (2.0f * PI / 60.0f)

enum Enum_Control_Method
{
    Control_Method_OPENLOOP = 0,
    Control_Method_OMEGA,
    Control_Method_ANGLE,
};

class Class_Motor
{
public:
    Class_PID PID_Angle;
    Class_PID PID_Omega;

    void Init(CAN_HandleTypeDef *hcan, uint16_t __CAN_ID, Enum_Control_Method method);

    float Get_Now_Angle();
    float Get_Now_Omega();
    float Get_Target_Angle();
    float Get_Target_Omega();

    void Set_Target_Angle(float angle);
    void Set_Target_Omega(float omega);

    void CAN_RxCpltCallback(uint16_t StdId, uint8_t *data);
    void TIM_PID_PeriodElapsedCallback();

private:
    CAN_HandleTypeDef *hcan;
    uint16_t CAN_ID;
    Enum_Control_Method Control_Method;

    uint16_t Rx_Encoder;
    int16_t  Rx_Omega;
    uint16_t Pre_Encoder;
    int32_t  Total_Round;
    int32_t  Total_Encoder;
    float    Now_Angle;
    float    Now_Omega;

    float Target_Angle;
    float Target_Omega;
    float Out;

    void Output();
};

#endif

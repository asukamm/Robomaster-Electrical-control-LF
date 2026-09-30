#include "dvc_motor.h"

void Class_Motor::Init(CAN_HandleTypeDef *__hcan, uint16_t __CAN_ID, Enum_Control_Method method)
{
    hcan = __hcan;
    CAN_ID = __CAN_ID;
    Control_Method = method;

    Rx_Encoder = 0;
    Pre_Encoder = 0;
    Total_Round = 0;
    Total_Encoder = 0;
    Rx_Omega = 0;

    Now_Angle = 0.0f;
    Now_Omega = 0.0f;
    Target_Angle = 0.0f;
    Target_Omega = 0.0f;
    Out = 0.0f;
}

float Class_Motor::Get_Now_Angle()   { return Now_Angle; }
float Class_Motor::Get_Now_Omega()   { return Now_Omega; }
float Class_Motor::Get_Target_Angle() { return Target_Angle; }
float Class_Motor::Get_Target_Omega() { return Target_Omega; }

void Class_Motor::Set_Target_Angle(float angle) { Target_Angle = angle; }
void Class_Motor::Set_Target_Omega(float omega) { Target_Omega = omega; }

void Class_Motor::CAN_RxCpltCallback(uint16_t StdId, uint8_t *data)
{
    if (StdId != CAN_ID)
        return;

    Pre_Encoder = Rx_Encoder;

    Rx_Encoder = (data[0] << 8) | data[1];
    Rx_Omega   = (int16_t)((data[2] << 8) | data[3]);

    int16_t delta = (int16_t)(Rx_Encoder - Pre_Encoder);
    if (delta < -4096)     Total_Round++;
    else if (delta > 4096) Total_Round--;

    Total_Encoder = Total_Round * 8192 + Rx_Encoder;
    Now_Angle = (float)Total_Encoder / 8192.0f * 2.0f * PI;
    Now_Omega = (float)Rx_Omega * RPM_TO_RADPS;
}

void Class_Motor::TIM_PID_PeriodElapsedCallback()
{
#if 0
    Out = 1500.0f;
    Output();
    return;
#endif

    switch (Control_Method)
    {
    case Control_Method_OMEGA:
        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();
        Out = PID_Omega.Get_Out();
        break;

    case Control_Method_ANGLE:
        PID_Angle.Set_Target(Target_Angle);
        PID_Angle.Set_Now(Now_Angle);
        PID_Angle.TIM_Adjust_PeriodElapsedCallback();
        Target_Omega = PID_Angle.Get_Out();

        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();
        Out = PID_Omega.Get_Out();
        break;

    default:
        Out = 0.0f;
        break;
    }

    Output();
}

void Class_Motor::Output()
{
    CAN_TxHeaderTypeDef tx_header;
    uint8_t  tx_data[8] = {0};
    uint32_t tx_mailbox;

    int16_t current = (int16_t)Out;
    tx_data[0] = (current >> 8) & 0xFF;
    tx_data[1] =  current       & 0xFF;

    tx_header.StdId = 0x200;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = 8;
    tx_header.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(hcan, &tx_header, tx_data, &tx_mailbox);
}

static Class_Motor motor;

extern "C"
{
void Motor_Init(CAN_HandleTypeDef *hcan, int can_id, int method)
{
    motor.PID_Angle.Init(10.0f, 0.0f, 0.0f, 0.0f, 15.0f * PI, 15.0f * PI, 0.001f);
    motor.PID_Omega.Init(100.0f, 5.0f, 0.0f, 0.0f, 2500.0f, 2500.0f, 0.001f);
    motor.Init(hcan, (uint16_t)can_id, (Enum_Control_Method)method);
}

void Motor_Set_Target_Angle(float angle) { motor.Set_Target_Angle(angle); }
void Motor_Set_Target_Omega(float omega) { motor.Set_Target_Omega(omega); }

float Motor_Get_Target_Angle() { return motor.Get_Target_Angle(); }
float Motor_Get_Target_Omega() { return motor.Get_Target_Omega(); }
float Motor_Get_Now_Angle()    { return motor.Get_Now_Angle(); }
float Motor_Get_Now_Omega()    { return motor.Get_Now_Omega(); }

void Motor_CAN_RxCpltCallback(uint16_t StdId, uint8_t *data) { motor.CAN_RxCpltCallback(StdId, data); }
void Motor_TIM_PID_PeriodElapsedCallback()   { motor.TIM_PID_PeriodElapsedCallback(); }
}

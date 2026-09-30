#include "dvc_motor.h"

void Class_Motor::Init(CAN_HandleTypeDef *__hcan, Enum_Control_Method method)
{
    hcan = __hcan;
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

void Class_Motor::CAN_RxCpltCallback(uint8_t *data)
{
    Pre_Encoder = Rx_Encoder;

    Rx_Encoder = (data[0] << 8) | data[1];
    Rx_Omega   = (int16_t)((data[2] << 8) | data[3]);

    // 连续角度：检测跨圈跳变（半圈阈值 4096），累加圈数
    int16_t delta = (int16_t)(Rx_Encoder - Pre_Encoder);
    if (delta < -4096)     Total_Round++;   // 正转一圈：8191 -> 0
    else if (delta > 4096) Total_Round--;   // 反转一圈：0 -> 8191

    Total_Encoder = Total_Round * 8192 + Rx_Encoder;
    Now_Angle = (float)Total_Encoder / 8192.0f * 2.0f * PI;   // rad（转子角度）
    Now_Omega = (float)Rx_Omega * RPM_TO_RADPS;               // rad/s
}

void Class_Motor::TIM_PID_PeriodElapsedCallback()
{
    switch (Control_Method)
    {
    case Control_Method_OMEGA:
        // 单环：速度环（3.7 的闭环）
        PID_Omega.Set_Target(Target_Omega);
        PID_Omega.Set_Now(Now_Omega);
        PID_Omega.TIM_Adjust_PeriodElapsedCallback();
        Out = PID_Omega.Get_Out();
        break;

    case Control_Method_ANGLE:
        // 双环串级：角度环 -> 目标速度 -> 速度环 -> 电流
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
    tx_data[0] = (current >> 8) & 0xFF;   // 电机1 电流高字节（大端）
    tx_data[1] =  current       & 0xFF;   // 电机1 电流低字节
    // tx_data[2..7] 保持 0 = 电机 2/3/4 不驱动

    tx_header.StdId = 0x200;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = 8;
    tx_header.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(hcan, &tx_header, tx_data, &tx_mailbox);
}

/* -------------------------------------------------------------------------- */
/*  extern "C" 桥接：main.c 是 C，只能通过这些 C 函数访问电机对象               */
/* -------------------------------------------------------------------------- */

static Class_Motor motor;

extern "C"
{
void Motor_Init(CAN_HandleTypeDef *hcan, int method)
{
    // 手动调参入口：改这里的 PID 参数（顺序对齐科大：K_P, K_I, K_D, K_F, I_Out_Max, Out_Max, D_T）
    // 角度环（pa/ia/da/fa）：角度误差 -> 目标速度，输出限幅 15π rad/s（3.9 速度环用不到它）
    motor.PID_Angle.Init(10.0f, 0.0f, 0.0f, 0.0f, 15.0f * PI, 15.0f * PI, 0.001f);
    // 速度环（po/io/do/fo）：速度误差 -> 电流，输出限幅 2500。fo 是 3.9 新加的前馈系数，追正弦目标时手动调
    motor.PID_Omega.Init(100.0f, 5.0f, 0.0f, 0.0f, 2500.0f, 2500.0f, 0.001f);
    motor.Init(hcan, (Enum_Control_Method)method);
}

void Motor_Set_Target_Angle(float angle) { motor.Set_Target_Angle(angle); }
void Motor_Set_Target_Omega(float omega) { motor.Set_Target_Omega(omega); }

float Motor_Get_Target_Angle() { return motor.Get_Target_Angle(); }
float Motor_Get_Target_Omega() { return motor.Get_Target_Omega(); }
float Motor_Get_Now_Angle()    { return motor.Get_Now_Angle(); }
float Motor_Get_Now_Omega()    { return motor.Get_Now_Omega(); }

void Motor_CAN_RxCpltCallback(uint8_t *data) { motor.CAN_RxCpltCallback(data); }
void Motor_TIM_PID_PeriodElapsedCallback()   { motor.TIM_PID_PeriodElapsedCallback(); }
}

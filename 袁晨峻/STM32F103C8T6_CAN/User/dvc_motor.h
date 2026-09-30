#ifndef DVC_MOTOR_H
#define DVC_MOTOR_H

#include "alg_pid.h"
#include "stm32f1xx_hal.h"

#define PI           3.14159265358979f
#define RPM_TO_RADPS (2.0f * PI / 60.0f)   // 转速 RPM 换算 rad/s

/**
 * @brief 电机控制方式
 */
enum Enum_Control_Method
{
    Control_Method_OPENLOOP = 0,   // 开环
    Control_Method_OMEGA,          // 速度环（单环，3.7 的内容）
    Control_Method_ANGLE,          // 角度环（双环串级，3.8 的内容）
};

/**
 * @brief 电机类（设备层 dvc）
 *
 * 把 3.2~3.7 散在 main.c 里的电机相关逻辑收进一个对象：
 *   - CAN 反馈拆包 + 连续角度计算（跨圈累加）
 *   - 双环串级 PID（角度环外 + 速度环内）
 *   - CAN 指令打包发送
 * 对外只暴露 Init / Set_Target / Get_Now / 两个周期回调。
 */
class Class_Motor
{
public:
    Class_PID PID_Angle;   // 位置环（外环）：角度误差 -> 目标速度
    Class_PID PID_Omega;   // 速度环（内环）：速度误差 -> 电流指令

    void Init(CAN_HandleTypeDef *hcan, Enum_Control_Method method);

    float Get_Now_Angle();
    float Get_Now_Omega();
    float Get_Target_Angle();
    float Get_Target_Omega();

    void Set_Target_Angle(float angle);
    void Set_Target_Omega(float omega);

    void CAN_RxCpltCallback(uint8_t *data);     // 反馈拆包 + 连续角度
    void TIM_PID_PeriodElapsedCallback();       // 串级 PID + 发 CAN

private:
    CAN_HandleTypeDef *hcan;
    Enum_Control_Method Control_Method;

    // 反馈
    uint16_t Rx_Encoder;     // 单圈编码器 0~8191
    int16_t  Rx_Omega;       // 转速 RPM
    uint16_t Pre_Encoder;    // 上一次编码器
    int32_t  Total_Round;    // 累计圈数
    int32_t  Total_Encoder;  // 连续编码器值
    float    Now_Angle;      // 当前角度 rad（转子角度，不除减速比）
    float    Now_Omega;      // 当前速度 rad/s

    // 目标 / 输出
    float Target_Angle;      // 目标角度 rad
    float Target_Omega;      // 目标速度 rad/s
    float Out;               // 电流指令

    void Output();           // 打包 0x200 发 CAN
};

#endif

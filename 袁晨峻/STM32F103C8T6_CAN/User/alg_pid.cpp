#include "alg_pid.h"

void Class_PID::Init(float __K_P, float __K_I, float __K_D, float __K_F, float __I_Out_Max, float __Out_Max, float __D_T)
{
    K_P = __K_P;
    K_I = __K_I;
    K_D = __K_D;
    K_F = __K_F;
    I_Out_Max = __I_Out_Max;
    Out_Max = __Out_Max;
    D_T = __D_T;

    Target = 0.0f;
    Now = 0.0f;
    Integral_Error = 0.0f;
    Pre_Error = 0.0f;
    Pre_Target = 0.0f;
    Out = 0.0f;
}

float Class_PID::Get_Out()
{
    return Out;
}

float Class_PID::Get_Integral_Error()
{
    return Integral_Error;
}

void Class_PID::Set_K_P(float v) { K_P = v; }
void Class_PID::Set_K_I(float v) { K_I = v; }
void Class_PID::Set_K_D(float v) { K_D = v; }
void Class_PID::Set_K_F(float v) { K_F = v; }

void Class_PID::Set_Target(float v) { Target = v; }
void Class_PID::Set_Now(float v) { Now = v; }
void Class_PID::Set_Integral_Error(float v) { Integral_Error = v; }

void Class_PID::TIM_Adjust_PeriodElapsedCallback()
{
    float error = Target - Now;

    // P：现在（弹簧）
    float p_out = K_P * error;

    // I：过去（记仇）。积分器存的是 ∫e dt，输出时再乘 K_I，这样 K_I 是纯增益、量纲独立于 D_T
    float i_out = 0.0f;
    if (K_I != 0.0f)
    {
        // 积分限幅：限的是"积分项输出 ≤ I_Out_Max"，所以积分器上限 = I_Out_Max / K_I
        if (I_Out_Max != 0.0f)
        {
            if (Integral_Error >  I_Out_Max / K_I) Integral_Error =  I_Out_Max / K_I;
            if (Integral_Error < -I_Out_Max / K_I) Integral_Error = -I_Out_Max / K_I;
        }
        Integral_Error += D_T * error;   // 离散积分：累加 error * dt
        i_out = K_I * Integral_Error;
    }

    // D：未来（刹车）。误差差分（科大默认，非微分先行）
    float d_out = K_D * (error - Pre_Error) / D_T;

    // F：前馈（提前量）。目标的变化量 × K_F，不等误差出来就提前给输出
    float f_out = (Target - Pre_Target) * K_F;

    Out = p_out + i_out + d_out + f_out;

    // 输出限幅
    if (Out_Max != 0.0f)
    {
        if (Out >  Out_Max) Out =  Out_Max;
        if (Out < -Out_Max) Out = -Out_Max;
    }

    Pre_Error = error;
    Pre_Target = Target;
}

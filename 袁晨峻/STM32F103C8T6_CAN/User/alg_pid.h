#ifndef ALG_PID_H
#define ALG_PID_H

/**
 * @brief PID 算法类（算法层 alg）
 *
 * 把 3.7 的 C 结构体 PID（PID_t + PID_Init + PID_Calc）升级成类：
 * 数据和操作装进同一个 class，对外只暴露 Init / Set / Adjust / Get 几个接口。
 * 接口对齐科大 Class_PID 的核心（D_T 显式传入，不揉进 K_I/K_D）。
 */
class Class_PID
{
public:
    void Init(float K_P, float K_I, float K_D, float K_F, float I_Out_Max, float Out_Max, float D_T);

    float Get_Out();
    float Get_Integral_Error();

    void Set_K_P(float K_P);
    void Set_K_I(float K_I);
    void Set_K_D(float K_D);
    void Set_K_F(float K_F);

    void Set_Target(float Target);
    void Set_Now(float Now);
    void Set_Integral_Error(float Integral_Error);

    void TIM_Adjust_PeriodElapsedCallback();

private:
    float K_P, K_I, K_D;      // 三件套系数
    float K_F;                // 前馈系数：目标变化量 -> 输出提前量
    float I_Out_Max;          // 积分项输出限幅（0 为不限）
    float Out_Max;            // 输出限幅（0 为不限）
    float D_T;                // 采样周期 s（显式传入）

    float Target;             // 目标值
    float Now;                // 当前值
    float Integral_Error;     // 积分器：累加 ∫e dt
    float Pre_Error;          // 上一次误差（D 项用）
    float Pre_Target;         // 上一次目标（F 项用）
    float Out;                // 输出
};

#endif

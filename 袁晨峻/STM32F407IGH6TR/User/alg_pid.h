#ifndef ALG_PID_H
#define ALG_PID_H

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
    float K_P, K_I, K_D;
    float K_F;
    float I_Out_Max;
    float Out_Max;
    float D_T;

    float Target;
    float Now;
    float Integral_Error;
    float Pre_Error;
    float Pre_Target;
    float Out;
};

#endif

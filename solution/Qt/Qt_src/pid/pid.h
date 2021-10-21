#ifndef PID_H
#define PID_H

#include "lowpassfilter/lowpassfilter.h"

class PID
{
public:
    PID(float kp, float ki, float kd, float kn);
    ~PID();

    float compute(float error, float dt);
private:
    float m_kp, m_ki, m_kd, m_kn;
    float m_proportional, m_integral, m_derivative, m_previous_error;
    LowPassFilter* m_filter;
};

#endif // PID_H

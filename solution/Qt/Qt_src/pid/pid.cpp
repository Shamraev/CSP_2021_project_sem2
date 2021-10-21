#include "pid.h"

PID::PID(float kp, float ki, float kd, float kn)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
    m_kn = kn;

    m_previous_error = 0;
    m_integral = 0;

    m_filter = new LowPassFilter(1/m_kn);
}

PID::~PID()
{
    delete m_filter;
}

float PID::compute(float error, float dt)
{
    // equations are from https://en.wikipedia.org/wiki/PID_controller#Pseudocode
    if (dt<=0)
        return error;

    m_proportional = error;
    m_integral = m_integral + error * dt;
    m_derivative = (error - m_previous_error)/dt;

    //m_derivative = m_filter->Update(m_derivative, dt);

    float output = m_kp * m_proportional + m_ki * m_integral + m_kd * m_derivative;
    m_previous_error = error;

    return output;
}

#include "lowpassfilter.h"

LowPassFilter::LowPassFilter(float wpass)
{
    m_wpass = wpass;
    m_y = 0;
}

float LowPassFilter::Update(float x, float dt)
{
    // equations are from https://en.wikipedia.org/wiki/Low-pass_filter

    if (dt<=0)
        return x;

    float f = 1/dt * m_wpass * 2*M_PI;
    float alpha = dt / (f + dt);
    m_y = m_y + (x - m_y)*alpha;
    return m_y;
}

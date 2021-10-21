#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <math.h>

class LowPassFilter
{
public:
    LowPassFilter(float wpass); // wpass - относительная частота среза
    float Update(float x, float dt);

private:
    float m_wpass;
    float m_y;
};

#endif // LOWPASSFILTER_H

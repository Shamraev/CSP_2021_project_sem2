#ifndef INTEGRATOR_H
#define INTEGRATOR_H


class Integrator
{
public:
    Integrator(float initValue);
    float state() const;
    float update(float input, float dt);

private:
    float m_state;
    float m_previous_input;
};

#endif // INTEGRATOR_H

#ifndef DISCRETE_H
#define DISCRETE_H


class Discrete
{
public:
    Discrete(float x1, float x2,
             float a11, float a12,
             float a21, float a22,
             float b1, float b2,
             float c1, float c2, float d);

    float getOutput() const;
    float getVelocity() const;
    float getAcceleration() const;

    float update(float input);

private:
    // Initial states
    float m_x1;
    float m_x2;

    // State matrix coefficients
    float m_a11;
    float m_a12;
    float m_a21;
    float m_a22;

    // Input matrix coefficients
    float m_b1;
    float m_b2;

    // Output matrix coefficients
    float m_c1;
    float m_c2;

    // Direct matrix coefficients
    float m_d;

    // Previous input
    float m_previous_input;
};

#endif // DISCRETE_H

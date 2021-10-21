#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "lowpassfilter/lowpassfilter.h"
#include "pid/pid.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

public slots:

    float computeU(float value, float dt, float referenceSignal);
    float computeReferenceSignal(float value, float t);

    void computeBytes(QByteArray message);


signals:

    void generatedInput(float value);
    void generatedReference(float value);

private:

    QDateTime m_lastMeasure;
    float m_seconds;
    long m_n, m_corM;
    QFile* m_file;
    QTextStream* m_stream;
    LowPassFilter* m_q_filter;
    PID* m_pid;

};

#endif // CONTROLLER_H

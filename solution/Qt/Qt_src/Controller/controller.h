#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QDateTime>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

public slots:

    void compute(float value, float seconds, float referenceSignal);

    void computeBytes(QByteArray message);


signals:

    void computed(float value);

private:

    QDateTime m_lastMeasure;
    float m_seconds;
    long m_n;

};

#endif // CONTROLLER_H

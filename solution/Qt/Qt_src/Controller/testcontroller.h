#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QDateTime>

class TestController : public QObject
{
    Q_OBJECT
public:
    explicit TestController(QObject *parent = nullptr);

public slots:

    void compute(float value, float seconds, float referenceSignal);

    void computeBytes(QByteArray message);


signals:

    void computed(float value);

private:

    QDateTime m_lastMeasure;

};

#endif // TESTCONTROLLER_H

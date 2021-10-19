#include "testcontroller.h"

TestController::TestController(QObject *parent) : QObject(parent)
{

}

void TestController::compute(float value, float seconds, float referenceSignal)
{
    //emit computed((referenceSignal - value) * 10);
    //emit computed(referenceSignal);
    //emit computed(50 + 50 * qSin(QDateTime::currentMSecsSinceEpoch() / 1000));
}

void TestController::computeBytes(QByteArray message)
{
    float seconds = 0;

    if (m_lastMeasure.isNull()) {

    } else {
        QDateTime currentDateTime = QDateTime::currentDateTime();

        seconds = m_lastMeasure.msecsTo(currentDateTime) / 1000.0;
    }

    m_lastMeasure = QDateTime::currentDateTime();

//    qDebug() << message.toHex(' ') << " " << seconds;
}

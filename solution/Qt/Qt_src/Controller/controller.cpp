#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_seconds = 0;
    m_n = 0;
}

void Controller::compute(float value, float seconds, float referenceSignal)
{
    //emit computed((referenceSignal - value) * 10);
    //emit computed(referenceSignal);
    //emit computed(50 + 50 * qSin(QDateTime::currentMSecsSinceEpoch() / 1000));
}

void Controller::computeBytes(QByteArray message)
{
    m_n += 1;
    if (m_lastMeasure.isNull()) {

    } else {
        QDateTime currentDateTime = QDateTime::currentDateTime();

        m_seconds += m_lastMeasure.msecsTo(currentDateTime) / 1000.0;
    }

    m_lastMeasure = QDateTime::currentDateTime();

    qDebug() << "controller received message ("<<m_n<<"): " << message.toHex(' ') << " t = " << m_seconds << " s";
}

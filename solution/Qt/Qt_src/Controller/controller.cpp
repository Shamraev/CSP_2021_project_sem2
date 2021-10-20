#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_seconds = 0;
    m_n = 0;
    m_corM = 0;
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

    // Checksum
    char s = message[message.length()-1];
    char s_pred = 0;
    for (int i=0; i<message.length()-1; i++) {
        s_pred += message[i];
    }
    //qDebug() << "predicted checksum if: first 10 sum: " << QChar(s_pred).unicode() <<" , real checksum: " << QChar(s).unicode(); // if the 11th byte is only the sum of the first 10 bytes
    s_pred = (0xff-s_pred);
    qDebug() << "predicted checksum if: ff - (first 10 sum): " << QChar(s_pred).unicode() <<" , real checksum: " << QChar(s).unicode();
    qDebug() << "corrupted messages: " << m_corM;
    if (s_pred!=s){
        qDebug() << "corrupted message";
        m_corM +=1;
        return;
    }

    // Message parsing
    float q, refLevel;
    memcpy(&q, message.data() + 2, 4);
    memcpy(&refLevel, message.data() + 6, 4);
    qDebug() << "q coordinate of plant: " << q << ", reference level: " << refLevel;

}

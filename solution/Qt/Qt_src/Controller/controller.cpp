#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_seconds = 0;
    m_n = 0;
    m_corM = 0;

    // Log file
    QString path("logs/");
    QDir dir; // Initialize to the desired dir if 'path' is relative
              // By default the program's working directory "." is used.

    // We create the directory if needed
    if (!dir.exists(path))
        dir.mkpath(path); // You can check the success if needed


    m_file = new QFile(path + QDateTime::currentDateTime().toString("hh_MM_ss"));
    if (!(m_file->open(QIODevice::WriteOnly))){
        qDebug() << "Failed to open log file";
    }
    m_stream = new QTextStream(m_file);
}

Controller::~Controller()
{
    m_file->close();
    delete m_stream;
    delete m_file;
}

void Controller::compute(float value, float seconds, float referenceSignal)
{
    emit computed((referenceSignal - value) * 10);
    emit computed(referenceSignal);
    emit computed(50 + 50 * qSin(seconds));
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
    if (refLevel<0) {refLevel = 0;}
    qDebug() << "q coordinate of plant: " << q << ", reference level: " << refLevel;

    // Saving q, refLevel, and t to the file for Matlab
    if (m_n==1)
        *m_stream << q << " " << refLevel << " " << m_seconds;
    else
        *m_stream << "\n" << q << " " << refLevel << " " << m_seconds;

    // Send u to object
    compute(q, m_seconds, refLevel);
}

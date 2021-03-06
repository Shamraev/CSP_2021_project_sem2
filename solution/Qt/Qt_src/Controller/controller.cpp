#include "controller.h"

using namespace std;

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_seconds = 0;
    m_n = 0;
    m_corM = 0;
    m_q_filter = new LowPassFilter(2.0e-4);
    m_pid = new PID(22.07, 24.3, 4.493, 0);

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
    delete m_q_filter;
    delete m_pid;
}

float Controller::computeU(float q, float dt, float referenceSignal)
{
    float u = m_pid->compute(referenceSignal-q, dt);
    return u;
}

float Controller::computeReferenceSignal(float q, float t)
{
    return 70 - 10*exp(-0.1*t)*((-pow((t-2),2)+60*t)/100)*sin(t/2);
}

void Controller::computeBytes(QByteArray message)
{
    m_n += 1;
    float dt = 0;
    if (m_lastMeasure.isNull()) {

    } else {
        QDateTime currentDateTime = QDateTime::currentDateTime();

        dt = m_lastMeasure.msecsTo(currentDateTime) / 1000.0;
        m_seconds += dt;
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

    // Generate and send a reference signal to an object
    float gen_refLevel = computeReferenceSignal(q, m_seconds);
    emit generatedReference(gen_refLevel);

    // Send u to object
    float q_filt = m_q_filter->Update(q, dt);
    float u = computeU(q_filt, dt, refLevel);
    emit generatedInput(u);

    // Saving q, refLevel, and t to the file for Matlab
    if (m_n==1)
        *m_stream << q << " " << refLevel << " " << u << " " << m_seconds << " " << q_filt;
    else
        *m_stream << "\n" << q << " " << refLevel << " " << u << " " << m_seconds << " " << q_filt;
}

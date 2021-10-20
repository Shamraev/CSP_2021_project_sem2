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

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

public slots:

    float computeU(float value, float seconds, float referenceSignal);
    float computeReferenceSignal(float value, float seconds);

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

};

#endif // CONTROLLER_H

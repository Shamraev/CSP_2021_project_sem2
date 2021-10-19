#ifndef CONTROLLERBACKEND_H
#define CONTROLLERBACKEND_H

#include <QDebug>
#include <QObject>
#include <QTimer>
#include <random>
#include <QDateTime>

#include "Discrete/discrete.h"
#include "ControllerBackend_global.h"
#include "Logger/Logger.h"

class CONTROLLERBACKEND_EXPORT ControllerBackend: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(float input READ input WRITE setInput NOTIFY inputChanged)
    Q_PROPERTY(float output READ output WRITE setOutput NOTIFY outputChanged)
    Q_PROPERTY(float referenceSignal READ referenceSignal WRITE setReferenceSignal NOTIFY referenceSignalChanged)

public:
    ControllerBackend(int timerInterval = 200, QString password = QString(), QObject* parent = nullptr);
    ~ControllerBackend();

    Q_INVOKABLE void start(float initialPosition = 0);
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setInput(float input);

    Q_INVOKABLE void startLog();
    Q_INVOKABLE void stopLog();

    bool active() const;

    float output() const;

    float input() const;

    float referenceSignal() const;

    void setGeneratorFunction(float (*function)(float));

public slots:
    void setActive(bool active);

    void setOutput(float output);

    void setReferenceSignal(float referenceSignal);

signals:

    void message(QString message);

    void outputChanged(float output, float seconds, float referenceSignal);

    void outputBytesChanged(QByteArray output);

    void activeChanged(bool active);

    void inputChanged(float input, float seconds);

    void referenceSignalChanged(float referenceSignal);

private slots:

    void generateSignal();

private:

    void clearAll();
    void update();
    void appendLog();

    float addNoise(float signal);    

    float m_input;
    float m_output;

    QByteArray encode();


    int m_member = 2;
    QTimer m_timer;

    QSharedPointer<Discrete> m_discreteModel;

    std::default_random_engine m_generator;
    std::normal_distribution<float> m_distribution;
    std::uniform_int_distribution<int> m_uniform_distribution;
    bool m_active;

    Logger m_logger;

    QDateTime m_startTime;
    float m_referenceSignal;

    // Pointer to processing function
    float (*m_processingFunction)(float);

    bool m_isAdmin = false;
};

#endif // CONTROLLERBACKEND_H

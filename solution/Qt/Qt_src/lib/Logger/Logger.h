#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QSharedPointer>
#include <QDebug>

class Logger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool writing READ writing WRITE setWriting NOTIFY writingChanged)
public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    Q_INVOKABLE void startLog(QString name);
    Q_INVOKABLE void stopLog();
    void log(QString string);

    bool writing() const;

public slots:
    void setWriting(bool writing);

signals:

    void writingChanged(bool writing);

private:

    QSharedPointer<QFile> m_file;
    QSharedPointer<QTextStream> m_stream;
    bool m_writing;
};

#endif // LOGGER_H

#ifndef TASK_H
#define TASK_H

#include <optional>
#include <QVariant>

class Task : public QObject {
    Q_OBJECT
    QString m_name;
    size_t m_arrival;
    std::optional<size_t> m_wcet;
    size_t m_deadline;
public:
    class Result {
        QString m_name;
        QVariant m_value;
        bool m_outstanding;
    public:
        Result(QString name, std::optional<QVariant> value);
        QVariant value() const;
        QString name() const;
        bool outstanding() const;
    };

    Task(QString name, std::optional<size_t> wcet = std::nullopt, QObject *parent = nullptr);

    virtual std::optional<QVariant> proceed() = 0;
    virtual void reset() = 0;

    void autodetectWcet();

    QString name() const;
    size_t arrival() const;
    void setArrival(size_t arrival);
    std::optional<size_t> wcet() const;
    size_t deadline() const;
    void setDeadline(size_t deadline);

    size_t measure();

};

QDebug &operator << (QDebug &d, const Task::Result& r);


#endif // TASK_H

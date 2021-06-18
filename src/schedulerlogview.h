#ifndef SCHEDULERLOGVIEW_H
#define SCHEDULERLOGVIEW_H

#include <QQuickPaintedItem>
#include <klibcore/src/kmacro.h>
#include <src/scheduler/scheduler.h>

class SchedulerLogView : public QQuickPaintedItem {
    Q_OBJECT
    K_QML_TYPE(SchedulerLogView)
    K_AUTO_PROPERTY(QList<SchedulerLog>, log, log, setLog, logChanged, QList<SchedulerLog>())
public:
    SchedulerLogView(QQuickItem *parent = nullptr);

    static QColor hashColor(const QString& str);

signals:


    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);
};

#endif // SCHEDULERLOGVIEW_H

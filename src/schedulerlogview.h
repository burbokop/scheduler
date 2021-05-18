#ifndef SCHEDULERLOGVIEW_H
#define SCHEDULERLOGVIEW_H

#include <QQuickPaintedItem>

class SchedulerLogView : public QQuickPaintedItem {
    Q_OBJECT
public:
    SchedulerLogView(QQuickItem *parent = nullptr);

signals:


    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);
};

#endif // SCHEDULERLOGVIEW_H

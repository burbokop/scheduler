#include "schedulerlogview.h"

#include <QDataStream>
#include <QPainter>

#include <src/math/kmath.h>
#include <QCryptographicHash>

SchedulerLogView::SchedulerLogView(QQuickItem *parent) : QQuickPaintedItem(parent) {
    connect(this, &SchedulerLogView::logChanged, this, [this](){ update(); });
    connect(this, &SchedulerLogView::widthChanged, this, [this](){ update(); });
    connect(this, &SchedulerLogView::heightChanged, this, [this](){ update(); });
}

QColor SchedulerLogView::hashColor(const QString &str) {
    QByteArray hash = QCryptographicHash::hash(
                QByteArray::fromRawData((const char*)str.utf16(), str.length()*2),
                QCryptographicHash::Md5
                );
    QDataStream stream(&hash, QIODevice::ReadOnly);
    quint8 r, g, b;
    stream >> r >> g >> b;
    return QColor::fromRgb(r, g, b);
}

void SchedulerLogView::paint(QPainter *painter) {
    auto l = log();
    if(l.size() < 2)
        return;

    auto firstTime = l.first().startTime();
    auto lastTime = l.last().endTime();

    for(auto it = l.begin(); it != l.end(); ++it) {
        auto startPos = KMath::instance()->dmap(it->startTime(), firstTime, lastTime, 0, width());
        auto endPos = KMath::instance()->dmap(it->endTime(), firstTime, lastTime, 0, width());
        auto name = it->name();
        painter->setBrush(QBrush(hashColor(name)));
        auto rect = QRect(startPos, 0, endPos - startPos, height());

        QPen pen = painter->pen();
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);

        painter->setPen(pen);
        painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, name);
        painter->drawText(rect, Qt::AlignLeft | Qt::AlignBottom | Qt::TextWordWrap, QString::number(it->startTime() - firstTime));
        painter->drawText(rect, Qt::AlignRight | Qt::AlignBottom | Qt::TextWordWrap, QString::number(it->endTime() - firstTime));
    }
}

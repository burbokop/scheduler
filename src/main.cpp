#include "executor.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "tools/optdebug.h"

#include <src/scheduler/erliestdeadlinefirstscheduler.h>
#include <src/scheduler/roundrobinscheduler.h>
#include <src/scheduler/time.h>

#include <src/tasks/fouriertask.h>

void printResults(const Scheduler::ResultVector &results) {
    qDebug() << "RESULTs:" << results;
    for(const auto &r : results) {
        if(r.has_value()) {
            if (r.value().value().canConvert<QVector<double>>()) {
                auto vec = r.value().value().value<QVector<double>>();
                qDebug() << "\tVEC LEN:" << vec.length();
            }
        }
    }
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    Executor executor;

    Scheduler *roundRobinScheduler = new RoundRobinScheduler(5);
    Scheduler *erliestDeadlineFirstScheduler = new ErliestDeadlineFirstScheduler();

    executor.addTask(new FourierTask("fourier0"), Time::nowMillis() + 3000);
    executor.addTask(new FourierTask("fourier1"), Time::nowMillis() + 2000);

    printResults(executor.exec(roundRobinScheduler));
    printResults(executor.exec(erliestDeadlineFirstScheduler));

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/resources/main.qml"));
    engine.load(url);

    return app.exec();
}

#include "executor.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "tools/optdebug.h"

#include <src/scheduler/erliestdeadlinefirstscheduler.h>
#include <src/scheduler/roundrobinscheduler.h>
#include <src/scheduler/time.h>

#include <src/tasks/autocorrelationtask.h>
#include <src/tasks/fouriertask.h>
#include <QQmlContext>

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

    Executor roundRobinExecutor;
    Executor erliestDeadlineFirstExecutor;

    Scheduler *roundRobinScheduler = new RoundRobinScheduler(10);
    Scheduler *erliestDeadlineFirstScheduler = new ErliestDeadlineFirstScheduler();
    {
        roundRobinExecutor.addTask(new FourierTask("fourier0"), Time::nowMillis() + 3000);
        roundRobinExecutor.addTask(new FourierTask("fourier1"), Time::nowMillis() + 2000);
        roundRobinExecutor.addTask(new AutoCorrelationTask("autocorel0"), Time::nowMillis() + 3000);
        roundRobinExecutor.addTask(new AutoCorrelationTask("autocorel1"), Time::nowMillis() + 2000);
    } {
        erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier0"), Time::nowMillis() + 300);
        erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier1"), Time::nowMillis() + 1000);
        erliestDeadlineFirstExecutor.addTask(new AutoCorrelationTask("autocorel0"), Time::nowMillis() + 200);
        erliestDeadlineFirstExecutor.addTask(new AutoCorrelationTask("autocorel1"), Time::nowMillis() + 500);
    }

    printResults(roundRobinExecutor.exec(roundRobinScheduler));
    printResults(erliestDeadlineFirstExecutor.exec(erliestDeadlineFirstScheduler));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("rrExec", &roundRobinExecutor);
    engine.rootContext()->setContextProperty("edfExec", &erliestDeadlineFirstExecutor);
    const QUrl url(QStringLiteral("qrc:/resources/main.qml"));
    engine.load(url);

    return app.exec();
}

#include "executor.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "tools/optdebug.h"

#include <src/scheduler/erliestdeadlinefirstscheduler.h>
#include <src/scheduler/roundrobinscheduler.h>
#include <src/scheduler/time.h>

#include <src/tasks/autocorrelationtask.h>
#include <src/tasks/fouriertask.h>
#include <QApplication>
#include <QQmlContext>
#include <qcustomplot.h>

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

double calcOutstandingPercent(const Scheduler::ResultVector &results) {
    size_t oc = 0;
    size_t all = 0;
    for(const auto &r : results) {
        if(r.has_value()) {
            if(r.value().outstanding()) {
                oc++;
            }
            all++;
        }
    }
    if(all == 0)
        return 0;

    return oc / all;
}

double calcOutstandingCount(const Scheduler::ResultVector &results) {
    size_t oc = 0;
    for(const auto &r : results) {
        if(r.has_value()) {
            if(r.value().outstanding()) {
                oc++;
            }
        }
    }
    return oc;
}

template<typename SchedulerType, typename ...Args>
QPair<QVector<double>, QVector<double>> generateOutstandingSeq(Args... args) {
    QVector<double> x, y;
    for(size_t i = 0; i < 100; ++i) {
        double intencity = i;

        Executor executor;
        executor.addErlangFlow(ErlangFlow::fromIntencity("fourier", intencity, 48, [](){
            return new FourierTask("fourier");
        }));
        auto er = executor.exec<SchedulerType>(2, 1000, args...);
        x.push_back(intencity);
        y.push_back(calcOutstandingPercent(er));
    }
    return { x, y };
}

int drawPlot(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto xy = generateOutstandingSeq<RoundRobinScheduler>(10);


    QCustomPlot customPlot;

    // create graph and assign data to it:
    customPlot.addGraph();
    customPlot.graph(0)->setData(xy.first, xy.second);
    // give the axes some labels:
    customPlot.xAxis->setLabel("intencity");
    customPlot.yAxis->setLabel("op");
    // set axes ranges, so we see all data:
    customPlot.xAxis->setRange(0, 100);
    customPlot.yAxis->setRange(0, 1);
    customPlot.replot();

    customPlot.show();
    return app.exec();
}

int drawDiagram(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    Executor roundRobinExecutor;
    Executor erliestDeadlineFirstExecutor;


    {
        //roundRobinExecutor.addTask(new FourierTask("fourier0"), Time::nowMillis() + 3000);
        //roundRobinExecutor.addTask(new FourierTask("fourier1"), Time::nowMillis() + 2000);
        //roundRobinExecutor.addTask(new AutoCorrelationTask("autocorel0"), Time::nowMillis() + 3000);
        //roundRobinExecutor.addTask(new AutoCorrelationTask("autocorel1"), Time::nowMillis() + 2000);

        roundRobinExecutor.addErlangFlow(ErlangFlow::fromIntencity("fourier", 20, 48, [](){
            return new FourierTask("fourier");
        }));
    } {
        //erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier0"), Time::nowMillis() + 300);
        //erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier1"), Time::nowMillis() + 1000);
        //erliestDeadlineFirstExecutor.addTask(new AutoCorrelationTask("autocorel0"), Time::nowMillis() + 200);
        //erliestDeadlineFirstExecutor.addTask(new AutoCorrelationTask("autocorel1"), Time::nowMillis() + 500);
    }

    auto rrResult = roundRobinExecutor.exec<RoundRobinScheduler>(2, 1000, 10);

    qDebug() << "rr oc:" << calcOutstandingCount(rrResult);
    qDebug() << "rr op:" << (calcOutstandingPercent(rrResult) * 100) << "%";

    printResults(erliestDeadlineFirstExecutor.exec<ErliestDeadlineFirstScheduler>(2, 1000));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("rrExec", &roundRobinExecutor);
    engine.rootContext()->setContextProperty("edfExec", &erliestDeadlineFirstExecutor);
    const QUrl url(QStringLiteral("qrc:/resources/main.qml"));
    engine.load(url);

    return app.exec();
}

int main(int argc, char *argv[]) {
    Task::printWcet(new FourierTask("Fourier"));
    Task::printWcet(new AutoCorrelationTask("AutoCorrelation"));

    return drawPlot(argc, argv);
}

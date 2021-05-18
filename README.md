# scheduler

# initial data
```
//addTask(task, deadline)
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
```
# timeline diagram of tasks executing
![alt text](https://github.com/burbokop/scheduler/blob/master/result_diagram.png)

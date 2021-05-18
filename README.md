# scheduler

# initial data
```
{
  roundRobinExecutor.addTask(new FourierTask("fourier0"), Time::nowMillis() + 3000);
  roundRobinExecutor.addTask(new FourierTask("fourier1"), Time::nowMillis() + 2000);
  roundRobinExecutor.addTask(new FourierTask("fourier2"), Time::nowMillis() + 3000);
  roundRobinExecutor.addTask(new FourierTask("fourier3"), Time::nowMillis() + 2000);
} {
  erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier0"), Time::nowMillis() + 300);
  erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier1"), Time::nowMillis() + 1000);
  erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier2"), Time::nowMillis() + 200);
  erliestDeadlineFirstExecutor.addTask(new FourierTask("fourier3"), Time::nowMillis() + 500);
}
```
# timeline diagram of tasks executing
![alt text](https://github.com/burbokop/scheduler/blob/master/result_diagram.png)

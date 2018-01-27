# single-threaded-scheduler
Summary:
The purpose of this project is to create a facility that can execute periodic recurrent tasks. This
facility behaves like a single threaded scheduler for running repeating tasks.

High Level Description:
The Periodic executor will execute tasks added to it. Each task can be executed more than one
time. The period of the recurrence is set per task and each task can specify if it needs more
execution cycles or it has finished.
The executor will arrange for the tasks to be executed according to following algorithm:
Whenever the executor run function is called, each task will be assigned a number representing the
next time it will be executed. Basically this is the time now plus the specified interval.
The executor will chose the task that it’s time to execute is the nearest and call it’s function when
the time has come.
Once the task function has returned, then if it needs another run, the time for the next time is
calculated based on finish time + interval and it is inserted for another cycle. otherwise, the task is
removed.

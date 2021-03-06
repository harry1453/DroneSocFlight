#include "Task.h"
#include "Scheduler.h"

namespace Scheduler {
    Task::Task(TaskFunction* task, const Name& name) : name(name), averageRunTime(0), maxRunTime(0), task(task) {
        allTasks.push_back(this);
    }

    void Task::run() {
        uint32_t start = micros();
        this->task();
        uint32_t runTime = micros() - start;
        if (this->maxRunTime < runTime) this->maxRunTime = runTime;
        this->averageRunTime.add(runTime);
    }
}

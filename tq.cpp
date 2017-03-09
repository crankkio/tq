#include <queue>
#include "mbed.h"
#include "Timeout.h"
#include "tq.h"

namespace tq {

std::queue<Task*> taskQueue;

class Task {
public:
    Task(std::function<void()> callback, float periodSeconds = 0, float delaySeconds = 0) :
        callback(callback),
        periodSeconds(periodSeconds)
    {
        if (delaySeconds != 0) {
            timeout = new Timeout;
            timeout->attach(this, &Task::timeoutHandler, delaySeconds);
        } else {
            timeout = NULL;
        }
    }
    
    ~Task() {
        if (timeout)
            delete timeout;
    }
    
    bool scheduled() const {
        return timeout == NULL;
    }
    
    void detachTimeout() {
        if (timeout)
            timeout->detach();
    }
    
    void repeatTimeout() {
        if (!timeout)
            timeout = new Timeout;
        timeout->attach(this, &Task::timeoutHandler, periodSeconds);
    }
    
    bool repeating() {
        return periodSeconds != 0;
    }
    
    std::function<void()> callback;
    
private:
    void timeoutHandler() {
        delete timeout;
        timeout = NULL;
        __disable_irq();
        taskQueue.push(this);
        __enable_irq();
    }
    Timeout* timeout;
    float periodSeconds;
};

Handle::Handle() :
    task(NULL)
{}

Handle::Handle(Task* task) :
    task(task)
{}

bool Handle::active() const {
    return task != NULL;
}

void Handle::cancel() {
    NVIC_DisableIRQ(RTC1_IRQn);
    if (task != NULL && !task->scheduled()) {
        task->detachTimeout();
        delete task;
        task = NULL;
    }
    NVIC_EnableIRQ(RTC1_IRQn);
}

Handle schedule(std::function<void()> callback) {
    Task* task = new Task(callback);
    __disable_irq();
    taskQueue.push(task);
    __enable_irq();
    return Handle(task);
}

Handle delayedSchedule(std::function<void()> callback, float delaySeconds) {
    Task* task = new Task(callback, 0, delaySeconds);
    return Handle(task);
}

Handle repeat(std::function<void()> callback, float periodSeconds) {
    Task* task = new Task(callback, periodSeconds);
    __disable_irq();
    taskQueue.push(task);
    __enable_irq();
    return Handle(task);
}

Handle delayedRepeat(std::function<void()> callback, float periodSeconds, float delaySeconds) {
    Task* task = new Task(callback, periodSeconds, delaySeconds);
    return Handle(task);
}

Task* getFrontTask() {
    __disable_irq();
    Task* task = NULL;
    if (!taskQueue.empty())
        task = taskQueue.front();
    __enable_irq();
    return task;
}

void dispatch() {
    Task* task;
    while ((task = getFrontTask()) != NULL) {
        task->callback();
        if (task->repeating())
            task->repeatTimeout();
        else
            delete task;
        __disable_irq();
        taskQueue.pop();
        __enable_irq();
    }
}

}

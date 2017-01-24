#include <cstddef>
#include <queue>
#include "tq.h"
#include "TqTask.h"

namespace tq {

    std::queue<Task*> tasks;
    std::queue<Task*> nextTick;

    unsigned int counter = 0;
    unsigned int (*getTimestamp)(void);

    void init(unsigned int (*fn)(void)) {
        getTimestamp = fn;
    }

    Task* schedule(void (*callback)(void)) {
        Task* task = new Task(counter++, callback);

        task->immediate = true;

        tasks.push(task);
        return task;
    }

    Task* schedule(void (*callback)(void), unsigned int delay) {
        Task* task = new Task(counter++, callback);

        task->delay = delay;
        task->timestamp = getTimestamp();

        tasks.push(task);
        return task;
    }

    Task* repeat(void (*callback)(void)) {
        Task* task = new Task(counter++, callback);

        task->repeat = true;
        task->immediate = true;

        tasks.push(task);
        return task;
    }

    Task* repeat(void (*callback)(void), unsigned int interval) {
        Task* task = new Task(counter++, callback);

        task->delay = interval;
        task->repeat = true;
        task->timestamp = getTimestamp();

        tasks.push(task);
        return task;
    }

    void dispatch(void) {
        unsigned int i   = tasks.size(),
                     now = getTimestamp();

        // iterate current turn
        while (i--) {
            Task* task = tasks.front();
            tasks.pop();

            // canceled - free
            if (task->canceled()) {
                delete task;
                task = NULL;
                continue;
            }
            // ready to execute
            else if (task->check(now)) {
                task->callback();

                // repeating - re-schedule
                if (task->repeat) {
                    task->timestamp = now;
                    nextTick.push(task);
                }
                // executed once - free
                else {
                    delete task;
                    task = NULL;
                }
            }
            // not current - re-enqueue
            else
                nextTick.push(task);
        }

        // enqueue next turn
        while (!nextTick.empty()) {
            tasks.push(nextTick.front());
            nextTick.pop();
        }
    }

}

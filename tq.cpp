#include <cstddef>
#include <queue>
#include "tq.h"
#include "TqTask.h"

namespace tq {

    unsigned int counter = 0;
    std::queue<Task*> tasks;
    std::queue<Task*> nextTick;
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

    Task* repeat(void (*callback)(void), unsigned int delay) {
        Task* task = new Task(counter++, callback);

        task->delay = delay;
        task->repeat = true;
        task->timestamp = getTimestamp();

        tasks.push(task);
        return task;
    }

    void dispatch() {
        unsigned int i   = tasks.size(),
                     now = getTimestamp();

        while (i--) {
            Task* task = tasks.front();
            tasks.pop();

            if (task->canceled()) {
                delete task;
                task = NULL;
                continue;
            }
            else if (task->check(now)) {
                task->callback();

                if (task->repeat) {
                    task->timestamp = now;
                    nextTick.push(task);
                }
                else {
                    delete task;
                    task = NULL;
                }
            }
            else
                nextTick.push(task);
        }

        // schedule next turn
        while (!nextTick.empty()) {
            tasks.push(nextTick.front());
            nextTick.pop();
        }
    }

}

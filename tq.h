#ifndef TQ_H
#define TQ_H

#include <functional>
#include "Timeout.h"

namespace tq {

class Task;

class Handle {
public:
    Handle();
    Handle(Task* task);
    bool active() const;
    void cancel();
private:
    Task* task;
};

Handle schedule(std::function<void(void)> callback);
Handle delayedSchedule(std::function<void(void)> callback, float delaySeconds);
Handle repeat(std::function<void(void)> callback, float periodSeconds);
Handle delayedRepeat(std::function<void(void)> callback, float periodSeconds, float delaySeconds);

void dispatch();

}

#endif



#ifndef TQ_TASK_H
#define TQ_TASK_H

namespace tq {

class Task {
public:
    Task(unsigned int id, void (*callback)(void)) {
        this->id = id;
        this->repeat = false;
        this->canceled_ = false;
        this->immediate = false;
        this->callback = callback;
    }

    virtual ~Task() {}

    void (*callback)(void);
    bool immediate, repeat;
    unsigned int id, delay, timestamp;

    bool check(unsigned int now) {
        // no delay
        if (immediate)
            return true;
        // time counter overflood
        else if (now < timestamp)
            return true;
        // delay elapsed
        else if (now >= timestamp + delay)
            return true;
        else
            return false;
    }

    void cancel(void) {
        this->canceled_ = true;
    }

    bool canceled() {
        return this->canceled_;
    }

private:
    bool canceled_;
};

}

#endif

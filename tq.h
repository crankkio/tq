#ifndef TQ_H
#define TQ_H

#include "TqTask.h"

namespace tq {

    void init(unsigned int (*fn)(void));

    Task* schedule(void (*fn)(void));
    Task* schedule(void (*fn)(void), unsigned int delay);

    Task* repeat(void (*fn)(void));
    Task* repeat(void (*fn)(void), unsigned int interval);

    void dispatch(void);

}

#endif

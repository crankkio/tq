# tq

Simple C++ task queue implementation.

# Example

Handle interruptions and schedule a task without blocking `wait()` calls.

```c++

#include "tq.h"
using namespace tq;

unsigned int getTimestamp() {
    // return an int which increases over time
    // (e.g. a unix-like timestamp)
}

void turnVibrationOn() {
    // turn on vibration hardware
}

void turnVibrationOff() {
    // turn off vibration hardware
}

void refreshDisplay() {
    // re-render UI and send data to the display
}

// turn on vibration for one second on button press
// without blocking the UI
// (assuming that getTimestamp() counts seconds)
void onButtonPressedInterrupt() {
    schedule(&turnVibrationOn);
    schedule(&turnVibrationOff, 1);
}

int main() {
    init(&getTimestamp);

    // execute once in each turn
    repeat(&refreshDisplay);

    while (1)
        dispatch();
}

```

# API

## Methods

### `void init(unsigned int (*getTimestamp)(void))`

### `Task* schedule(void (*callback)(void))`
### `Task* schedule(void (*callback)(void), unsigned int delay)`

### `Task* repeat(void (*callback)(void))`
### `Task* repeat(void (*callback)(void), unsigned int delay)`

### `void dispatch()`

## Class: `Task`

`Task* task = schedule(&callback, 1);`

### `unsigned int task->id`
### `unsigned int task->delay`
### `bool task->repeat`
### `bool task->immediate`

### `void task->callback()`
### `void task->cancel()`
### `bool task->canceled()`

# tq

Simple C++ task queue implementation.

# Example

Handle interrupts and schedule a task without blocking `wait()` calls.

```c++

#include "tq.h"

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
void onButtonPressedInterrupt() {
    tq::schedule(&turnVibrationOn);
    tq::delayedSchedule(&turnVibrationOff, 1);
}

int main() {
    // execute every second
    repeat(&refreshDisplay, 1);

    while (1) {
        tq::dispatch();
        __WFI(); // wait for interrupts
    }
}

```

# API

## Methods

### `Handle schedule(std::function<void(void)> callback)`
### `Handle delayedSchedule(std::function<void(void)> callback, float delaySeconds)`

### `Handle repeat(std::function<void(void)> callback, float periodSeconds)`
### `Handle delayedRepeat(std::function<void(void)> callback, float periodSeconds, float delaySeconds)`

### `void dispatch()`


## Class: `Handle`

`Handle handle = delayedSchedule(&callback, 1);`

### `void handle->cancel()`
### `bool handle->active()`

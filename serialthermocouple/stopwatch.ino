#include "stopwatch.h"

Stopwatch::Stopwatch(unsigned long tDelay_us) {
    tDelay = tDelay_us;
    timer = micros();
}

bool Stopwatch::is_ready(void) {
  /* Return true if the amount of time elapsed is greater than the internal delay */
    return ((micros() - timer) > tDelay);
}

void Stopwatch::set_delay(unsigned long tDelay_us) {
  tDelay = tDelay_us;
}

unsigned long Stopwatch::last_time(void) {
  return timer;
}

 bool Stopwatch::call(void (*start_routine) ()) {
   /* Call a function if `timer` has elapsed since the last call. This is the basic version, 
   it just takes a plain void function (takes no args and returns nothing) */
    bool ready = is_ready();

    if (ready) {
        timer = micros();
        start_routine();
    }
    return ready;
 };

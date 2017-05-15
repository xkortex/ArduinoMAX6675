#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

class Stopwatch {
    private:
     unsigned long timer;
     unsigned long tDelay;
    public:
     Stopwatch(unsigned long tDelay_us);
     void set_delay(unsigned long tDelay_us);
     bool is_ready(void);
     unsigned long last_time(void);
     bool call(void *(*start_routine) (void *argt), void *args);
     bool call(void (*start_routine) ());
     void *call(void *(*start_routine) ());


};


#endif // _STOPWATCH_H_

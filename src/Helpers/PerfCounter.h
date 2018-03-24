#ifndef PERFCOUNTER_H
#define PERFCOUNTER_H

#include "./EMA.h"
#include "../Settings/Settings.h"
#include "./StreamOut.h"
#include <Arduino.h>

class PerfCounter
{
  public:
    PerfCounter(const char *name) : name(name) {}
    void start()
    {
        startTime = micros();
    }
    unsigned long stop()
    {
        unsigned long duration = micros() - startTime;
        unsigned long avg = averageDuration.filter(duration);
#ifdef DEBUG_TIME
        if (name == nullptr) {
            DEBUG_OUT << "PerfCounter:";
        } else {
            DEBUG_OUT << name;
        }
        DEBUG_OUT << tab << "duration = " << duration 
                  << tab << "average = " << avg << endl;
        return duration;
#endif
    }

  private:
    unsigned long startTime = 0;
    const char *name = nullptr;
    EMA64<4> averageDuration;
};

#endif // PERFCOUNTER_H
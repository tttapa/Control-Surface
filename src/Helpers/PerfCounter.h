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
        counter++;
#ifdef DEBUG_TIME
        if (counter >= 100)
        {
            buffIndex += snprintf(&buff[buffIndex], maxBuffLen - buffIndex, "%s\tduration = %lu\taverage = %lu\r\n", name, duration, avg);
            counter = 0;
        }
#endif
        return duration;
    }

    static void print()
    {
#ifdef DEBUG_TIME
        DEBUG_OUT << buff << (buff[0] == '\0' ? "" : endl);
#endif
        buff[0] = '\0';
        buffIndex = 0;
    }

  private:
    unsigned long startTime = 0;
    const char *name = nullptr;
    EMA64<4> averageDuration;
    size_t counter = 0;

    static char buff[32768];
    const static size_t maxBuffLen = sizeof(buff) / sizeof(buff[0]) - 1;
    static size_t buffIndex;
};

#endif // PERFCOUNTER_H
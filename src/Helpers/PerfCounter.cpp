#include "PerfCounter.h"

char PerfCounter::buff[32768] = {'\0'};
size_t PerfCounter::buffIndex = 0;
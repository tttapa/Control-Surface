#include "StreamOut.h"

#ifdef ARDUINO
StreamOut_format_t streamOutFormat = dec;
StreamOut_leadingZeros_t streamOutLeadingZeros = noLeadingZeros;

const char *endl = "\r\n"; // TODO: should endl flush?
#endif

char tab = '\t';

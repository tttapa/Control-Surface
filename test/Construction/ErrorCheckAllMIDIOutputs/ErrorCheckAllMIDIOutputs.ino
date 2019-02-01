#include <Encoder.h>

#include <Control_Surface.h>

MIDICNChannelAddress a = {0x7F, CHANNEL_10, 4};
RelativeMIDICNChannelAddress r = {1, 1, 1};
Bank<4> b = {2, 0};
Channel c = CHANNEL_11;
pin_t p = 0;
uint8_t n = 0;

NoteButton aa = {p, a};
Bankable::NoteButton ab = {b, p, a};
CCButton ac = {p, a};
Bankable::CCButton ad = {b, p, a};

NoteButtonLatching ae = {p, a};
Bankable::NoteButtonLatching af = {b, p, a};
CCButtonLatching ag = {p, a};
Bankable::CCButtonLatching ah = {b, p, a};

NoteButtonLatched ai = {p, a};
Bankable::NoteButtonLatched aj = {b, p, a};
CCButtonLatched ak = {p, a};
Bankable::CCButtonLatched al = {b, p, a};

NoteButtonMatrix<3, 4> am = {
    {p, p, p},
    {p, p, p, p},
    {{
        {n, n, n, n},
        {n, n, n, n},
        {n, n, n, n},
    }},
    c,
};
Bankable::NoteButtonMatrix<3, 4> an = {
    b,
    {p, p, p},
    {p, p, p, p},
    {{
        {n, n, n, n},
        {n, n, n, n},
        {n, n, n, n},
    }},
    c,
};

CCButtonMatrix<3, 4> ao = {
    {p, p, p},
    {p, p, p, p},
    {{
        {n, n, n, n},
        {n, n, n, n},
        {n, n, n, n},
    }},
    c,
};
Bankable::CCButtonMatrix<3, 4> ap = {
    b,
    {p, p, p},
    {p, p, p, p},
    {{
        {n, n, n, n},
        {n, n, n, n},
        {n, n, n, n},
    }},
    c,
};

NoteButtons<3> aq = {
    {p, p, p},
    a,
    r,
};
Bankable::NoteButtons<3> ar = {
    b,
    {p, p, p},
    a,
    r,
};

CCButtons<3> as = {
    {p, p, p},
    a,
    r,
};
Bankable::CCButtons<3> at = {
    b,
    {p, p, p},
    a,
    r,
};

CCIncrementDecrementButtons au = {
    {p, p},
    a,
    1,
    a,
};
Bankable::CCIncrementDecrementButtons av = {
    b, {p, p}, a, 1, a,
};

CCRotaryEncoder aw = {{0, 0}, a, 1, 4};
Bankable::CCRotaryEncoder ax = {b, {0, 0}, a, 1, 4};

CCPotentiometer ay = {p, a};
Bankable::CCPotentiometer az = {b, p, a};

PBPotentiometer ba = {p, c};
Bankable::PBPotentiometer bb = {b, p, c};

// -------------------------------------------------------------------------- //

void setup(){};
void loop(){};
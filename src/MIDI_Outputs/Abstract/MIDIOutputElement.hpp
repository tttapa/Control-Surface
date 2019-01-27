#pragma once

#include <Def/Def.hpp>
#include <Def/MIDICNChannelAddress.hpp>
#include <Helpers/Updatable.hpp>

struct MIDIOutputElement : public Updatable<> {};
struct MIDIOutputElementPotentiometer : public Updatable<Potentiometer> {};
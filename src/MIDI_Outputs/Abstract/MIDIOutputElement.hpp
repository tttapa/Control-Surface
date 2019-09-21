#pragma once

#include <Def/Def.hpp>
#include <Def/MIDICNChannelAddress.hpp>
#include <Helpers/Updatable.hpp>

BEGIN_CS_NAMESPACE

using MIDIOutputElement = Updatable<>;
using MIDIOutputElementPotentiometer = Updatable<Potentiometer>;

END_CS_NAMESPACE
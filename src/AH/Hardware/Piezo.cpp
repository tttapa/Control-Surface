#include "Piezo.hpp"

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

BEGIN_AH_NAMESPACE

using namespace ExtIO;

// Helper function - this is built in to the Arduino IDE, but GitHub doesn't build successfully without it.
long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Piezo::Piezo(pin_t pin) : pin(pin) { }

void Piezo::begin() { ExtIO::pinMode(pin, INPUT); }

Piezo::State Piezo::update() {
    uint16_t input = ExtIO::analogRead(pin);
    unsigned long now = millis();
	
    if (now - startTime > strikeDuration) {
        uint16_t currentThreshold = threshold;
        
        // Fancy Debouncing. Basically, it raises the threshold high up for a moment after
        // the hit. That way, if you hit it really fast, it can sense a second hard hit rather than ignoring
        // all hits for a set amount of time.
        // TODO: Test and optimize. Working, but commented out until testing is done. 
		// if (recentlyHit) { 
        //     // below algorithm obtained from
        //     // https://beammyselfintothefuture.wordpress.com/2015/01/28/sensing-hit-velocity-and-quick-subsequent-hits-of-a-piezo-with-an-arduino-teensy/
		// 	uint16_t dynamicThreshold = (highestYet >> ((now - highestInputTime) >> thresholdDecaySpeed)) * secondHitThreshold;
			
		// 	if (dynamicThreshold < threshold) recentlyHit = false;
		// 	else currentThreshold = dynamicThreshold;
		// }
            
        // Max Velocity found for previous hit
        if (struck) { 
			state = Struck; // notify the system of the strike
			velocity = map(highestYet, 0, maxInput, 40, 127); // TODO: switch 127 for maximum volume
			struck = false; // reset
            recentlyHit = true;
		}
        // New Hit                                      // Simple Debouncing.
		else if (input > currentThreshold && now - highestInputTime > DebounceTime) { // new hit
			state = Playing; // Be free of bugs! Don't notify the system until we have max velocity
			struck = true;
			startTime = now;
            highestYet = 0;
		}
        // No hit
		else {
            // Send TurnOff after note duration
            if (state == Playing) {
                if (now - startTime > playDuration) {
                    state = TurnOff;
                }
            // Idle otherwise
            } else {
                state = Idle;
            }
		}
    }
	else { // Reading the input after a hit. Find the highest value for input. Will run a few times
		if (input > highestYet) {
			highestYet = input;
			highestInputTime = now;
		}
		
		//FEATURE - Auto adjust max possible input. 
        // Means notes will rarely reach full volume
		if (input > maxInput) {
            maxInput = input;
        }
	}
	return state;
}

Piezo::State Piezo::getState() const { return state; }

void Piezo::dropState() { state = Idle; }

uint8_t Piezo::getVelocity() const { return velocity; }

uint16_t Piezo::sampleSilence(uint16_t sampleTime) {
    uint16_t maxSilenceVolume = 0;

    unsigned long now = millis();
    startTime = now;

    while (now - startTime < sampleTime) {
        uint16_t input = ExtIO::analogRead(pin);
        if (input > maxSilenceVolume) {
            maxSilenceVolume = input;
        }
        now = millis();
    }

    return maxSilenceVolume;
}

void Piezo::setHitThreshold(uint16_t hitThreshold) {
    this->threshold = hitThreshold;
}

FlashString_t Piezo::getName(Piezo::State state) {
    switch (state) {
        case Piezo::Struck: return F("Struck");
        case Piezo::Idle: return F("Idle");
        case Piezo::Playing: return F("Playing");
        case Piezo::TurnOff: return F("TurnOff");
        default: return F("<invalid>"); // Keeps the compiler happy
    }
}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()

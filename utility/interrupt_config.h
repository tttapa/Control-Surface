#if defined(__AVR__)

#include <avr/io.h>
#include <avr/interrupt.h>

#define attachInterrupt(num, func, mode) enableInterrupt(num)
#define detachInterrupt(num) disableInterrupt(num)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define CS_SCRAMBLE_INT_ORDER(num) ((num < 4) ? num + 2 : ((num < 6) ? num - 4 : num))
#define CS_DESCRAMBLE_INT_ORDER(num) ((num < 2) ? num + 4 : ((num < 6) ? num - 2 : num))
#else
#define CS_SCRAMBLE_INT_ORDER(num) (num)
#define CS_DESCRAMBLE_INT_ORDER(num) (num)
#endif

#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

static void enableInterrupt(uint8_t num)
{
	switch (CS_DESCRAMBLE_INT_ORDER(num)) {
		#if defined(EICRA) && defined(EIMSK)
		case 0:
			EICRA = (EICRA & 0xFC) | 0x01;
			EIMSK |= 0x01;
			return;
		case 1:
			EICRA = (EICRA & 0xF3) | 0x04;
			EIMSK |= 0x02;
			return;
		case 2:
			EICRA = (EICRA & 0xCF) | 0x10;
			EIMSK |= 0x04;
			return;
		case 3:
			EICRA = (EICRA & 0x3F) | 0x40;
			EIMSK |= 0x08;
			return;
		#elif defined(MCUCR) && defined(GICR)
		case 0:
			MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
			GICR |= (1 << INT0);
			return;
		case 1:
			MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
			GICR |= (1 << INT1);
			return;
		#elif defined(MCUCR) && defined(GIMSK)
		case 0:
			MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
			GIMSK |= (1 << INT0);
			return;
		case 1:
			MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
			GIMSK |= (1 << INT1);
			return;
		#endif
		#if defined(EICRB) && defined(EIMSK)
		case 4:
			EICRB = (EICRB & 0xFC) | 0x01;
			EIMSK |= 0x10;
			return;
		case 5:
			EICRB = (EICRB & 0xF3) | 0x04;
			EIMSK |= 0x20;
			return;
		case 6:
			EICRB = (EICRB & 0xCF) | 0x10;
			EIMSK |= 0x40;
			return;
		case 7:
			EICRB = (EICRB & 0x3F) | 0x40;
			EIMSK |= 0x80;
			return;
		#endif
	}
}

static void disableInterrupt(uint8_t num)
{
	switch (CS_DESCRAMBLE_INT_ORDER(num)) {
		#if defined(EICRA) && defined(EIMSK)
		case 0:
			EIMSK &= ~0x01;
			return;
		case 1:
			EIMSK &= ~0x02;
			return;
		case 2:
			EIMSK &= ~0x04;
			return;
		case 3:
			EIMSK &= ~0x08;
			return;
		#elif defined(MCUCR) && defined(GICR)
		case 0:
			GICR &= ~(1 << INT0);
			return;
		case 1:
			GICR &= ~(1 << INT1);
			return;
		#elif defined(MCUCR) && defined(GIMSK)
		case 0:
			GIMSK &= ~(1 << INT0);
			return;
		case 1:
			GIMSK &= ~(1 << INT1);
			return;
		#endif
		#if defined(EICRB) && defined(EIMSK)
		case 4:
			EIMSK &= ~0x10;
			return;
		case 5:
			EIMSK &= ~0x20;
			return;
		case 6:
			EIMSK &= ~0x40;
			return;
		case 7:
			EIMSK &= ~0x80;
			return;
		#endif
	}
}

BEGIN_CS_NAMESPACE

#elif defined(__PIC32MX__)

#ifdef CS_ENCODER_OPTIMIZE_INTERRUPTS
#undef CS_ENCODER_OPTIMIZE_INTERRUPTS
#endif

#else

#ifdef CS_ENCODER_OPTIMIZE_INTERRUPTS
#undef CS_ENCODER_OPTIMIZE_INTERRUPTS
#endif

#endif

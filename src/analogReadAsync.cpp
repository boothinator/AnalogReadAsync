// Asynchronous Analog Read
// Copyright (C) 2021  Joshua Booth

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "analogReadAsync.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>
#include <pins_arduino.h>

static volatile analogReadCompleteCallback_t analogReadCompleteCallback = nullptr;
static const void * volatile analogReadCompleteCallbackData = nullptr;

bool getAutoTriggeringEnabled()
{
	return (ADCSRA & _BV(ADATE)) == _BV(ADATE);
}

bool inFreeRunningMode()
{
	return (ADCSRB & (_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0))) == 0;
}

void analogReadAsync(uint8_t pin, analogReadCompleteCallback_t cb, const void *data)
{
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#endif
	pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
	if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
	if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#else
	if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

#if defined(ADCSRB) && defined(MUX5)
	// the MUX5 bit of ADCSRB selects whether we're reading from channels
	// 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = (1 << REFS0) | (pin & 0x07);
#else
	ADMUX = (1 << REFS0) | (pin & 0x07);
#endif

  // Set the callback
  analogReadCompleteCallback = cb;
	analogReadCompleteCallbackData = data;

	// Only start conversion if auto-triggering is disabled or in free-running mode
	if (!getAutoTriggeringEnabled() || inFreeRunningMode())
	{
		// start the conversion
		ADCSRA |= (1 << ADSC);
	}

	// enable the interrupt
	if (cb)
	{
		ADCSRA |= (1 << ADIE);
	}
}

ISR(ADC_vect)
{
  analogReadCompleteCallback_t cb = analogReadCompleteCallback;
	const void *data = analogReadCompleteCallbackData;

	// Only disable interrupt if auto-triggering is disabled
	if (!getAutoTriggeringEnabled())
	{
		// Disable interrupt and clear global callback and data before calling to allow for another call from the callback
		ADCSRA &= ~(1 << ADIE);
		analogReadCompleteCallback = nullptr;
		analogReadCompleteCallbackData = nullptr;
	}
	
  if (cb)
  {
    cb(ADC, const_cast<void *>(data));
  }
}

bool getAnalogReadComplete(bool clearInterruptFlag)
{
	bool complete;
	if (getAutoTriggeringEnabled())
	{
		complete = (ADCSRA & _BV(ADIF)) == _BV(ADIF);
	}
	else
	{
		complete = (ADCSRA & _BV(ADSC)) == 0;
	}

	if (complete && clearInterruptFlag)
	{
		ADCSRA |= _BV(ADIF);
	}

	return complete;
}

uint16_t getAnalogReadValue()
{
	uint16_t tmp;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		tmp = ADC;
	}

	return tmp;
}

void setAnalogReadFreeRunning(bool enable)
{
	if (enable)
	{
		// Enable auto-trigger
		ADCSRA |= _BV(ADATE);

		// Set to free-running mode
		ADCSRB &=  ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0));
	}
	else
	{
		// Disable auto-trigger
		ADCSRA &= ~_BV(ADATE);
	}
}

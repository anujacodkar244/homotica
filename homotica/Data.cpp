#include "Data.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

Data::Data(void)
{
	isUsed = EMPTY_DATA;
}

void Data::refresh(void)
{
	unsigned long now = millis();
	if (now - creationTime >= duration)
	{
		digitalWrite(pin, !startingValue);
		isUsed = EMPTY_DATA;
	}
}

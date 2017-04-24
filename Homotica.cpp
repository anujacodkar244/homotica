#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Homotica.h"

Homotica::Homotica(void)
{}

int8_t Homotica::pushPin(uint8_t pin, unsigned long duration, uint8_t startingValue)
{
	int8_t i = findSuitableDataIndex();
	if (i == NOTHING_AVAILABLE) return NOTHING_AVAILABLE;
	_datas[i].isUsed = USED_DATA;
	_datas[i].pin = pin;
	_datas[i].duration = duration;
	_datas[i].startingValue = !startingValue;
	_datas[i].creationTime = millis();
	digitalWrite(pin, !startingValue);
	return i;
}

void Homotica::refresh(void)
{
	for (int8_t i = 0; i < MAX_ITEMS; i++)
	{
		if (_datas[i].isUsed == USED_DATA)
		{
			_datas[i].refresh();
		}
	}
}

int8_t Homotica::findSuitableDataIndex(void)
{
	for (int8_t i = 0; i < MAX_ITEMS; i++)
	{
		if (_datas[i].isUsed == EMPTY_DATA)
		{
			return i;
		}
	}
	return NOTHING_AVAILABLE;
}

#ifndef Homotica_h
#define Homotica_h

#include <inttypes.h>
#include "Data.h"

#define MAX_ITEMS (15)
#define NOTHING_AVAILABLE (-1)

class Homotica
{
public:
	Homotica(void);
	int8_t pushPin(uint8_t pin, unsigned long duration, uint8_t startingValue);
	void refresh(void);
protected:
	Data _datas[MAX_ITEMS];
	int8_t findSuitableDataIndex(void);
};
#endif

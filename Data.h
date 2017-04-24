#ifndef Data_h
#define Data_h

#include <inttypes.h>

#define EMPTY_DATA 0
#define USED_DATA 1

class Data
{

public:
	Data(void);
	void refresh(void);
	int8_t isUsed;
	unsigned long duration;
	uint8_t pin;
	uint8_t startingValue;
	unsigned long creationTime;
};
#endif
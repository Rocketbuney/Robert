#ifndef HCSR04_H
#define HCSR04_H

#include "../RPI/RPI.h"

#include <pthread.h>

struct HCSR04_Sensor {
	__UINT8_TYPE__ trig, echo;
	volatile float dist;
	pthread_t thread;
};

void HCSR04_init(struct HCSR04_Sensor *sensor, __UINT8_TYPE__ trig, __UINT8_TYPE__ echo);
void HCSR04_deInit(struct HCSR04_Sensor *sensor);

#endif

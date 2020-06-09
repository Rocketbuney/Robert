#include "HCSR04.h"

#include "../RPI/RPI.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

static void HCSR04_thread(struct HCSR04_Sensor *sensor) {
	long pulseStart, pulseEnd, pulseDuration;	

	for(;;) {
		/* Pulse trigger pin */
		GPIO_SET = 1 << sensor->trig;	
		delayMicroseconds(10);
		GPIO_CLR = 1 << sensor->trig;
		delayMicroseconds(1);

		/* Accurately get pulse start and end */
		while(GPIO_READ(sensor->echo) == 0)
			pulseStart = micros();  /* get time */	
		while(GPIO_READ(sensor->echo) > 1)
			pulseEnd = micros();  /* get time */

		/* calculate distance */
		sensor->dist = (((pulseEnd - pulseStart) / 2) / 29) < 0 ? 0 : ((pulseEnd - pulseStart) / 2) / 29;
	}
}

void HCSR04_init(struct HCSR04_Sensor *sensor, __UINT8_TYPE__ trig, __UINT8_TYPE__ echo) {
	sensor = (struct HCSR04_Sensor*)malloc(sizeof(struct HCSR04_Sensor));
	memset(sensor, 0, sizeof(*sensor));

	sensor->trig = trig;
	sensor->echo = echo;
	sensor->dist = 0;
	sensor->thread = 0;

	INP_GPIO(sensor->trig);
	OUT_GPIO(sensor->trig);
	INP_GPIO(sensor->echo);

	GPIO_CLR = 1 << sensor->trig;
	GPIO_CLR = 1 << sensor->echo;

	printf("Sensor Balancing.\n");

	pthread_create(&sensor->thread, NULL, (void *)HCSR04_thread, sensor);
}

void HCSR04_deInit(struct HCSR04_Sensor *sensor) {
	pthread_detach(pthread_self());
		
	GPIO_CLR = 1 << sensor->trig;
	GPIO_CLR = 1 << sensor->echo;

	INP_GPIO(sensor->trig);
	INP_GPIO(sensor->echo);
}

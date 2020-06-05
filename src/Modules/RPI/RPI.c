//
//  RPI.c
//  Robert
//
//  Created by Jack Thake on 6/1/20.
//  Copyright © 2020 Jack Thake. All rights reserved.
//

#include "RPI.h"

#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include <pthread.h>
#include <stdlib.h>

#include <unistd.h>

volatile struct bcm2835_peripheral gpio = { GPIO_BASE, 0, 0, 0 };

// Exposes the physical address defined in the passed structure using mmap on /dev/mem
int rpi_mapPeripheral(volatile struct bcm2835_peripheral *p) {
   // Open /dev/mem
   if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("Failed to open /dev/mem, try checking permissions.\n");
      return -1;
   }

   p->map = mmap(
      NULL,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED,
      p->mem_fd,     // File descriptor to physical memory virtual file '/dev/mem'
      p->addr_p      // Address in physical map that we want this memory block to expose
   );

   if (p->map == MAP_FAILED) {
        perror("mmap");
        return -1;
   }

   p->addr = (volatile unsigned int *)p->map;
   return 0;
}

void rpi_unmapPeripheral(volatile struct bcm2835_peripheral *p) {
    munmap(p->map, BLOCK_SIZE);
    close(p->mem_fd);
} 

// Software PWM Implementation
static volatile int marks         [MAX_PINS];
static volatile int range         [MAX_PINS];
static volatile pthread_t threads [MAX_PINS];
static volatile int newPin		  = -1;

static void delayMicrosecondsHard (unsigned int howLong) { 
	struct timeval tNow, tLong, tEnd;

	gettimeofday (&tNow, NULL);
	tLong.tv_sec  = howLong / 1000000;
	tLong.tv_usec = howLong % 1000000;
	timeradd (&tNow, &tLong, &tEnd);

	while (timercmp (&tNow, &tEnd, <))
		gettimeofday (&tNow, NULL);
}

static void delayMicroseconds (unsigned int howLong) {
	struct timespec sleeper;
	unsigned int uSecs = howLong % 1000000;
	unsigned int wSecs = howLong / 1000000;

	if (howLong == 0)
		return;
	else if (howLong  < 100)
		delayMicrosecondsHard (howLong);
	else {
		sleeper.tv_sec  = wSecs;
		sleeper.tv_nsec = (long)(uSecs * 1000L);
		nanosleep (&sleeper, NULL);
	}
}

static void *softPwmThread (void *arg) {
	int pin, mark, space;
	struct sched_param param;

	param.sched_priority = sched_get_priority_max (SCHED_RR);
	pthread_setschedparam (pthread_self (), SCHED_RR, &param);

	pin = *((int *)arg);
	free (arg);

	pin    = newPin;
	newPin = -1;

	// piHiPri (90);

	for (;;) {
		mark  = marks [pin];
		space = range [pin] - mark;

		if (mark != 0)
			GPIO_SET = 1 << pin;
		delayMicroseconds (mark * 100);

		if (space != 0)
			GPIO_CLR = 1 << pin;
		delayMicroseconds (space * 100);
	}

  return NULL ;
}

int rpi_softPWMCreate(int pin, int initialValue, int pwmRange) {
	int res;
	pthread_t myThread;
	int *passPin;

	if (pin >= MAX_PINS)
		return -1;

	if (range [pin] != 0)	// Already running on this pin
		return -1;

	if (pwmRange <= 0)
		return -1;

	passPin = malloc (sizeof (*passPin));
	if (passPin == NULL)
		return -1;

	GPIO_CLR = 1 << pin;
	INP_GPIO(pin);
	OUT_GPIO(pin);

	marks [pin] = initialValue;
	range [pin] = pwmRange;

	*passPin = pin;
	newPin   = pin;
	res      = pthread_create (&myThread, NULL, softPwmThread, (void *)passPin);

	while (newPin != -1)
		delayMicroseconds(1000);

	threads [pin] = myThread;
	return res;
}

void rpi_softPWMWrite(int pin, int value) {
	if (pin < MAX_PINS) {
		if (value < 0)
			value = 0;
		else if (value > range [pin])
			value = range [pin];

		marks [pin] = value;
	}
}

void rpi_softPWMStop(int pin) {
	if (pin < MAX_PINS) {
		if (range [pin] != 0) {
			pthread_cancel (threads [pin]);
			pthread_join   (threads [pin], NULL);
			range [pin] = 0;
			GPIO_CLR = 1 << pin;	
		}
	}
}


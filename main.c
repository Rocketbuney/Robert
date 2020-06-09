//
//	main.c
//	Robert
//
//	Created by Jack Thake on 6/1/20.
//	Copyright © 2020 Jack Thake. All rights reserved.
//


#include "Modules/RPI/RPI.h"
#include "Modules/TB6612/TB6612.h"
#include "Modules/HCSR04/HCSR04.h"
#include <stdio.h>
#include <unistd.h>

static void initCPUFan() {
	INP_GPIO(3);
	OUT_GPIO(3);

	rpi_softPWMCreate(3, 0, 100);
}

static void manageCPUTemps() {
	float systemp, millideg;
	FILE *thermal;
	int n;

	thermal = fopen("/sys/class/thermal/thermal_zone0/temp","r");
	n = fscanf(thermal, "%f", &millideg);
	fclose(thermal);
	systemp = millideg / 1000;

	if(systemp < 49.9) {
		rpi_softPWMWrite(3, 25);
	} else if(systemp >= 40.0 && systemp < 44.9) {
		rpi_softPWMWrite(3, 50);
	} else if(systemp >= 45.0 && systemp < 49.9) {
		rpi_softPWMWrite(3, 75);
	} else {
		rpi_softPWMWrite(3, 100);
	}
}

int main() {
	if(rpi_mapPeripheral(&gpio) == -1) {
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		return -1;
	}
	
	TB6612_initBoard();

//	initCPUFan();
	
//	while(1) {
//		printf("%f\n", sensor.dist);
//		manageCPUTemps();	
//		delayMicroseconds(1000000);
//	}

	GPIO_SET = 1 << PWMA;
    GPIO_SET = 1 << PWMB;
    TB6612_motorMask = AIN1_enable | BIN1_enable | STBY_enable;
	
	sleep(2);

	TB6612_deInit();
	rpi_unmapPeripheral(&gpio);
	return 0;
}

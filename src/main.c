//
//	main.c
//	Robert
//
//	Created by Jack Thake on 6/1/20.
//	Copyright © 2020 Jack Thake. All rights reserved.
//


#include "Modules/RPI/RPI.h"
#include "Modules/TB6612/TB6612.h"
#include <stdio.h>
#include <unistd.h>

int main() {
	if(rpi_mapPeripheral(&gpio) == -1) {
		printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
		return -1;
	}

	TB6612_initBoard();
	
	TB6612_motorASpeed = TB6612_motorBSpeed = 50;	
	TB6612_motorMask = AIN1_enable | BIN1_enable | STBY_enable;
	sleep(1);
	TB6612_motorMask = AIN1_enable | STBY_enable;
	sleep(1);
	
	TB6612_deInit();
	rpi_unmapPeripheral(&gpio);
	return 0;
}

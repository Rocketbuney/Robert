//
//  main.c
//  Robert
//
//  Created by Jack Thake on 6/1/20.
//  Copyright © 2020 Jack Thake. All rights reserved.
//


#include "RPI.h"
#include <stdio.h>
#include <unistd.h>

volatile struct bcm2835_peripheral gpio = { GPIO_BASE, 0, 0, 0 };

int main() {
    if(map_peripheral(&gpio) == -1) {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    // Define BCM 4 as output
    INP_GPIO(4);
    OUT_GPIO(4);

    while(1) {
        GPIO_SET = 1 << 4;
        sleep(1);

        GPIO_CLR = 1 << 4;
        sleep(1);
    }

    return 0;
}

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

#define PWMA 17
#define AIN1 22
#define AIN2 27

#define PWMB 0
#define BIN1 9
#define BIN2 11

#define STBY 10

volatile struct bcm2835_peripheral gpio = { GPIO_BASE, 0, 0, 0 };

int main() {
    if(rpi_mapPeripheral(&gpio) == -1) {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    // Define BCM 4 as output
    INP_GPIO(PWMA); // pwm a
    OUT_GPIO(PWMA);
    INP_GPIO(AIN2); // ain2
    OUT_GPIO(AIN2);
    INP_GPIO(AIN1); // ain1
    OUT_GPIO(AIN1);
    INP_GPIO(PWMB); // pwm b
    OUT_GPIO(PWMB);
    INP_GPIO(BIN2); // bin2
    OUT_GPIO(BIN2);
    INP_GPIO(BIN1); // bin1
    OUT_GPIO(BIN1);
    INP_GPIO(STBY); // stby
    OUT_GPIO(STBY);

    GPIO_SET = 1 << PWMA;
    GPIO_CLR = 1 << AIN1;
    GPIO_SET = 1 << AIN2;
    
    GPIO_SET = 1 << STBY;
    
    GPIO_SET = 1 << PWMB;
    GPIO_CLR = 1 << BIN1;
    GPIO_SET = 1 << BIN2;
    
    sleep(1);
    
    GPIO_CLR = 1 << PWMA;
    GPIO_CLR = 1 << AIN1;
    GPIO_CLR = 1 << AIN2;
    GPIO_CLR = 1 << PWMB;
    GPIO_CLR = 1 << BIN1;
    GPIO_CLR = 1 << BIN2;
    GPIO_CLR = 1 << STBY;
    
    rpi_unmapPeripheral(&gpio);
    return 0;
}

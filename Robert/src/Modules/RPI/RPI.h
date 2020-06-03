//
//  RPI.H
//  Robert
//
//  Created by Jack Thake on 6/1/20.
//  Copyright © 2020 Jack Thake. All rights reserved.
//


#ifndef RPI_H
#define RPI_H

#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE         (BCM2708_PERI_BASE + 0x200000)    // GPIO controller
#define BSC0_BASE         (BCM2708_PERI_BASE + 0x205000)    // I2C controller
#define PAGE_SIZE         (4*1024)
#define BLOCK_SIZE        (4*1024)

// IO Acces
struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void *map;
    volatile unsigned int *addr;
};

// One 32 bit address holds info for 10 pins
// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g)       *(gpio.addr + ((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g)       *(gpio.addr + ((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio.addr + (((g)/10))) |= (((a)<=3?(a) + 4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET     *(gpio.addr + 7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR     *(gpio.addr + 10) // clears bits which are 1 ignores bits which are 0

#define GPIO_READ(g)     *(gpio.addr + 13) &= (1<<(g))

extern volatile struct bcm2835_peripheral gpio;

// Function prototypes
int rpi_mapPeripheral(volatile struct bcm2835_peripheral *p);
void rpi_unmapPeripheral(volatile struct bcm2835_peripheral *p);

#endif

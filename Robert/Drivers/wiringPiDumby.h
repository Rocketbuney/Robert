//
//  wiringPiDumby.cpp
//  Robert
//
//  Created by Jack Thake on 6/1/20.
//  Copyright © 2020 Jack Thake. All rights reserved.
//

#ifndef wiringPiDumby_h
#define wiringPiDumby_h
#ifdef __APPLE_CC__

#include <iostream>
int INPUT = 1, OUTPUT = 2, PWM_OUTPUT = 3;

int wiringPiSetup() { return 0; }
void digitalWrite(int pin, int value) { std::cout << "Pin " << pin << " set to " << value << std::endl; }
void pwmWrite(int pin, int value) { std::cout << "PWM Pin " << pin << " set to " << value << std::endl; }
void pinMode(int pin, int mode) {
    if(mode == INPUT)
        std::cout << "Pin " << pin << "'s mode set to INPUT\n";
    else if(mode == OUTPUT)
        std::cout << "Pin " << pin << "'s mode set to OUTPUT\n";
    else if(mode == PWM_OUTPUT)
        std::cout << "Pin " << pin << "'s mode set to PWM_OUTPUT\n";
};
#endif
#endif /* wiringPiDumby_h */

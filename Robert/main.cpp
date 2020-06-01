//
//  main.cpp
//  Robert
//
//  Created by Jack Thake on 6/1/20.
//  Copyright © 2020 Jack Thake. All rights reserved.
//

#include <iostream>
#include <unistd.h>

#include "Drivers/TB6612.hpp"

#define unused(x) (void)(x)

int main(int argc, const char * argv[]) {
    unused(argc);
    unused(argv);
    
    struct TB6612Motor *mot1 = createMotor(9, 8, 1);
    struct TB6612Motor *mot2 = createMotor(15, 12, 24);
    TB6612 board = TB6612(*mot1, *mot2, 7, 500);
    
    std::cout << "motor on\n";
    board.updateBoard(MotorMask::kMotorAForward, true);
    sleep(10);
    std::cout << "motor off\n";
    board.updateBoard(MotorMask::kMotorNone, true);
    
    return 0;
}

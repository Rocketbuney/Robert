//
//  TB6612.hpp
//  Robert
//
//  Created by Jack Thake on 6/1/20.
//  Copyright © 2020 Jack Thake. All rights reserved.
//

#ifndef TB6612_hpp
#define TB6612_hpp
#include <stdint.h>

typedef enum : uint8_t {
    kMotorAForward  = 0b00000001,
    kMotorABackward = 0b00000010,
    kMotorBForward  = 0b00000100,
    kMotorBBackward = 0b00001000,
    kMotorNone      = 0b00010000
} MotorMask;

/// Structure representing one motor connected to the TB6612 board
struct TB6612Motor {
    uint8_t IN1, IN2, PWM;
};

/// Class representing one TB6612 motor driver board
class TB6612 {
    struct TB6612Motor m_motorA;
    struct TB6612Motor m_motorB;
    uint8_t m_standbyPin;
    uint16_t m_motorSpeed;
public:
    TB6612(struct TB6612Motor, struct TB6612Motor, uint8_t, uint16_t);
    ~TB6612();
    void updateBoard(MotorMask, bool);
};

struct TB6612Motor *createMotor(uint8_t, uint8_t, uint8_t);

#endif /* TB6612_hpp */

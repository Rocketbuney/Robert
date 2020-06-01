//
//  TB6612.cpp
//  Robert
//
//  Created by Jack Thake on 6/1/20.
//  Copyright © 2020 Jack Thake. All rights reserved.
//

#include "TB6612.hpp"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __APPLE_CC__
#include "wiringPiDumby.h"
#else
#include <wiringPi.h>
#endif

struct TB6612Motor *createMotor(uint8_t IN1, uint8_t IN2, uint8_t PWM) {
    TB6612Motor *mot = (TB6612Motor*)malloc(sizeof(TB6612Motor));
    memset(mot, 0, sizeof(TB6612Motor));
    assert(mot);
    
    mot->IN1 = IN1;
    mot->IN2 = IN2;
    mot->PWM = PWM;
    
    return mot;
}

TB6612::TB6612(struct TB6612Motor motorA, struct TB6612Motor motorB, uint8_t standbyPin, uint16_t motorSpeed) {
    this->m_motorA = motorA;
    this->m_motorB = motorB;
    this->m_standbyPin = standbyPin;
    this->m_motorSpeed = motorSpeed;
    
    pinMode(motorA.IN1, OUTPUT);
    pinMode(motorA.IN2, OUTPUT);
    pinMode(motorB.IN1, OUTPUT);
    pinMode(motorB.IN2, OUTPUT);
    
    pinMode(motorA.PWM, PWM_OUTPUT);
    pinMode(motorB.PWM, PWM_OUTPUT);
}

TB6612::~TB6612() {
    updateBoard(MotorMask::kMotorNone, false);
    
    pinMode(this->m_motorA.IN1, INPUT);
    pinMode(this->m_motorA.IN2, INPUT);
    pinMode(this->m_motorB.IN1, INPUT);
    pinMode(this->m_motorB.IN2, INPUT);
    pinMode(this->m_motorA.PWM, INPUT);
    pinMode(this->m_motorB.PWM, INPUT);
}

void TB6612::updateBoard(MotorMask motorMask, bool STBY) {
    if ((motorMask & MotorMask::kMotorNone) > 0) {
        digitalWrite(this->m_motorA.IN1, 0);
        digitalWrite(this->m_motorA.IN2, 0);
        digitalWrite(this->m_motorB.IN1, 0);
        digitalWrite(this->m_motorB.IN2, 0);
    } else {
        if ((motorMask & MotorMask::kMotorAForward) > 0) {
            digitalWrite(this->m_motorA.IN1, 0);
            digitalWrite(this->m_motorA.IN2, 1);
        } else if((motorMask & MotorMask::kMotorABackward) > 0) {
            digitalWrite(this->m_motorA.IN1, 1);
            digitalWrite(this->m_motorA.IN2, 0);
        }

        if ((motorMask & MotorMask::kMotorBForward) > 0) {
          digitalWrite(this->m_motorB.IN1, 0);
          digitalWrite(this->m_motorB.IN2, 1);
        } else if((motorMask & MotorMask::kMotorBBackward) > 0) {
          digitalWrite(this->m_motorB.IN1, 1);
          digitalWrite(this->m_motorB.IN2, 0);
        }
    }
    
    if (STBY) {
        digitalWrite(this->m_standbyPin, 1);
    } else {
        digitalWrite(this->m_standbyPin, 0);
    }
    
    pwmWrite(this->m_motorA.PWM, this->m_motorSpeed);
    pwmWrite(this->m_motorB.PWM, this->m_motorSpeed);
}

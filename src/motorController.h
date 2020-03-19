#if !defined(MOTOR_CONTROLLER_H)
#define MOTOR_CONTROLLER_H

// Flags for motor opperation
enum mot_driveMasks {
    RIGHT_FORWARD   = 0b00000001,
    RIGHT_BACKWARD  = 0b00000010,
    LEFT_FORWARD    = 0b00000100,
    LEFT_BACKWARD   = 0b00001000,
    STBY_HIGH       = 0b00010000,
    STBY_LOW        = 0b00100000,
};

char motorFlags;
unsigned motorSpeed;

void mot_initPins();
void mot_updatePins();
void mot_cleanup();

#endif // MOTOR_CONTROLLER_H

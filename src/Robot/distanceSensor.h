#if !defined(DISTANCE_SENOR_H)
#define DISTANCE_SENOR_H

typedef enum dist_sensorID {
    front,
    left,
    right
} dist_sensorID;

void dist_initSensor(dist_sensorID sensor);
float dist_getDistance(dist_sensorID sensor);
void dist_cleanup(dist_sensorID sensor);

#endif // DISTANCE_SENOR_H

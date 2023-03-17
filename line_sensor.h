#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include "mbed.h"

class LineSensor{
    private:
        DigitalIn* centre_sensor;
        DigitalIn* left_sensor;
        DigitalIn* right_sensor;

    public:
        enum Direction{Forward, Left, Right, Fast, Stop, Lost};

        LineSensor(PinName left, PinName centre, PinName right);
        ~LineSensor();

        int check_line_sensors();
        Direction get_direction();
};

#endif

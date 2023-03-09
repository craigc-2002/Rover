#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include "mbed.h"

class LineSensor{
    private:
        DigitalIn* centre_sensor;
        DigitalIn* left_sensor;
        DigitalIn* right_sensor;

        bool line_centre;
        bool line_left;
        bool line_right;

    public:
        enum Direction{Forward, Left, Right, Fast, Stop};

        LineSensor(PinName left, PinName centre, PinName right);
        ~LineSensor();

        int check_line_sensors();
        Direction get_direction();
};

#endif

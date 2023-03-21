#include "mbed.h"
#include "line_sensor.h"

LineSensor::LineSensor(PinName left, PinName centre, PinName right){
    // Constructor method
    centre_sensor = new DigitalIn(centre);
    left_sensor = new DigitalIn(left);
    right_sensor = new DigitalIn(right);

    // Configure each input with an internal pullup resistor
    //  inputs will show no line if sensor is disconnected
    centre_sensor->mode(PullUp);
    left_sensor->mode(PullUp);
    right_sensor->mode(PullUp);
}

LineSensor::~LineSensor(){
    // Destructor method
    delete(centre_sensor);
    delete(left_sensor);
    delete(right_sensor);
}

int LineSensor::check_line_sensors(){
    /*
    Method to check the line sensors
    The three sensors are checked and the line detection results are placed in an integer in the form
        0b [Left] [Centre] [Right]
    eg. detecting the line on the left returns 0b100, and the centre returns 0b010
    */

    // check each of the sensors for the line
    // Sensor returns 1 for no line, 0 for line detected
    bool line_left {bool(left_sensor->read())};
    bool line_centre {bool(centre_sensor->read())};
    bool line_right {bool(right_sensor->read())};

    // Concatenate the sensor data into a single integer
    int sensor_data = (line_left<<2) + (line_centre<<1) + line_right;

    // Bitwise inversion of bottom 3 bits so that a 1 represents line detected and 0 no line
    sensor_data = sensor_data ^ 0b111;
    return sensor_data;
}

LineSensor::Direction LineSensor::get_direction(){
    /*
    Method to return a direction for the rover to move in
    Checks the sensors which returns an integer encoding the position of the line

    If the line is detected on the left or right sensor, the rover will move in that direction
    If the line is detected by the centre sensor only, the rover will speed up
    If the line is not detected the rover will stop 
    */
    
    LineSensor::Direction direction{LineSensor::Lost};

    int sensor_data{check_line_sensors()};
    printf("%d", sensor_data);

    if ((sensor_data & 0b100)==0b100){
        // If line detected by left sensor
        direction = LineSensor::Left;
    }else if ((sensor_data & 0b001)==0b001){
        // If line detected by right sensor
        direction = LineSensor::Right;
    }

    if ((sensor_data & 0b010)==0b010){
        // If line detected by middle sensor
	    direction = LineSensor::Forward;
    }

    return direction;
}
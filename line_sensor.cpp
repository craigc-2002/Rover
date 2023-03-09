#include "mbed.h"
#include "line_sensor.h"

LineSensor::LineSensor(PinName left, PinName centre, PinName right){
    // Constructor method
    centre_sensor = new DigitalIn(centre);
    left_sensor = new DigitalIn(left);
    right_sensor = new DigitalIn(right);
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
    // Sensor returns HIGH for no line, LOW for line detected so the DigitalIn values are inverted
    line_left = ~left_sensor->read();
    line_centre = ~centre_sensor->read();
    line_right = ~right_sensor->read();

    int sensor_data = (line_left<<2) + (line_centre<<1) + line_right;
    return sensor_data;
}

LineSensor::Direction LineSensor::get_direction(){
    /*
    Method to return a direction for the rover to move in
    Checks the sensors which returns an integer encoding the position of the line

    If the line is detected on the left or right sensor, the rover will move in that direction
    If the line is detected by the centre sensor only, the rover will speed up
    If the line is not detected the rover will move forward 
    */
    
    LineSensor::Direction direction{LineSensor::Forward};
    int sensor_data{check_line_sensors()};

    if ((sensor_data & 0b100)==0b100){
        // If line detected by left sensor
        direction = LineSensor::Left;
    }else if ((sensor_data & 0b001)==0b001){
        // If line detected by right sensor
        direction = LineSensor::Right;
    }else if (sensor_data == 0b010){
        // If line only detected by middle sensor
        direction = LineSensor::Fast;
    }

    return direction;
}
// RoverLineFollowing - main.cpp
//  Craig Cochrane
//  Program to demonstrate line following

#include "mbed.h"
#include "rover.h"
#include "line_sensor.h"

Rover rover(PTA12, PTA5, PTC9, PTA4, PTD4, PTC8);
LineSensor line(PTC10, PTC6, PTC11);
bool on_line{true};

const int forward_duty_cycle_slow{20};
const int forward_duty_cycle_fast{60};

enum class Status{normal, line_searching, error=-1};
Status rover_status{Status::normal};

// ---------- Main rover control loop ----------
int main()
{
    while (true) {
        // Find the direction of the line
        LineSensor::Direction line_direction{line.get_direction()};

        // command the rover based on the current status
        switch (rover_status){
            case Status::normal:{
                // if the rover is in normal operation, move along the line
                rover.rgb(1, 0, 1);
                printf("On line\n");

                switch (line_direction){
                    case LineSensor::Forward:{
                        rover.En_duty_set(forward_duty_cycle_slow, forward_duty_cycle_slow);
                        rover.forward();
                        break;
                    }
                    case LineSensor::Fast:{
                        rover.En_duty_set(forward_duty_cycle_fast, forward_duty_cycle_fast);
                        rover.forward();
                        break;
                    }
                    case LineSensor::Left:{
                        rover.anticlockwise();
                        break;
                    }
                    case LineSensor::Right:{
                        rover.clockwise();
                        break;
                    }
                    case LineSensor::Stop:{
                        rover.stop();
                        break;
                    }
                }
                break;
            }
            case Status::line_searching:{
                // if the rover is searching for the line, rotate until it is found
                rover.rgb(0, 0, 1);
                rover.stop();
                //rover.clockwise();
                break;
            }
            case Status::error:{
                // if the system is in an error state, stop the rover and flash the LED red
                rover.stop();
                while (1){
                    rover.rgb(0, 1, 1);
                    wait_us(1000000);
                    rover.rgb(1, 1, 1);
                    wait_us(1000000);
                }
                break;
            }
        }

        wait_us(1000);
    }
    return 0;   
}
// ---------------------------------------------

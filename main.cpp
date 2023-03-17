// RoverLineFollowing - main.cpp
//  Craig Cochrane
//  Program to demonstrate line following

#include "mbed.h"
#include "rover.h"
#include "line_sensor.h"

Rover rover(PTD4, PTA4, PTA12, PTC8, PTC9, PTA5);
LineSensor line(PTC2, PTB3, PTB2);

Timer off_line_timer;

const int forward_duty_cycle_slow{20};
const int forward_duty_cycle_fast{40};

enum class Status{normal, line_searching, error=-1};
Status rover_status{Status::normal};

// ---------- Main rover control loop ----------
int main()
{
    while (true) {
        // Find the direction of the line
        LineSensor::Direction line_direction{line.get_direction()};

        /*
        Use line measurements to determine the status of the rover

        Rover status changed to line_searching if a line hasn't been detected for 2 seconds
        */


        // Update the rover status
        //rover_status = Status::normal;
        
        if (line_direction == LineSensor::Lost){
            off_line_timer.start();
            int time_us = off_line_timer.elapsed_time().count();
            if (time_us >= 2000000){
                rover_status = Status::line_searching;
            }
        }else{
            off_line_timer.stop();
            off_line_timer.reset();
            rover_status = Status::normal;
        }


        // command the rover based on the current status
        switch (rover_status){
            case Status::normal:{
                // if the rover is in normal operation, move along the line
                rover.rgb(1, 0, 1);

                switch (line_direction){
                    case LineSensor::Forward:{
                        //printf("forward\n");
                        rover.En_duty_set(forward_duty_cycle_slow, forward_duty_cycle_slow);
                        rover.forward();
                        break;
                    }
                    case LineSensor::Fast:{
                        //printf("fast\n");
                        rover.En_duty_set(forward_duty_cycle_fast, forward_duty_cycle_fast);
                        rover.forward();
                        break;
                    }
                    case LineSensor::Left:{
                        //printf("left\n");
                        rover.stop();
                        rover.En_duty_set(forward_duty_cycle_fast, forward_duty_cycle_fast);
                        rover.anticlockwise();
                        break;
                    }
                    case LineSensor::Right:{
                        //printf("right\n");
                        rover.stop();
                        rover.En_duty_set(forward_duty_cycle_fast, forward_duty_cycle_fast);
                        rover.clockwise();
                        break;
                    }
                    case LineSensor::Stop:{
                        //printf("stop\n");
                        rover.rgb(0,1,1);
                        rover.stop();
                        break;
                    }
                    case LineSensor::Lost:{
                        rover.rgb(0, 1, 1);
                        break;
                    }
                }
                break;
            }
            case Status::line_searching:{
                // if the rover is searching for the line, move straight backwards until it's found
                rover.rgb(0, 0, 1);
                rover.stop();
                rover.reverse();
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

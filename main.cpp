// Rover - main.cpp

/*
Full rover code.
Follows a line on the ground and avoids obstacles
*/

#include "mbed.h"
#include "rover.h"
#include "line_sensor.h"
#include "hc-sr04.h"

Rover rover(PTD4, PTA4, PTA12, PTC8, PTC9, PTA5);
LineSensor line(PTC2, PTB3, PTB2);
HCSR04 forward_obstacle_sensor(PTD5, PTA13);
HCSR04 side_obstacle_sensor(PTD3, PTA2);
DigitalOut forward_obstacle_LED(PTD0);
DigitalOut side_obstacle_LED(PTD1);

Timer off_line_timer;

const int duty_cycle_slow{20};
const int duty_cycle_fast{40};

enum class Status{normal, obstacle_manouvre, line_searching, error=-1};
Status rover_status{Status::normal};

// ---------- Function to manouvre rover around obstacle ----------
void obstacle_avoidance_routine(){
    // stop the rover and turn 90 degrees anticlockwise
    rover.stop();
    wait_us(500000);
    rover.resume();
    rover.anticlockwise_90();

    bool obstacle_cleared{false};

    while (!obstacle_cleared){
        /* while not past the obstacle 
            obstacle is assumed to be cleared when the line is found again*/
        if(side_obstacle_sensor.get_distance() <= 0.4){
            // if obstacle 40cm or less from rover
            rover.forward();
            side_obstacle_LED.write(1);
        }else{
            rover.clockwise();
            side_obstacle_LED.write(0);
        }

        if (line.get_direction() != LineSensor::Lost){
            obstacle_cleared = true;
        }
    }

    side_obstacle_LED.write(0);
}

// ---------- Main rover control loop ----------
int main()
{
    while (true) {
        // Find the direction of the line
        LineSensor::Direction line_direction{line.get_direction()};
        // measure distance to obstacle using the ultrasonic sensor
        float distance = forward_obstacle_sensor.get_distance();

        /*
        Use line and obstacle distance measurements to determine the status of the rover

        Rover status changed to line_searching if a line hasn't been detected for 2 seconds
        Rover status changed to obstacle_manouvre if an obstacle is detected within 40cm
        */
        
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

        // if obstacle 40cm or less from rover
        if (distance <= 0.4){
            rover_status = Status::obstacle_manouvre;
            forward_obstacle_LED = 1;
        }else{
            forward_obstacle_LED = 0;
        }


        // command the rover based on the current status
        switch (rover_status){
            case Status::normal:{
                // if the rover is in normal operation, move along the line
                rover.rgb(1, 0, 1);

                if (line_direction == LineSensor::Fast){
                    rover.En_duty_set(duty_cycle_fast, duty_cycle_fast);
                }else{
                    rover.En_duty_set(duty_cycle_slow, duty_cycle_slow);
                }

                rover.follow_direction(line_direction);
                break;
            }
            case Status::obstacle_manouvre:{
                // if the rover has detected an obstacle, manouvre around it
                rover.rgb(0, 0, 1);
                obstacle_avoidance_routine();
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
                    forward_obstacle_LED = 1;
                    wait_us(1000000);
                    rover.rgb(1, 1, 1);
                    forward_obstacle_LED = 0;
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

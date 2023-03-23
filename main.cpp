// Rover - main.cpp

/*
Full rover code.
Follows a line on the ground and avoids obstacles
*/

#include "mbed.h"
#include "rover.h"
#include "line_sensor.h"
#include "hc-sr04.h"
#include "clap_switch.h"

Rover rover(PTC9, PTC8, PTA12, PTA4, PTD4, PTA5);

LineSensor line(PTB2, PTB1, PTB0);

ClapSwitch clap(PTA1);

HCSR04 forward_obstacle_sensor(PTD5, PTA13);
HCSR04 side_obstacle_sensor(PTD3, PTD2);

DigitalOut forward_obstacle_LED(PTD0);
DigitalOut side_obstacle_LED(PTD1);

Timer off_line_timer;
bool lost_object{false};

// enable pin duty cycle values
const int duty_cycle_slow{20};
const int duty_cycle_cruise{25};
const int duty_cycle_med{50};
const int duty_cycle_fast{60};

enum class Status{normal, obstacle_manouvre, line_searching, error=-1};
Status rover_status{Status::normal};

// ---------- Function to manouvre rover around obstacle ----------
void obstacle_avoidance_routine(){
    // stop the rover and turn 90 degrees anticlockwise
    rover.stop();
    wait_us(500000);
    rover.rgb(1, 1, 0);

    while (side_obstacle_sensor.get_distance() > 0.4){
        rover.anticlockwise();
    }
    rover.stop();
    rover.rgb(1, 1, 0);

    bool obstacle_cleared{false};

    while (!obstacle_cleared){
        /* 
        while not past the obstacle 
        obstacle is assumed to be cleared when the line is found again
        */
        if(side_obstacle_sensor.get_distance() <= 0.4){
            // if obstacle 40cm or less from rover
            side_obstacle_LED.write(1);
            rover.En_duty_set(40, 15);
            rover.clockwise();
            lost_object = false;
        }else{
            side_obstacle_LED.write(0);
            rover.En_duty_set(duty_cycle_med, duty_cycle_med);

            if (lost_object){
                rover.clockwise();
            }else{
                rover.forward();
                wait_us(100000);
                rover.stop();
                wait_us(200000);
                rover.resume();
            }

            lost_object = true;        
        }

        if (line.get_direction() != LineSensor::Lost){
            obstacle_cleared = true;
        }
    }
    side_obstacle_LED.write(0);
    rover.anticlockwise();
    wait_us(200000);
}

// ---------- Main rover control loop ----------
int main(){
    int i{1};

    // wait until a clap is received to start the main line following routine
    while(!clap.clap_received()){
        rover.rgb(0, 1, 0);
        forward_obstacle_LED.write(0);
        wait_us(500000);
        rover.rgb(1, 1, 1);
        forward_obstacle_LED.write(1);
        wait_us(500000);
    }
    
    while (true) {
        // Find the direction of the line
        LineSensor::Direction line_direction{line.get_direction()};
        LineSensor::Direction rover_direction{rover.get_current_direction()};
        bool direction_changed{false};

        // measure distance to obstacle using the ultrasonic sensor
        float distance = forward_obstacle_sensor.get_distance();

        /*
        Use line and obstacle distance measurements to determine the status of the rover

        Rover status changed to line_searching if a line hasn't been detected for 5 seconds
        Rover status changed to obstacle_manouvre if an obstacle is detected within 30cm of the rover
        */
        
        if (line_direction == LineSensor::Lost){
            off_line_timer.start();
            int time_us = off_line_timer.elapsed_time().count();
            if (time_us >= 5000000){
                rover_status = Status::line_searching;
            }else{
                rover_status = Status::normal;
            }
        }else{
            off_line_timer.stop();
            off_line_timer.reset();
            
            if (line_direction != rover_direction){
                direction_changed = true;
            }
            rover_direction = line_direction;
            
            rover_status = Status::normal;
        }

        // if obstacle 30cm or less from rover
        if (distance <= 0.3){
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

                /* 
                Set the duty cycle to the motors
                
                Default to fast, and are decreased based on line sensor readings
                Rover should start moving forward fast, but slow down once it's started to avoid overshooting line
                Rover moves even slower if the line is lost
                When turning, the duty cycle of the inside wheel is reduced to make the power asymmetrical so the rover moves forward
                */
                int duty_left{duty_cycle_fast};
                int duty_right{duty_cycle_fast};

                if (rover_direction == LineSensor::Forward and !direction_changed){
                    // if rover is moving forward and hasn't just changed
                    duty_left = duty_right = duty_cycle_cruise;
                }

                if (line_direction == LineSensor::Lost and rover_direction == LineSensor::Forward){
                    // if the line is not detected
                    duty_left = duty_right = duty_cycle_slow;
                }

                if (rover_direction == LineSensor::Left){
                    duty_left = duty_cycle_med;
                }else if (rover_direction == LineSensor::Right){
                    duty_right = duty_cycle_med;
                }

    
                // command the rover to follow the line at the correct speed
                rover.En_duty_set(duty_left, duty_right);
                rover.follow_direction(rover_direction);
                break;
            }
            case Status::obstacle_manouvre:{
                // if the rover has detected an obstacle, manouvre around it
                rover.rgb(1, 1, 0);
                obstacle_avoidance_routine();
                break;
            }
            case Status::line_searching:{
                // if the rover is searching for the line, move straight backwards until it's found
                rover.rgb(0, 0, 1);
                rover.stop();
                rover.En_duty_set(duty_cycle_slow, duty_cycle_slow);
                rover.reverse();
                break;
            }
            case Status::error:{
                // if the system is in an error state, stop the rover and flash the LEDs red
                rover.stop();
                for (int i{0}; i<3; i++){
                    rover.rgb(0, 1, 1);
                    forward_obstacle_LED.write(1);
                    side_obstacle_LED.write(1);
                    wait_us(1000000);
                    rover.rgb(1, 1, 1);
                    forward_obstacle_LED.write(0);
                    side_obstacle_LED.write(0);
                    wait_us(1000000);
                }
                NVIC_SystemReset();
                break;
            }
        }
    }
    return 0;   
}
// ---------------------------------------------

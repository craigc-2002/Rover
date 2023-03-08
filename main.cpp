#include "mbed.h"
// Program to demonstrate basic obstacle detection and avoidance

#include "rover.h"
#include "hc-sr04.h"

Rover rover(PTC9, PTA5, PTA12, PTA4, PTD4, PTC8);
HCSR04 distance_sensor(PTD5, PTA13);

DigitalOut obs_LED(PTD0);

const int forward_duty_cycle{20};

enum class Status{normal, obstacle_manouvre, error=-1};
Status rover_status{Status::normal};


// ---------- Functions to control the rover ----------
void anticlockwise_90(void){
    // function to turn the rover 90 degrees anticlockwise
    rover.En_duty_set(100, 100);
    rover.anticlockwise();
    wait_us(450000);
    rover.stop();
    wait_us(500000);
    rover.En_duty_set(forward_duty_cycle, forward_duty_cycle);
}

void clockwise_90(void){
    // function to turn the rover 90 degrees clockwise
    rover.En_duty_set(100, 100);
    rover.clockwise();
    wait_us(450000);
    rover.stop();
    wait_us(500000);
    rover.En_duty_set(forward_duty_cycle, forward_duty_cycle);
}

void forward_1s(){
    // function to move the rover forward for 1 second
    rover.forward();
    wait_us(1000000);
    rover.stop();
    wait_us(500000);
}

void obstacle_avoidance_routine(){
    // function to manouvre rover around an obstacle
    rover.stop();
    wait_us(500000);
    rover.resume();
    anticlockwise_90();
    forward_1s();
    clockwise_90();
    forward_1s();
    clockwise_90();
    forward_1s();
    anticlockwise_90();
}
// ---------------------------------------------



// ---------- Main rover control loop ----------
int main()
{
    rover.rgb(0, 0, 0);
    wait_us(1000000);

    while (true) {
        // measure distance to obstacle using the ultrasonic sensor
        float distance = distance_sensor.get_distance();
        printf("Obstacle distance: %f m\n", distance);

        // if obstacle 35cm or less from rover
        if (distance <= 0.35){
            rover_status = Status::obstacle_manouvre;
            obs_LED = 1;
        }else{
            rover_status = Status::normal;
            obs_LED = 0;
        }

        // command the rover based on the current status
        switch (rover_status){
            case Status::normal:{
                // if the rover is in normal operation, move in a straight line
                rover.rgb(1, 0, 1);
                rover.forward();
                break;
            }
            case Status::obstacle_manouvre:{
                // if the rover has detected an obstacle, manouvre around it
                rover.rgb(0, 0, 1);
                obstacle_avoidance_routine();
                break;
            }
            case Status::error:{
                // if the system is in an error state, stop the rover and flash the LED red
                rover.stop();
                while (1){
                    rover.rgb(0, 1, 1);
                    obs_LED = 1;
                    wait_us(1000000);
                    rover.rgb(1, 1, 1);
                    obs_LED = 0;
                    wait_us(1000000);
                }
                break;
            }
            default:{
                rover.rgb(0, 0, 0);
                break;
            }
        }

        wait_us(1000);
    }      
}
// ---------------------------------------------

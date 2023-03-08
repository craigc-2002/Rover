#ifndef ROVER_H
#define ROVER_H

#include "mbed.h"

class Rover{
    public:
        Rover(PinName L1, PinName L2, PinName EnL, PinName R1, PinName R2, PinName EnR);  // Constructor method
        ~Rover(void); // Destructor method

        // Methods to control rover motors
        void stop(void); // Method to stop the rover
        void free(void); // Method to let rover roll
        void resume(void); // Method to write the stored duty cycles to each PWM enable output
        void forward(void); // Method to move both sets of wheels forwards
        void reverse(void); // Method to reverse both sets of wheels
        void clockwise(void); // Method to run left wheels forward and right in reverse
        void anticlockwise(void); // Method to run left wheels in reverse and right forward

        void En_duty_set(float leftDuty_new, float rightDuty_new); // Method to set the duty cycle of both motor En pins
        void rgb(int red, int green, int blue); // Method to set the colour of the on-board rgb LED. Sending a 0 turns the LED on
    
    private:
        // Pointers to DigitalOut pins for motors
        // Left motors - direction and en
        DigitalOut* Left1;
        DigitalOut* Left2;
        PwmOut* EnLeft;

        // Right motors - direction and en
        DigitalOut* Right1;
        DigitalOut* Right2;
        PwmOut* EnRight;

        // Pointers to Digital Out pins for Freedom Board built in LEDs
        DigitalOut* blueLED;
        DigitalOut* redLED;
        DigitalOut* greenLED;

        float leftDuty;
        float rightDuty;

        void write_En_PWM(float leftDuty_new, float rightDuty_new); // Method to write values to PWM enable outputs without changing stored values
};

#endif

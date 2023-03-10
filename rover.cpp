#include "mbed.h"
#include "rover.h"

Rover::Rover(PinName L1, PinName L2, PinName EnL, PinName R1, PinName R2, PinName EnR){
    // Constructor method

    // Control of Left Motors
    Left1 = new DigitalOut(L1); // Forward Left Drive Signal
    Left2 = new DigitalOut(L2); // Reverse Left Drive Signal
    EnLeft = new PwmOut(EnL); // Left enable signal

    // Control of Right Motors
    Right1 = new DigitalOut(R1); // Forward Left Drive Signal
    Right2 = new DigitalOut(R2); // Reverse Left Drive Signal
    EnRight = new PwmOut(EnR); // Right enable signal

    // LED indicators
    redLED = new DigitalOut(PTB18);
    greenLED = new DigitalOut(PTB19);
    blueLED = new DigitalOut(PTD1);

    // Initialise PWM outputs for enable pins to 20%
    En_duty_set(20, 20);
}

Rover::~Rover(void){
    // Destructor method
    delete(Left1);
    delete(Left2);
    delete(EnLeft);
    delete(Right1);
    delete(Right2);
    delete(EnRight);

    delete(blueLED);
    delete(redLED);
    delete(greenLED);
}

void Rover::rgb(int red, int green, int blue){
    // Method to change the value of the rgb LED indicator light
    // 0 turns the LED on
    redLED -> write(red);
    greenLED -> write(green);
    blueLED -> write(blue);
}

void Rover::write_En_PWM(float leftDuty_new, float rightDuty_new){
    // Method to write values to PWM enable outputs without changing stored values
    EnLeft -> write(leftDuty_new);
    EnRight -> write(rightDuty_new);
}

void Rover::resume(void){
    // Method to write the stored duty cycles to each PWM enable output
    write_En_PWM(leftDuty, rightDuty);
}

void Rover::En_duty_set(float leftDuty_new, float rightDuty_new){
    // Method to set the PWM pulse width of the left and right motors
    if (leftDuty != leftDuty_new){
        leftDuty = leftDuty_new / 100;
    }

    if (rightDuty != rightDuty_new){
	rightDuty = rightDuty_new / 100;
    }
    
    resume();
}

void Rover::stop(void){
    // Method to stop driving the motor's wheels
    write_En_PWM(1, 1);
    Left1 -> write(0);
    Left2 -> write(0);
    Right1 -> write(0);
    Right2 -> write(0);
}

void Rover::free(void){
    // Method to let rover roll
    write_En_PWM(0, 0);
    Left1 -> write(0);
    Left2 -> write(0);
    Right1 -> write(0);
    Right2 -> write(0);
}

void Rover::forward(void){
    // Method to drive the rover's wheels forwards
    Left1 -> write(1);
    Left2 -> write(0);
    Right1 -> write(1);
    Right2 -> write(0);
}

void Rover::reverse(void){
    // Method to drive the motor's wheels in reverse
    Left1 -> write(0);
    Left2 -> write(1);
    Right1 -> write(0);
    Right2 -> write(1);
}

void Rover::clockwise(void){
    // Method to rotate the rover clockwise by driving left wheels forward and right wheels in reverse
    Left1 -> write(1);
    Left2 -> write(0);
    Right1 -> write(0);
    Right2 -> write(1);
}

void Rover::anticlockwise(void){
    // Method to rotate the rover clockwise by driving right wheels forward and left wheels in reverse
    Left1 -> write(0);
    Left2 -> write(1);
    Right1 -> write(1);
    Right2 -> write(0);
}

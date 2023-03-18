# Rover
Rover that follows a line on the ground and avoids obstacles. Written for the FRDM-KL25Z running mbed-os.

## Rover Control
The wheel motors of the rover are controlled using an L298 motor controller.
The two sides, left and right, are controlled seperately to allow the rover to turn. Each side has two DigitalOuts to select the direction of the motors and a PWMOut Enable pin.
The duty cycle of this PWM output is used to control the speed of the motors.
Control of the rover motors is implemented in `rover.cpp` and `rover.h`.

## Line Following
An infrared sensor is used to detect the line. This outputs a low signal when the line is detected.
3 sensors are used, one on the left, centre and right. The readings from these sensors are used to dertemine whether the rover needs to turn left or right.
When the line is detected in the centre, then the rover can speed up.

Note: The sensor has to be mounted very close to the line for it to be detected.

## Obstacle Avoidance
Obstacles in the path of the rover are detected using an HC-SR04 ultrasonic sensor.
This is interfaced with using the [HC-SR04_mbed_lib](https://github.com/craigc-2002/HC-SR04_mbed_lib).

# Using the code
`main.cpp` contains driver code that follows a line, and if it is not detected for 2 seconds, reverses to try and locate the line again. Object detection is not implemented in this code.

## Building in Keil Studio
- To build or edit the code, clone this repository into a new project in Keil Studio (File > Clone > enter `https://github.com/craigc-2002/Rover`).
- Add the mbed-os library to the project (Right Click > add Mbed Library > enter `https://github.com/ARMmbed/mbed-os` > select version 6.15.1).
- Add the HC-SR04_mbed_lib to the project (Right Click > add Mbed Library > enter `https://github.com/craigc-2002/HC-SR04_mbed_lib` > select v0.0.1).
- Select `FRDM-KL25Z` as tartget hardware.

The code is now ready to be built and edited.

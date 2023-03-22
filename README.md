# Rover
Rover that follows a line on the ground and avoids obstacles. Written for the FRDM-KL25Z running mbed-os.

## Rover Control
The wheel motors of the rover are controlled using an L298 motor controller. This interfaces with the FRDM-KL25Z using 6 GPIO pins.

The two sides, left and right, are controlled seperately to allow the rover to turn. Each side has two DigitalOuts to select the direction of the motors and a PWMOut Enable pin. The duty cycle of this PWM output is used to control the speed of the motors.

Control of the rover motors is implemented in `rover.cpp` and `rover.h`.

## Line Following
An infrared sensor is used to detect the line. This outputs a low signal when the line is detected.
3 sensors are used, one on the left, centre and right. The readings from these sensors are used to dertemine whether the rover needs to turn left or right.
When the line is detected in the centre, then the rover can move forward.

If the line is lost, i.e. not detected by any of the 3 sensors, for 5 seconds, then the rover stops and reverses to attempt to find the line again.

The duty cycle of the enable pin PWM signal is varied depending on the position of the line to enable the rover to complete turns and accelerate quickly while not overshooting the line when moving forwards.

Note: The sensors needto be mounted very close to the line for it to be detected, and have to be as close to the front of the rover as possible.

## Obstacle Avoidance
Obstacles in the path of the rover are detected using an HC-SR04 ultrasonic sensor.
This is interfaced with using the [HC-SR04_mbed_lib](https://github.com/craigc-2002/HC-SR04_mbed_lib).

## Clap to start
An external sound detection circuit is used to detect a sudden loud sound such as a clap and set an output line high.
This is used to start the rover with a clap using the simple ClapSwitch class which uses an interrupt to detect a clap.
The main program sits in a while loop to wait for a clap before beginning the main line following routine.

# Using the code
`main.cpp` contains driver code that follows a line, and if it is not detected for 2 seconds, reverses to try and locate the line again.
If an obstacle is deected less than 30cm from the rover, it stops and attempts to manouvre around it by using a second ultrasonic sensor on the side to attempt to keep the obstacle at a constant distance away and move round it in a circle.

This routine doesn't start until a clap has been detected by the clap detector circuit.

## Building in Keil Studio
- To build or edit the code, clone this repository into a new project in Keil Studio (File > Clone > enter `https://github.com/craigc-2002/Rover`).
- Add the mbed-os library to the project (Right Click > add Mbed Library > enter `https://github.com/ARMmbed/mbed-os` > select version 6.15.1).
- Add the HC-SR04_mbed_lib to the project (Right Click > add Mbed Library > enter `https://github.com/craigc-2002/HC-SR04_mbed_lib` > select v0.0.1).
- Select `FRDM-KL25Z` as tartget hardware.

The code is now ready to be built and edited.

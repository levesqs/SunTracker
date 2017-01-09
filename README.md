# SunTracker
by Steve Levesque
8 jan 2017

project for a sun tracker based on two motors. one is the Newport URS75CC controled with the Newport SMC100CC.
The second is a generic step motor with 5x gearbox driven by the Adafruit motor shield over the arduino UNO.
the feedback is from the pixy Cmucam5 camera. 
it can track Sun,Moon and Venus. 
The Uno gets position of the object and calculate response to send to the motors.
The newport works over a serial port with a max232cpe chip and need a lot of talking. it is not blocking calls.
The stepper is currently blocking commands that emulates the Newport ones.
boths motors are commandable through commands from the USB of UNO who pasthrough Newport ones to serial port.
the UNO initialise both motors (stepper with an optoswitch, Newport with self commands) 
then wait for pixy positions, buttons or usb commands.
4 button on the proto board handle 4 direction of initial positionning. 

done and working
1-initialisation and tracking of objects in close loop
2-passthrou commands
3-buttons
4-pixy objects reading through SPI
5-calculation of basic response

to do
1-handle bog of calculation near the pole area.
2-add GPS to get time and position
3-calculate target initial positions.
4-safety commands parser for both.
5-unblocking commands on the stepper.

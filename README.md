# SunTracker
by Steve Levesque
8 jan 2017

project for a sun tracker based on two motor. one is the Newport URS75CC controled with the Newport SMC100CC.
the second is a generic step motor with gearbox driven by the Adafruit motor shield over the UNO.
the feedback is from the pixy Cmucam5 camera. 
it can track Sun Moon and Venus. 
The Uno get position of the object and calculate response to send to the motor.
The newport works over a serial port with a max232cpe chip and nneed a lot of talking.
The stepper is currently blocking commands that emulates the newport ones.
boths motors are commandable through passthru commands from the USB of UNO.
the UNO initialise both motor (steper with a optoswitch , newport on commands) then wait for pixy positions or usb commands.
4 button on the proto board handle 4 direction of initial positionning. 

done
1-initialisation and tracking of objects in close loop
2-passthrou commands

to do
1-handle bog of calculation near the pole.
2-add GPS to get time and position
3-calculate target initial positions.
4-safety commands parser
5-unblocking commands on the stepper.

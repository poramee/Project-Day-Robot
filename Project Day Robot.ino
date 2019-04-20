#include "Robot.hpp"

/*
 << PIN LIST >>
 ULTRASONIC
	UL1 "Left"	[Trig] -> 11
				[Echo] -> 1
	UL2 "Cen"	[Trig] -> 10
				[Echo] -> 14
	UL3 "Right"	[Trig] -> 9
				[Echo] -> 50

 WHEEL & CONTROLLER
 	ENA -> 13
 	IN1 -> 46
 	IN2 -> 47
 	IN3 -> 48
 	IN4 -> 49
 	ENB -> 12

 PHOTORESISTORS
 	OUT1 -> 41
 	OUT2 -> 42
 	OUT3 -> 43
 	OUT4 -> 44
 	OUT5 -> 45
*/

Robot robot(70, -7);

void setup() {
	robot.pinInit();
	robot.setDirection(Stop);
	for(int i = 41;i <= 45;++i) pinMode(i,INPUT);
	Serial.begin(9600);
}

void loop() {
	// ultrasonicPID();

}
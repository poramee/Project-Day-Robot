#include <NewPing.h>

class Sonar : public NewPing{
	public:
	Sonar(uint8_t a,uint8_t b,unsigned int c): NewPing(a,b,c){}
	bool isObstract(){
		const int check = this -> ping_cm();
		return check <= 20 && check > 0;
	}
	bool outOfBound(){
		return this -> ping_cm() == 0;
	}
};

enum Direction {
	Backward = -1,
	Stop,
	Forward,
};

class Wheel {
private:
	int ctrl;
	int A;
	int B;
	int speed;
	Direction direction;

public:
	Wheel(int ctrl, int A, int B) {
		this -> ctrl = ctrl;
		this -> A = A;
		this -> B = B;
	}
	~Wheel() {}
	void setSpeed(int speed) {
		analogWrite(ctrl, speed);
		this -> speed = speed;
	}
	int getSpeed() { return speed; }
	void setDirection(Direction direction) {
		if (direction == Forward) {
			digitalWrite(A, LOW);
			digitalWrite(B, HIGH);
		}
		else if (direction == Backward) {
			digitalWrite(A, HIGH);
			digitalWrite(B, LOW);
		}
		else if (direction == Stop) {
			digitalWrite(A, LOW);
			digitalWrite(B, LOW);
		}
		else return;
		this -> direction = direction;
	}
	Direction getDirection() { return direction; }
	void pinInit() {
		pinMode(ctrl, OUTPUT);
		pinMode(A, OUTPUT);
		pinMode(B, OUTPUT);
	}
};

class Robot {
private:
	const unsigned int MAX_SONAR_DISTANCE = 100;
	Wheel left, right;
	int motorCompensation;
	Direction direction;

public:
	Sonar sonarLeft, sonarCenter, sonarRight;

public:
	Robot(): Robot(100,0){};
	Robot(int speed, int motorCompensation): left(12, 49, 48), right(13, 46, 47), sonarLeft(11, 1, MAX_SONAR_DISTANCE), sonarCenter(10, 14, MAX_SONAR_DISTANCE), sonarRight(9, 50, MAX_SONAR_DISTANCE) {
		setSpeed(speed, motorCompensation);
		left.setDirection(Forward);
		right.setDirection(Forward);
	};
	~Robot() {};
	void pinInit() {
		left.pinInit();
		right.pinInit();
	}
	void setDirection(Direction direction) {
		if (direction != Forward && direction != Backward && direction != Stop) return;
		else this -> direction = direction;
		left.setDirection(direction);
		right.setDirection(direction);
	}
	void setSpeed(int speed) {
		int leftSpeedCompensation = speed + motorCompensation;
		if(leftSpeedCompensation > 255) leftSpeedCompensation = 255;
		else if(leftSpeedCompensation < 0) leftSpeedCompensation = 0;

		int rightSpeedCompensation = speed - motorCompensation;
		if(rightSpeedCompensation > 255) rightSpeedCompensation = 255;
		else if(rightSpeedCompensation < 0) rightSpeedCompensation = 0;

		left.setSpeed(leftSpeedCompensation);
		right.setSpeed(rightSpeedCompensation);
	}
	void setSpeed(int speed, int motorCompensation) {
		left.setSpeed(speed + motorCompensation > 255 ? 255 : speed + motorCompensation);
		right.setSpeed(speed - motorCompensation < 0 ? 0 : speed - motorCompensation);
		this -> motorCompensation = motorCompensation;
	}
	void setCompensation(int motorCompensation) { this -> motorCompensation = motorCompensation; }
	int getCompensation() { return motorCompensation; }
	void turnLeft() {
		left.setSpeed(200);
		right.setSpeed(200);
		left.setDirection(Backward);
		right.setDirection(Forward);
		while (!sonarCenter.outOfBound()) delay(50);
		setDirection(direction);
	}
	void turnRight() {
		left.setSpeed(200);
		right.setSpeed(200);
		left.setDirection(Forward);
		right.setDirection(Backward);
		while (!sonarCenter.outOfBound()) delay(50);
		setDirection(direction);
	}
	void brake() {
		if (direction == Stop) return;

		left.setDirection(direction == Forward ? Backward : Forward);
		right.setDirection(direction == Forward ? Backward : Forward);
		delay(50);
		left.setDirection(Stop);
		right.setDirection(Stop);

		direction = Stop;
	}
	int getUltrasonicErrorValue() {
		return sonarLeft.ping_cm() - sonarRight.ping_cm();
	}
};
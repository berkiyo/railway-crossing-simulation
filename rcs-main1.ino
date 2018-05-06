#include <Servo.h>
Servo boomgatesServo; // Servo 

int pin[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
int RED = 0; int GREEN =1;  int OFF= 2;
int trainTrigger = pin[3];
int eastTrigger = pin[2];

int northsouthGreen = pin[11];
int northsouthYellow = pin[4];
int northsouthRed = pin[5];
int eastGreen = pin[6];
int eastYellow = pin[7];
int eastRed = pin[8];
int eastLeftRed = pin[9];
int boomgates = pin[10];

int northsouthTimer = 0;
int eastTimer = 0;
int northsouthStatus = RED; //red =0, green = 1
int eastStatus = RED; //red =0, green = 1


int eastTriggerState = 0;
int trainTriggerState = 0;

void setup() {  

  pinMode(eastTrigger, INPUT);
  pinMode(trainTrigger, INPUT);
  pinMode(northsouthGreen, OUTPUT);
  pinMode(northsouthYellow, OUTPUT);
  pinMode(northsouthRed, OUTPUT);
  pinMode(eastGreen, OUTPUT);
  pinMode(eastYellow, OUTPUT);
  pinMode(eastRed, OUTPUT);
  pinMode(eastLeftRed, OUTPUT);
  pinMode(boomgates, OUTPUT);

  setnorthsouthGreen(northsouthGreen, northsouthYellow, northsouthRed);
  seteastRed(eastGreen, eastYellow, eastRed);

  digitalWrite(eastTrigger, HIGH);
  digitalWrite(trainTrigger, HIGH);
  boomgatesServo.write(0); // 0 degrees
  Serial.begin(9600); //for debugging
  boomgatesServo.attach(boomgates);
  
  attachInterrupt (digitalPinToInterrupt (trainTrigger), traininteruptSensor, CHANGE);  // attach interrupt handler
  attachInterrupt (digitalPinToInterrupt (eastTrigger), eastinteruptSensor, CHANGE);  // attach interrupt handler
}

void loop() {

  northsouthTimer++;
  
  if (eastTimer > 0) {
    eastTimer--; //decrements
  }

  Serial.print("East Trigger State: ");
  Serial.println(eastTriggerState);
  
  checkeastTrafficSensors();
  checkTrainSensors();
  ourdelay(1000);
}



int checkeastTrafficSensors() {

	if (eastTriggerState == 1) {
		if (northsouthTimer > 5) { //5 seconds for north lights
		  
			northsouthgreentored(northsouthGreen, northsouthYellow, northsouthRed);
			ourdelay(1000);
			eastredtogreen(eastGreen, eastYellow, eastRed);
			Serial.println("East Traffic Active! (5 seconds)");
	
			ourdelay(5000);
			digitalWrite(eastTrigger, HIGH);
			eastgreentored(eastGreen, eastYellow, eastRed);
			delay(1000);
			if (trainTriggerState != 1) {
				ourdelay(1000);
				northsouthredtogreen(northsouthGreen, northsouthYellow, northsouthRed);
			}
				northsouthTimer=0;
				eastTriggerState=0;			
		}
		
	}
	
	
}
int ourdelay(int timer) {
    int i;
	for (i=0; i<timer; i++) {
		if (trainTriggerState == 1) break;
		delay(1);
	}

}
int checkTrainSensors() {
	int pos = 0;
	if (trainTriggerState == 1) {	
		
		if (northsouthStatus == GREEN) 
			northsouthgreentored(northsouthGreen, northsouthYellow, northsouthRed);
		if (eastStatus == RED) {
			delay (1000);
			eastredtogreen(eastGreen, eastYellow, eastRed);
		}
		seteastleftarrow(RED);

		for(pos = 0; pos <= 90; pos += 1) // goes from 0 degrees to 180 degrees 
		{                                  // in steps of 1 degree 
			boomgatesServo.write(pos);              // tell servo to go to position in variable 'pos' 
		  delay(15);                       // waits 15ms for the servo to reach the position 
		} 
		northsouthTimer = 0;
        delay (5000);
		trainTriggerState = 0;
		digitalWrite(boomgates,HIGH);
		boomgatesServo.write(0); 
		delay (1000);
		seteastleftarrow(OFF);
		eastgreentored(eastGreen, eastYellow, eastRed);
		delay(1000);
		northsouthredtogreen(northsouthGreen, northsouthYellow, northsouthRed);

	}
}

// Interrupt Service Routine (ISR)
void eastinteruptSensor ()
{
  int TriggerState = digitalRead(eastTrigger);
    if (TriggerState == LOW) {
    	Serial.print("INTAeasttriggerstate: ");
    	Serial.println(eastTriggerState);
    	eastTriggerState = 1;
    	delay(1000);
	}
	  
}  // end of eastSensor

void traininteruptSensor ()
{
  int TriggerState = digitalRead(trainTrigger);
    if (TriggerState == LOW) {
    	Serial.print("INT Train Trigger: ");
    	Serial.println(trainTriggerState);
    	trainTriggerState = 1;
    	delay(1000);
	}
	  
}  // end of eastSensor


// Functions
int seteastleftarrow(int LEFTARROW) {
	if (LEFTARROW == RED) 
		digitalWrite(eastLeftRed, HIGH);
	if (LEFTARROW == OFF) 
		digitalWrite(eastLeftRed, LOW);

}

int setnorthsouthGreen(int green, int yellow, int red) {
  digitalWrite(green, HIGH);
  digitalWrite(yellow, LOW);
  digitalWrite(red, LOW);
  northsouthStatus = GREEN;
}

int seteastRed(int green, int yellow, int red) {
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  eastStatus = RED;
}


int northsouthgreentored(int green, int yellow, int red) {
  digitalWrite(green, LOW);
  delay(200);
  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
  delay(200);
  digitalWrite(red, HIGH);
  northsouthStatus = RED;
}

int northsouthredtogreen(int green, int yellow, int red) {
	digitalWrite(red, LOW);
	delay(200);
	digitalWrite(yellow, HIGH);
	delay(1000);
	digitalWrite(yellow, LOW);
	delay(200);
	digitalWrite(green, HIGH);
	northsouthStatus = GREEN;
}

int eastgreentored(int green, int yellow, int red) {
	digitalWrite(green, LOW);
	delay(200);
	digitalWrite(yellow, HIGH);
	delay(1000);
	digitalWrite(yellow, LOW);
	delay(200);
	digitalWrite(red, HIGH);
	eastStatus = RED;
  }

int eastredtogreen(int green, int yellow, int red) {
  digitalWrite(red, LOW);
  delay(200);
  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
  delay(200);
  digitalWrite(green, HIGH);
  eastStatus = GREEN;
}


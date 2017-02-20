#include <SoftwareSerial.h>

#define ONE_SECOND 1000

#define M1_IB 4                    // Motor 1 Input B
#define M1_IA 5                    // Motor 1 Input A
#define M2_IB 6                    // Motor 2 Input B
#define M2_IA 7                    // Motor 2 Input A

                                   //  The circuit:
                                   // * RX is digital pin 10 (connect to TX of other device)
                                   // * TX is digital pin 11 (connect to RX of other device)
#define TxD 2                      // Digital pin used to transmit data to the bluetooth module
#define RxD 3                      // Digital pin used to receive data from the bluetooth module
SoftwareSerial BTSerial(RxD, TxD); // RX, TX for Bluetooth

char data[] = "+000|090";  // Array of size 9 (8 characters + null terminate).
                           // Direction and Speed | Turn.  +/- for direction.
                           // Speed is from -100 to 100, and -5 to 5 will be 0.
                           // Turn 0 to 180, and 85 to 95 will be 90. 

int turn = 90;             // Start out with no turn.
int vector = 0;            // Start out not moving.

void setup() {
  Serial.begin(9600);
  BTSerial.begin(38400);   // Transmission speed used on the bluetooth module  

  // Motors
  pinMode(M1_IB, OUTPUT);
  pinMode(M1_IA, OUTPUT); 
  pinMode(M2_IB, OUTPUT);
  pinMode(M2_IA, OUTPUT);   

  digitalWrite(M1_IB, LOW);  
  digitalWrite(M1_IA, LOW);  
  digitalWrite(M2_IB, LOW);  
  digitalWrite(M2_IA, LOW);  
}

void loop() {
  if(receivedCommand()) {
    setTurn();
    setSpeedAndDirection();
    engage();
  }
}

boolean receivedCommand() {
  if(BTSerial.available() && BTSerial.read() == '^') {  // Check if transmission started
    for(int i = 0; i < 8; i++) { 
      while(!BTSerial.available());                     // Wait for data to show up.
      data[i] = BTSerial.read(); 
    }
    // Serial.println(data);
  } else {
    //Serial.println("Bad Data");
    return false;                                       // Transmission has not started yet.
  }
  return validateCommand();                             // Validate the command that has been received.
}

boolean validateCommand() {
  int i;
  if(data[0] != '+' && data[0] != '-') { return false; }           // [0] must be a + or -
  for(i=1; i<=3; i++) { if(!isDigit(data[i])) { return false; } }  // [1,2,3] must be an integer
  if(data[4] != '|') { return false; }                             // [4] must be a |
  for(i=5; i<=7; i++) { if(!isDigit(data[i])) { return false; } }  // [5,6,7] must be an integer
  return true;  
}

void setTurn() {
  char new_turn[] = { data[5], data[6], data[7], '\0' };      
  turn = atoi(new_turn);
}

void setSpeedAndDirection() {
  char new_speed[] = { data[1], data[2], data[3], '\0' }; 
  vector = atoi(new_speed);
  if(data[0] == '-') { vector *= -1; }  
}

void engage() {
  if(vector > 0) { 
    moveForward();
  } else if(vector < 0) {
    moveBackward();
  } else {
    stopMoving();
  }
}


//##########################################
// Motors
//##########################################
void testMotors() {
  stopMoving();  
  delay(ONE_SECOND);
  moveForward();
  delay(ONE_SECOND);
  moveBackward();
  delay(ONE_SECOND);
//  turnLeft();
//  delay(ONE_SECOND);
//  turnRight();
//  delay(ONE_SECOND);
//  noTurn();
//  stopMoving();  
}

void moveForward() {
  Serial.println("Moving Foward");
  digitalWrite(M1_IB, LOW);  
  digitalWrite(M1_IA, HIGH);  
  digitalWrite(M2_IB, LOW);  
  digitalWrite(M2_IA, HIGH); 
}

void moveBackward() {  
  Serial.println("Moving Backword");
  digitalWrite(M1_IB, HIGH);  
  digitalWrite(M1_IA, LOW);  
  digitalWrite(M2_IB, HIGH);  
  digitalWrite(M2_IA, LOW); 
}

//void noTurn() {
//  digitalWrite(STEERING_A, LOW);
//  digitalWrite(STEERING_B, LOW);
//}
//
//void turnLeft() {
//  digitalWrite(STEERING_A, HIGH);
//  digitalWrite(STEERING_B, LOW);
//}
//
//void turnRight() {
//  digitalWrite(STEERING_A, LOW);
//  digitalWrite(STEERING_B, HIGH);  
//}

void stopMoving() {
  Serial.println("Stopped Moving");
  digitalWrite(M1_IB, LOW);  
  digitalWrite(M1_IA, LOW);  
  digitalWrite(M2_IB, LOW);  
  digitalWrite(M2_IA, LOW); 
}
//##########################################
//##########################################

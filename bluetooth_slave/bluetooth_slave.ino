#include <SoftwareSerial.h>

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
  BTSerial.begin(38400);           // Transmission speed used on the bluetooth module  
}

void loop() {
  if(receivedCommand()) {
    setTurn();
    setSpeedAndDirection();
  }
}

boolean receivedCommand() {
  if(BTSerial.available() && BTSerial.read() == '^') {  // Check if transmission started
    for(int i = 0; i < 8; i++) { 
      while(!BTSerial.available()); // Wait for data to show up
      data[i] = BTSerial.read(); 
    }
  } else {
    return false;                                       // Transmission has not started yet.
  }
  return validateCommand(); 
}

boolean validateCommand() {
  int i;
  if(data[0] != '+' && data[0] != '-') { return false; }
  if(data[4] != '|') { return false; }
  for(i=1; i<=3; i++) { if(!isDigit(data[i])) { return false; } }
  for(i=5; i<=7; i++) { if(!isDigit(data[i])) { return false; } }
  return true;  
}

void setTurn() {
  char new_turn[] = { data[5], data[6], data[7], '\0' };      
  turn = atoi(new_turn);
  Serial.print("Turn: ");
  Serial.print(turn);
}

void setSpeedAndDirection() {
  char new_speed[] = { data[1], data[2], data[3], '\0' }; 
  vector = atoi(new_speed);
  if(data[0] == '-') { vector *= -1; }
  Serial.print(" | Vector: ");
  Serial.println(vector);     
}


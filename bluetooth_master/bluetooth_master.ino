#include <SoftwareSerial.h>
                                  
                                   //  The circuit:
                                   // * RX is digital pin 10 (connect to TX of other device)
                                   // * TX is digital pin 11 (connect to RX of other device)
#define TxD 2                      // Digital pin used to transmit data to the bluetooth module
#define RxD 3                      // Digital pin used to receive data from the bluetooth module
SoftwareSerial BTSerial(RxD, TxD); // RX, TX for Bluetooth

#define JoystickUD 3               // Analog pin used for Up/Down on the Parallax Thumbstick
#define JoystickLR 4               // Analog pin used for Left/Right on the Parallax Thumbstick

char data[] = "^+000|000";          // Array of size 9 (8 characters + null terminate).
                                   // Direction and Speed | Turn.  +/- for direction.
                                   // Speed is from -100 to 100, and -5 to 5 will be 0.
                                   // Turn 0 to 180, and 85 to 95 will be 90. 

void setup() {
  Serial.begin(9600);
  BTSerial.begin(38400);           // Transmission speed used on the bluetooth module
}

void loop() {
  setDirectionAndSpeed();
  setTurn();
  sendData();
}

void setDirectionAndSpeed() {
  char vector[4];  // 3 characters + '\0'
  int vectorValRaw = map(analogRead(JoystickUD), 0, 1023, -100, 100);
  if(vectorValRaw >= -5 && vectorValRaw <= 5) { vectorValRaw = 0; }
  String vectorVal = String( abs(vectorValRaw) );
  vectorVal.toCharArray(vector, 4); 
  replaceRange(2, 4, vector);
  data[1] = vectorValRaw >= 0 ? '+' : '-';
}

void setTurn() {
  char turn[4]; // 3 characters + '\0'
  int turnValRaw = map(analogRead(JoystickLR), 0, 1023, 0, 180);
  if(turnValRaw >= 85 && turnValRaw <= 95) { turnValRaw = 90; }
  String turnVal = String(turnValRaw);
  turnVal.toCharArray(turn, 4);
  replaceRange(6, 8, turn);
}

void replaceRange(int start_pos, int end_pos, char* vals) {
  int next_pos = strlen(vals) - 1;             // -1 so it can be used directly as an index
  for(int i = end_pos; i >= start_pos; i--) {  
    if(next_pos >= 0) {
      data[i] = vals[next_pos];
      next_pos--;   
    } else {
      data[i] = '0';                           // Zero out the data
    }
  }
}

void sendData() {
  BTSerial.write(data);
  Serial.println(data);
}


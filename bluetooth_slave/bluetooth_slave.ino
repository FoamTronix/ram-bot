char data[] = "+000|090";  // Array of size 9 (8 characters + null terminate).
                           // Direction and Speed | Turn.  +/- for direction.
                           // Speed is from -100 to 100, and -5 to 5 will be 0.
                           // Turn 0 to 180, and 85 to 95 will be 90. 

int turn = 90;             // Start out with no turn.
int vector = 0;            // Start out not moving.

void setup() {
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}

void loop() {
  if(Serial.available() > 0) { // Checks whether data is comming from the serial port.
    parseCommands();           // Parse data from Master.
    setTurn();
    setSpeedAndDirection();
  }
}

void parseCommands() {
  String new_data = Serial.readStringUntil('\n'); // Array of size 9 (8 characters + null terminate).
  new_data.toCharArray(data, 9);
}

void setTurn() {
  
}

void setSpeedAndDirection() {
  
}


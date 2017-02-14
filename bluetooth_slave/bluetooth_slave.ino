int state = ' ';
int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  Serial.begin(38400); // Default communication rate of the Bluetooth module
}

void loop() {
  if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port
    Serial.println(state);
    if (state == 97) { // 'a'
      Serial.println("LED ON");
      digitalWrite(ledPin, HIGH); // LED ON
    } else if (state == 98) { // 'b'
      Serial.println("LED OFF");
      digitalWrite(ledPin, LOW); // LED OFF
    }
  }
}

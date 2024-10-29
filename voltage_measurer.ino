const int analog_read_pin = A3;
int FREQUENCY = 50;
int cycles = 1;

void setup() {
  analogReference(EXTERNAL);
  Serial.begin(115200);
  Serial.println("Arduino started");
}

void loop() {
      if (Serial.available() > 0) {
        char command = Serial.read();
        if (command == 'm') {
            float voltage = nplc(cycles);
            Serial.println(voltage);
        }
        else if (command == 'c') {
          String str_cycles = Serial.readStringUntil('\n');
          int new_cycles_count = str_cycles.toInt();
          if (new_cycles_count > 0) {
            cycles = new_cycles_count;
            Serial.println("OK");
          }
          else {
            Serial.println("invalid number of cycles. Should be greater than 0. Standard value is 1");
          }
        }
    }
}

float nplc(int cycles) {
  int samples = 0;
  float sum = 0;
  float duration = (1000.0 / FREQUENCY) * cycles;
  unsigned long start = millis();

  while (millis() - start < duration) {
    sum += (analogRead(analog_read_pin) * 3.3 / 1023);
    samples++;
  }
  float voltage = sum / samples;
  return voltage;
}

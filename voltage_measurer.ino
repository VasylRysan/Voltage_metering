const int analog_read_pin = A3;                             // adc pin on arduino nano
int FREQUENCY = 50;                                         // frequancy of the grid
int cycles = 1;                                             // number of grid cicles 


void setup() {
  analogReference(EXTERNAL);                                // choosed as Vref from AREF pin (3.3V from LM317)
  Serial.begin(115200);                                     // open com port with speed
  Serial.println("Arduino started");
}

void loop() {
      if (Serial.available() > 0) {                          //wait for command from COM port to start measuring
        char command = Serial.read();                        //wait for a char to com port
        if (command == 'm') {                                //react on 1 char 'm'
            float voltage = nplc(cycles);                    //call method "nplc" with argument "cicles"
            Serial.println(voltage);                         //return the result of "nplc" to the serial port
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

float nplc(int cycles) {                                     //function for measuring with nplc method, int cicles - argument of the function
  int samples = 0;                                           //variable for counting number of voltage measurements
  float sum = 0;                                             //variable for sum of voltages
  float duration = (1000.0 / FREQUENCY) * cycles;            //find duration of the measurements
  unsigned long start = millis();

  while (millis() - start < duration) {                      //make measurements during duration
    sum += (analogRead(analog_read_pin) * 3.3 / 1023);       //add samples of voltage to var "sum"
    samples++;                                               //increase number of samples by 1
  }
  float voltage = sum / samples;                             // variable that represents average voltage based on measurements
  return voltage;                                            // function returns measured average voltage
}
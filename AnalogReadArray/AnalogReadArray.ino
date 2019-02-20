int samplingFrequency = 40000;
int delaytime;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  delaytime = 1/40000*1000000;
}

void loop() {
  int channel1[500];
  int channel2[500];
  for (int i = 0; i < 500; i++) {
    channel1[i] = analogRead(A0);
    channel2[i] = analogRead(A1);
    Serial.println(channel1[i]);
    Serial.println(channel2[i]);
    delayMicroseconds(delaytime);
  }
  //do fourier
  //integrate
  //norm to 8 bit
  //display on matrix
}

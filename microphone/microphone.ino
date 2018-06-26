
void setup() {
 pinMode(D0,INPUT);
 pinMode(D7,OUTPUT);

}

void loop() {
  Serial.begin(9600);

  int SensorData=digitalRead(D0); 
  int adc = analogRead(D0);
  int dB = (adc+83.2073) / 11.003; 
  Serial.println(dB);
  if(SensorData==1){
    digitalWrite(D7,HIGH);
  }else{
     digitalWrite(D7,LOW);
  }
 }

float Kp = 2;
int cpwm = 130;

void setup(){
  Serial.begin(9600);
}

void loop(){
  float distance = (1 / (0.000413153 * analogRead(0) - 0.0055266887));
  maintainDistance(10, distance);
  delay(200);
}

void maintainDistance(float dist, float current){
  float error = current - dist;
  float output = error * Kp;
  output =  constrain(output, -50, 50);

  analogWrite(6, (int)(cpwm + output));
  analogWrite(11, (int)(cpwm - output) * 1.2);
  
  Serial.print(current);
  Serial.print("\t");
  Serial.print(output);
  
  Serial.println();
}

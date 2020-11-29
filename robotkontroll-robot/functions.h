//-- Functions --//
// The ones without comments should be self explanatory...

//-- Sensor functions
void ask_pin_F() { // test forward distance
  float Fdistance = sonar.ping_cm();
  Serial.print("F distance:");
  Serial.println(Fdistance);
  frontDist = Fdistance;
}
void ask_pin_L() { // test left distance
  senseServo.write(150);
  delay(delay_time);
  float Ldistance = sonar.ping_cm();;
  Serial.print("L distance:");
  Serial.println(Ldistance);
  leftDist = Ldistance;
}
void ask_pin_R() { // test right distance
  senseServo.write(20);
  delay(delay_time);
  float Rdistance = sonar.ping_cm();
  Serial.print("R distance:");
  Serial.println(Rdistance);
  rightDist = Rdistance;
}

//--Motor functions
void setSpeed(int speed) {
  analogWrite(motorAspeed, speed);
  analogWrite(motorBspeed, speed);
  leftSpeed = speed;
  rightSpeed = speed; 
}
void advance() {
  roboDirection = 1;
  setSpeed(mediumSpeed);
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,HIGH);
}
void turnR() {
  roboDirection = 3;
  setSpeed(mediumSpeed);
  digitalWrite(pinRB,LOW);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,LOW);
}
void turnL() {
  roboDirection = 2;
  setSpeed(mediumSpeed);
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,LOW);
  digitalWrite(pinLF,HIGH);
}
void stopp() {
  roboDirection = 0;
  setSpeed(stopSpeed);
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,HIGH);
  
}
void back() {
  roboDirection = 4;
  setSpeed(mediumSpeed);
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,LOW);
}

//---*--- Main Autonomous-Mode logic-loop function  ---*---//
void detection() { //test the distance of different direction 
  
  ask_pin_F(); // read forward distance
  
  if(frontDist < 25) { // if distance less then 25 cm
    if (frontDist != 0){
    setSpeed(mediumSpeed);
    ask_pin_L();
    delay(delay_time);
    ask_pin_R();
    delay(delay_time);
    if(leftDist > rightDist) {//if left distance more than right distance
      isTurning=true;
      turnStart=millis();
      turnL();
    }
    if(leftDist <= rightDist) { //if left distance not more than right distance
      isTurning=true;
      turnStart=millis();
      turnR();
    }
     if (leftDist < 10 && rightDist < 10) //if left distance and right
    //distance both less than 10 cm
    {
      isBacking=true;
      backStart=millis();
      back();
    }
    }
  }
  else {
    if (frontDist != 0){
    advance();
    
    }
  }
  senseServo.write(90);

}


//---*--- Main Manual-Mode logic-loop function  ---*---//
void analogAnalyzer(){
  xSpeed = map(xVal,analogMax, 1024, 70, 255);
  ySpeed = map(yVal,analogMax, 1024, 70, 255);
  xSpeedNeg = map(xVal,analogMin, 0, 70, 255);
  ySpeedNeg = map(yVal,analogMin, 0, 70, 255);
  xSpeedInv = ySpeed - xSpeed;
  xSpeedInvNeg = ySpeed - xSpeedNeg - 70;
  
  //stopp
  if(xVal < analogMax && xVal > analogMin && yVal < analogMax && yVal > analogMin){
  analogWrite(motorAspeed, 0);
  analogWrite(motorBspeed, 0);
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,HIGH);  }
  
  if(xVal < analogMax && xVal > analogMin && yVal > analogMax){
    //go forward
    analogWrite(motorAspeed, ySpeed);
    analogWrite(motorBspeed, ySpeed);
    digitalWrite(pinRB,LOW);
    digitalWrite(pinRF,HIGH);
    digitalWrite(pinLB,LOW);
    digitalWrite(pinLF,HIGH);
  }
  if(xVal > analogMin && yVal > analogMin){
    //go forward and right
    analogWrite(motorAspeed, ySpeed);
    analogWrite(motorBspeed, xSpeedInv);
    digitalWrite(pinRB,LOW);
    digitalWrite(pinRF,HIGH);
    digitalWrite(pinLB,LOW);
    digitalWrite(pinLF,HIGH);
  }
  if(xVal < analogMin && yVal > analogMin){
    //go forward and left
    analogWrite(motorAspeed, xSpeedInv);
    analogWrite(motorBspeed, ySpeed);
    digitalWrite(pinRB,LOW);
    digitalWrite(pinRF,HIGH);
    digitalWrite(pinLB,LOW);
    digitalWrite(pinLF,HIGH);
  }
  if(xVal < analogMin && xVal > analogMin && yVal < analogMin){
    //go backwards
    analogWrite(motorAspeed, ySpeedNeg);
    analogWrite(motorBspeed, ySpeedNeg);
    digitalWrite(pinRB,HIGH);
    digitalWrite(pinRF,LOW);
    digitalWrite(pinLB,HIGH);
    digitalWrite(pinLF,LOW);
  }
  if(xVal > analogMin && yVal < analogMin){
    //go backwards and right
    analogWrite(motorAspeed, ySpeedNeg);
    analogWrite(motorBspeed, xSpeedInv);
    digitalWrite(pinRB,HIGH);
    digitalWrite(pinRF,LOW);
    digitalWrite(pinLB,HIGH);
    digitalWrite(pinLF,LOW);
  }
  if(xVal < analogMin && yVal < analogMin){
    //go backwards and left
    analogWrite(motorAspeed, xSpeedInvNeg);
    analogWrite(motorBspeed, ySpeedNeg);
    digitalWrite(pinRB,HIGH);
    digitalWrite(pinRF,LOW);
    digitalWrite(pinLB,HIGH);
    digitalWrite(pinLF,LOW);
  }
  if(yVal < analogMin && yVal > analogMin && xVal > analogMin){
    //turn right
    analogWrite(motorAspeed, xSpeed);
    analogWrite(motorBspeed, xSpeed);
    digitalWrite(pinRB,LOW);
    digitalWrite(pinRF,HIGH);
    digitalWrite(pinLB,HIGH);
    digitalWrite(pinLF,LOW);
  }
  if(yVal < analogMin && yVal > analogMin && xVal < analogMin){
    //turn left
    analogWrite(motorAspeed, xSpeedNeg);
    analogWrite(motorBspeed, xSpeedNeg);
    digitalWrite(pinRB,HIGH);
    digitalWrite(pinRF,LOW);
    digitalWrite(pinLB,LOW);
    digitalWrite(pinLF,HIGH);
  }

}

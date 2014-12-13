// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
/// \author  Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2009 Mike McCauley
// $Id: ConstantSpeed.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

int LF_DIR = 28, LF_STEP = 26, LF_EN = 24,  
    LB_DIR = A1, LB_STEP = A0, LB_EN = 30, 
    RF_DIR = 34, RF_STEP = 36, RF_EN = 38,
    RB_DIR = A7, RB_STEP = A6, RB_EN = A2;
    
AccelStepper lf(1, LF_STEP, LF_DIR); 
AccelStepper lb(1, LB_STEP, LB_DIR); 
AccelStepper rf(1, RF_STEP, RF_DIR); 
AccelStepper rb(1, RB_STEP, RB_DIR); 

void setup()
{
  Serial.begin(115200);
  int maxs = 2000;
  
   lf.setMaxSpeed(maxs);
   lb.setMaxSpeed(maxs);
   rf.setMaxSpeed(maxs);
   rb.setMaxSpeed(maxs);
   
   changeSpeed(0,0);

   pinMode(LF_EN, OUTPUT);
   digitalWrite(LF_EN, LOW);	
   pinMode(RF_EN, OUTPUT);
   digitalWrite(RF_EN, LOW);	
   pinMode(LB_EN, OUTPUT);
   digitalWrite(LB_EN, LOW);	
   pinMode(RB_EN, OUTPUT);
   digitalWrite(RB_EN, LOW);	
}

void loop()
{ 
  static String cmd="";
  while(Serial.available()){
    char c = (char)Serial.read();
    cmd += c;
    if(c=='\n'){
      Serial.print("Process: ");
      Serial.println(cmd);
      processCommand(cmd);
      cmd="";
    }
  } 
  lf.runSpeed();
  lb.runSpeed();
  rf.runSpeed();
  rb.runSpeed();
}

void changeSpeed(int lspd, int rspd){
   lf.setSpeed(lspd);
   lb.setSpeed(lspd);
   rf.setSpeed(rspd);
   rb.setSpeed(rspd);  
}

void processCommand(String cmd){
  if (cmd.startsWith("f")) changeSpeed(1000, 1000);
  else if (cmd.startsWith("b")) changeSpeed(-400, -400);
  else if (cmd.startsWith("l")) changeSpeed(400, -400);
  else if (cmd.startsWith("r")) changeSpeed(-400, 400);
  else changeSpeed(0,0);
}

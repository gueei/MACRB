// Random.pde
// -*- mode: C++ -*-
//
// Make a single stepper perform random changes in speed, position and acceleration
//
// Copyright (C) 2009 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(1, 26, 28);

void setup()
{
  pinMode(24, OUTPUT);
  digitalWrite(24, LOW);
}


int dir = 1;
void loop()
{
    if (stepper.distanceToGo() == 0)
    {
	// Random change to speed, position and acceleration
	// Make sure we dont get 0 speed or accelerations
	delay(1000);
	stepper.moveTo(dir * 2000);
	stepper.setMaxSpeed(2000);
	stepper.setAcceleration(500);
        dir *= -1;
    }
    stepper.run();
}

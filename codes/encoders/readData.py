#!/usr/bin/env python

def readEncoders(arduino):
    encoders = arduino.readline().decode().strip().split("-")
    #leftMotor = (encoders[0], encoders[1])
    #rightMotor = (encoders[2], encoders[3])
    
    print(encoders)

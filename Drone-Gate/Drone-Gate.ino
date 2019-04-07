#include "Arduino.h"
#include "NewPing.h"
#include <RH_ASK.h>
#include <SPI.h> 

/*
   Arduino Gate
*/

// speed, rxPin, txPin, pttPin, pttInverted
RH_ASK driver(2000, 2, 3, 4, false);

// Ultrasonic Sensor
const uint16_t minDistance = 5;
const uint16_t maxDistance = 50;


// Ultrasonic Sensor #1
const uint8_t trigPin = 9;
const uint8_t echoPin = 10;

NewPing sonar1(trigPin, echoPin, maxDistance);

int redPin = 7;
int greenPin = 6;

bool finishGate = true;
bool startGate = false;

bool triggered = false;

void setup() {
    Serial.begin(19200); // Debugging only
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    driver.init();
}

void loop() {
  
    long dist =  sonar1.ping_cm();

    if(!triggered && dist > minDistance && dist < maxDistance) {
        triggered = true;

        if(finishGate) {
            const char *msg = "HIT         ";
            driver.send((uint8_t *)msg, strlen(msg));
            driver.waitPacketSent();
        }
        if(startGate) {
            const char *msg = "START       ";
            driver.send((uint8_t *)msg, strlen(msg));
            driver.waitPacketSent();
        }
    }
    
    uint8_t buf[12];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        // RESET
        String tmp = (char*)buf; 
        if(tmp.substring(0,5) == "RESET") {    
            triggered = false;
        }  
    }

    
    if(!triggered){
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, LOW);
    } else {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, HIGH);
    }

}


#include <RH_ASK.h>
#include <SPI.h>

/*
   Main Arduino
   Stopwatch + Reset
*/


RH_ASK driver(2000, 2, 3, 4, false);

long startTime;
bool runTime;

void setup()
{
    Serial.begin(19200); // Debugging only
    
    Serial.println("                    ");
    Serial.println("                    ");
    
    driver.init();

    pinMode(6, INPUT);

    startTime = millis();
    runTime = false;
}

void loop()
{
    if(runTime) {
        long delta = millis() - startTime;

        Serial.print("Time:  ");
        int seconds = (int)(delta/1000);
        if(seconds < 10)
          Serial.print("00");
        else if(seconds < 100)
          Serial.print("0");
        
        Serial.print(seconds);
        Serial.print(".");
        
        int fraction = (int)(delta%1000);
        if (fraction == 0)       
          Serial.print("000");
        else if (fraction < 10)       
          Serial.print("00");
        else if (fraction < 100)       
          Serial.print("0");
        
        Serial.print(fraction);
        Serial.print("   ");
        Serial.println("");
        Serial.println("");
    }
    
    uint8_t buf[12];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        String tmp = (char*)buf; 
        if(tmp.substring(0,3) == "HIT") {    
            runTime = false;
        }
        if(tmp.substring(0,5) == "START") {    
            startTime = millis();
            runTime = true;
        }
    }

    if(digitalRead(6) == 1) {
        const char *msg = "RESET       ";
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
        runTime = false;
        delay(100);
    }
}




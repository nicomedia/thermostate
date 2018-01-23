// Bylink Connection
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "9f7e0f63ae7949ecaa68fa328bf91b62";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = NETWORK_SSID;
char pass[] = NETWORK_PASSW;

// Initial desired temperature
int desired_temperature = 22;
int temp = 11;

// Kombi ON/OFF
BLYNK_WRITE(V0)
{
  int state = param.asInt();
  // Debug LED
  if(state == 1)
  {
    digitalWrite(4,HIGH);
  }
  else
  {
    digitalWrite(4,LOW);
  }
  Serial.println('K');
  Serial.println(state);
}

// Desired Temperature
BLYNK_WRITE(V1)
{
  desired_temperature = param.asInt();
  if(desired_temperature > 25)
  {
    digitalWrite(4,HIGH);
  }
  else
  {
    digitalWrite(4,LOW);
  }
  Serial.println('D');
  Serial.println(desired_temperature);
}

void setup() {
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
}

void loop() {
  // Read Serial Data 
  if(Serial.available() > 0)
  {
    char ch = Serial.read();
    // Read temperature value
    if(ch == 'T')
    {
      temp = Serial.parseInt();      
    }
    // Kombi ON
    else if(ch == 'X')
    {
      Blynk.virtualWrite(V2,1023);
      delay(100); 
      Blynk.virtualWrite(V3,0); 
      delay(100);
      Blynk.virtualWrite(V0,0);   
      delay(100);
    }
    // Kombi OFF
    else if(ch == 'Y')
    {
      Blynk.virtualWrite(V3,1023); 
      delay(100);
      Blynk.virtualWrite(V2,0);    
      delay(100);
      Blynk.virtualWrite(V0,1);
      delay(100);
    }
    else if(ch == 'G')
    {
      int gas_sensor = Serial.parseInt();
      Blynk.virtualWrite(V5,gas_sensor); 
    }
  }
  // Send temperature value to the mobile app
  Blynk.virtualWrite(V4,temp);
  
  Blynk.run();
}

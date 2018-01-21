#include <OneWire.h> // Required for ds18b20 temparature sensor 

#define TEMPERATURE_PIN   10
#define RELAY_PIN          5
#define POTENTIOMETER_PIN  2
#define GREEN_LED_PIN     13
#define RED_LED_PIN       12
#define YELLOW_LED_PIN    53
#define STATUS_LED_PIN    52
#define BUTTON_PIN        3

#define SENSOR_READ         1
#define POTENTIOMETER_READ   2

#define TEMP_ERR          -255

#define DESIRED_TEMP      22

OneWire  ds(TEMPERATURE_PIN);  // Normally 4.7k resistor is neccessary but i am using ready to use sensor card.

float desired_temperature = DESIRED_TEMP;
int button_val = HIGH;
int read_mode = SENSOR_READ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);   // declare the ledPin as an OUTPUT
  pinMode(RED_LED_PIN, OUTPUT);     // declare the ledPin as an OUTPUT
  pinMode(YELLOW_LED_PIN, OUTPUT);  // declare the ledPin as an OUTPUT
  pinMode(STATUS_LED_PIN, OUTPUT);  // declare the ledPin as an OUTPUT
  delay(250);
  blink_led();  
  pinMode(BUTTON_PIN,INPUT_PULLUP);

  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature_val = desired_temperature;

  button_val = digitalRead(BUTTON_PIN);

  Serial.print(" Button val :");
  Serial.print(button_val);
  Serial.println();

  if ( button_val == LOW )
  {
    if ( read_mode == SENSOR_READ )
    {
      read_mode = POTENTIOMETER_READ;
    }
    else
    {
      read_mode = SENSOR_READ;
    }
  }
  
  Serial.print(" read_mode :");
  Serial.print(read_mode);
  Serial.println();

  if ( read_mode == SENSOR_READ )
  {
      digitalWrite(STATUS_LED_PIN, HIGH);
      temperature_val = Read_Temperature_Celsius();

      Serial.print(" Temperature val :");
      Serial.print(temperature_val);
      Serial.println();
    
      if ( temperature_val == TEMP_ERR )
      {
          //error_cond();
          return;
      }
    
      if ( (temperature_val < (desired_temperature - 2)))
      {
          start_heater();
      }
      else if ( (temperature_val > (desired_temperature + 2)))
      {
          stop_heater();
      }
      else
      {
        // DO nothing!!! Keep last state.
      }
  }
  else if ( read_mode == POTENTIOMETER_READ )
  {
    digitalWrite(STATUS_LED_PIN, LOW);
    int potentiomter_val = analogRead(POTENTIOMETER_PIN);    // read the value from the sensor
    Serial.print(" Potantiometer val :");
    Serial.print(potentiomter_val);
    Serial.println();
  
    if(potentiomter_val > 500)
    {
      start_heater();
    }
    else
    {
      stop_heater();
    }
  }  

  delay(5000);
}

void mode_change(int new_mode)
{
  if ( new_mode == SENSOR_READ )
  {
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(250);
    digitalWrite(RED_LED_PIN, LOW);
    delay(250);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    delay(3000);
  }
  else 
  {
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(250);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(250);
    digitalWrite(YELLOW_LED_PIN, HIGH);
    delay(3000);
  }
}

void blink_led()
{
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(250);
  digitalWrite(YELLOW_LED_PIN, HIGH);
  delay(250);
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(2000);
  digitalWrite(GREEN_LED_PIN, LOW);
  delay(250);
  digitalWrite(RED_LED_PIN, LOW);
  delay(250);
  digitalWrite(YELLOW_LED_PIN, LOW);
  delay(250);  
  digitalWrite(STATUS_LED_PIN, LOW);
}

void error_cond()
{
  digitalWrite(GREEN_LED_PIN, LOW);
  delay(250);
  digitalWrite(RED_LED_PIN, LOW);
  delay(250);
  digitalWrite(YELLOW_LED_PIN, HIGH);
  delay(250);
}

void stop_heater()
{
  digitalWrite(RELAY_PIN, HIGH);
  delay(250);
  digitalWrite(GREEN_LED_PIN, LOW);
  delay(250);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(250);
  digitalWrite(YELLOW_LED_PIN, LOW);
  delay(250);
}

void start_heater()
{
  digitalWrite(RELAY_PIN, LOW);
  delay(250);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_LED_PIN, LOW);
  delay(250);
  digitalWrite(YELLOW_LED_PIN, LOW);
  delay(250);
}

float Read_Temperature_Celsius()
{
  byte index;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return TEMP_ERR;
  }
  
  Serial.print("ROM =");
  for( index = 0; index < 8; index++) {
    Serial.write(' ');
    Serial.print(addr[index], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return TEMP_ERR;
  }
  Serial.println();

// the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return TEMP_ERR;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( index = 0; index < 9; index++) {           // we need 9 bytes
    data[index] = ds.read();
    Serial.print(data[index], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");

  return celsius;
}

# thermostate
Home-made thermostate with arduino uno + esp8266 wifi

Arduino wifi + esp8266 wifi card :

https://tr.aliexpress.com/item/UNO-WiFi-R3-ATmega328P-ESP8266-32Mb-memory-USB-TTL-CH340G-Compatible-for-Arduino-Uno-NodeMCU-WeMos/32806748844.html?spm=a2g0s.9042311.0.0.CdPxYm

Temperature sensor : ds18b20

I used resistor included temperature card.
https://www.robotistan.com/mini-ds18b20-sicaklik-sensoru-karti?lang=tr&h=8e3d7304&gclid=Cj0KCQiAkZHTBRCBARIsAMbXLhHxNyBrOJAwbsfr22yjsicQv1qUM6T3fzAvHEPlUAHhQEEJ09k5LZoaAphHEALw_wcB

Gas sensor : MQ-4

I placed my thermostate to the kitchen. I used gas sensor to detect gas leakage.

https://tr.aliexpress.com/item/MQ-4-Methangas-Erdgas-Natural-Gas-Sensor-Module-Smoke-Detection-Alarm-Sensitive-Switch-For-Arduino-For/32733356496.html?spm=a2g0s.9042311.0.0.CdPxYm

Real time clock : DS3231

I don't impplement any functionality yet.
https://tr.aliexpress.com/item/DS3231-AT24C32-IIC-High-Precision-RTC-Module-Clock-Timer-Memory-Module/2037934408.html?spm=a2g0s.9042311.0.0.CdPxYm

TODO : I am going to implement RTC to open/close my heater after determined time like 2 hours later. I am also planning to implement a weekly schedule.

Button:

Ordinary button to open/close manually. ( In case there is no connection)

BLYNK:

I am using blynk as a MQTT server. It has nice free mobile app as well.


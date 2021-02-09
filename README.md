# SpeedingDetectionSystem
Project to build system that monitors speed via GPS

## Team Information 
### Saad Khan, Ibrahim Khan
#### ECE4180, Section B

## Description 
The Speeding Detection System is designed to track a driver's speed and notify them when they cross a certain speed or when they are driving unsafely. The system records when a violation occurs for later viewing by anyone interested. GPS is used to calculate the speed a driver is moving by using two longitude and latitude measurements. An accelerometer is used to detect sharp turns. When a driver crosses a certain speed or turns too sharply, a speaker plays a warning and a time stamp is recorded onto an SD card. 
An image of the system is shown below:

![system](images/speeddetectionsystem.jpg)

## Parts 
* MBED

![mbed](images/mbed.jpg)

* BR-355 with RS-232 Breakout and PS/2 Breakout

![gps](images/gps.jpg)
![br35](images/br355_optional_cable_sets.jpg)

* MicroSD breakout board

![microsd](images/microsd.jpg)

* Speaker

![speaker](images/speaker.jpg)

* LSM9DS1 IMU

![IMU](images/imu.jpg)

* TPA2005D1 Class D Audio Amp

![Amp](images/amp.jpg)

* uLCD-144-G2

![uLCD](images/ulcd.jpg)

* 5v External Power

## Wiring 

== BR-355 == 
| PS/2 breakout pin | MBED |
| ----------------- | ---- |
|Vcc                 |5V (external) |
|GND | GND |

| RS-232 breakout pin | MBED |
| ------------------- | ---- |
| Vcc | 5V (external) |
| GND | GND |
| TX | p14 | 

== MicroSD breakout ==
| MicroSD breakout pin | MBED |
| -------------------- | ---- |
| CS | p8 (DigitalOut) |
| DI | p5 (SPI mosi) |
| VCC | VOUT |
| SCK | p7 (SPI sclk) |
| GND | GND |
| DO | p6 (SPI miso) |

== Amp and Speaker ==
| MBED | TPA2005D1 | Speaker |
| ---- | --------- | ------- |
| GND | PWR-, IN- ||
|5V (external) | PWR+ ||
| p26 (PWM) | IN+ ||
|| OUT+ | + |
|| OUT- | - |

== LSM9DS1 ==
| IMU breakout pin | MBED |
| ---------------- | ---- |
| GND | GND |
| VDD | VOUT |
| SDA | p9 (SDA) |
| SCL | p10 (SCL) |

== uLCD ==
| uLCD | MBED |
| ---- | ---- |
| +5V | 5V (external) |
| TX | p27 |
| RX | p28 |
| GND | GND |
| RES | p30 |


### Video 
https://www.youtube.com/watch?v=xHG3McPrGxA&edit=vd


### Code 
Available at mbed repo: https://developer.mbed.org/users/gotmilk/code/SpeedDetectionSystem/

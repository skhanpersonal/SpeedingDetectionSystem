#include "mbed.h"
#include "math.h"
#include "GPS.h"
#include "uLCD_4DGL.h"
#include "LSM9DS1.h"
#include "SDFileSystem.h"
#include "wave_player.h"

#define M_PI  3.141592653589793238462

Serial pc(USBTX, USBRX);
GPS gps(p13, p14);
DigitalOut led2(LED1);
uLCD_4DGL uLCD(p28,p27,p30);
Timer t;
SDFileSystem sd(p5, p6, p7, p8, "sd");
AnalogOut DACout(p18);
wave_player waver(&DACout);


float oldlat, oldlong, oldtime;

float distance(float oldlat, float oldlong, float newlat, float newlong)
{
    oldlat = oldlat * M_PI / 180.0;
    oldlong = oldlong * M_PI / 180.0;
    newlat = newlat * M_PI / 180.0;
    newlong = newlong * M_PI / 180.0;
    const float r = 6378100;
    float rho1 = r * cos(oldlat);
    float z1 = r * sin(oldlat);
    float x1 = rho1 * cos(oldlong);
    float y1 = rho1 * sin(oldlong);

    float rho2 = r * cos(newlat);
    float z2 = r * sin(newlat);
    float x2 = rho2 * cos(newlong);
    float y2 = rho2 * sin(newlong);

    float dot = (x1 * x2 + y1 * y2 + z1 * z2);
    float cos_theta = dot / (r * r);
    float theta = acos(cos_theta);
    return r * theta;
}
//Algorithm reference: http://www.ridgesolutions.ie/index.php/2013/11/14/algorithm-to-calculate-speed-from-two-gps-latitude-and-longitude-points-and-time-difference/

void checkgps()
{
    uLCD.locate(0,1);
    if(gps.sample()) {
        time_t seconds = time(NULL);
        float newtime = t.read();
        float newlat = gps.latitude;
        float newlong = gps.longitude;
        float dist = distance(oldlat, oldlong, newlat, newlong);
        float timediff = newtime - oldtime;
        float speed = dist/timediff; //meters per second
        float speed_mph = (speed * 2.23);
        uLCD.printf("Longitude: %f degrees\n\rLatitude: %f degrees\n\r", newlong, newlat);
        uLCD.printf("Distance: %f\n\r", dist);
        uLCD.printf("D-time: %f\n\r", timediff);
        uLCD.printf("Speed in M/ph: %f\n\r", speed_mph);
        if (speed_mph > 50 && speed_mph < 100) {
            FILE *wave_file;
            wave_file=fopen("/sd/fast.wav","r");
            waver.play(wave_file);
            fclose(wave_file);
            FILE *fp = fopen("/sd/speedlog.txt", "w");
            if(fp == NULL) {
                uLCD.printf("Can't open file");
            }
            fprintf(fp, "Driver drove above limit on %s\n", ctime(&seconds));
            fclose(fp);
        }
        oldlat = newlat;
        oldlong = newlong;
        oldtime = newtime;
    } else {
        uLCD.printf("Oh Dear! No lock :(\n\r");
    }
}

int main()
{
    time_t seconds = time(NULL);
    uLCD.cls();
    uLCD.printf("start main\r\n");
    LSM9DS1 IMU(p9, p10, 0xD6, 0x3C);
    IMU.begin();
    if (!IMU.begin()) {
        uLCD.printf("Failed to communicate with LSM9DS1.\n");
    }
    IMU.calibrate(1);
    int counter, display = 0;
    t.start();
    t.reset();
    float newtime, accel = 0;
    oldtime = t.read();
    oldlat = gps.latitude;
    oldlong = gps.longitude;
    //checkgps();
    while(1) {
        newtime = t.read();
        if ((newtime-oldtime)>300) {
            checkgps();
            t.reset();
            oldtime = t.read();
        } else {
            while(!IMU.accelAvailable());
            IMU.readAccel();
            display++;
            if (display%100==0) {
                uLCD.locate(0,10);
                accel = IMU.calcAccel(IMU.ay);
                uLCD.printf("Y-acceleration in g's: %2f\n", accel);
                if (abs(accel) > .6 && abs(accel) < 1) {
                    counter++;
                } else {
                    counter = 0;
                }
                if (counter >= 2) {
                    FILE *wave_file;
                    wave_file=fopen("/sd/turn.wav","r");
                    waver.play(wave_file);
                    fclose(wave_file);
                    FILE *fp = fopen("/sd/speedlog.txt", "w");
                    if(fp == NULL) {
                        uLCD.printf("Can't open file");
                    }
                    fprintf(fp, "Driver turned above g-limit on on %s\n", ctime(&seconds));
                    fclose(fp);
                    counter = 0;
                }
            }
        }
    }
}


/*
$GPRMC,000115.039,V,,,,,,,291006,,*2C
$GPGGA,000116.031,,,,,0,00,,,M,0.0,M,,0000*52
$GPGSA,A,1,,,,,,,,,,,,,,,*1E
$GPGSV,3,1,12,20,00,000,,10,00,000,,31,00,000,,27,00,000,*7C
$GPGSV,3,2,12,19,00,000,,07,00,000,,04,00,000,,24,00,000,*76
$GPGSV,3,3,12,16,00,000,,28,00,000,,26,00,000,,29,00,000,*78
$GPRMC,000116.031,V,,,,,,,291006,,*27
$GPGGA,000117.035,,,,,0,00,,,M,0.0,M,,0000*57
$GPGSA,A,1,,,,,,,,,,,,,,,*1E
$GPRMC,000117.035,V,,,,,,,291006,,*22
$GPGGA,000118.039,,,,,0,00,,,M,0.0,M,,0000*54
$GPGSA,A,1,,,,,,,,,,,,,,,*1E
$GPRMC,000118.039,V,,,,,,,291006,,*21
$GPGGA,000119.035,,,,,0,00,,,M,0.0,M,,0000*59
$GPGSA,A,1,,,,,,,,,,,,,,,*1E
$GPRMC,000119.035,V,,,,,,,291006,,*2C
$GPGGA,000120.037,,,,,0,00,,,M,0.0,M,,0000*51
$GPGSA,A,1,,,,,,,,,,,,,,,*1E
$GPRMC,000120.037,V,,,,,,,291006,,*24
*/
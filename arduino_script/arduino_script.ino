#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <Mouse.h>

MPU6050 mpu;

uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;     
VectorFloat gravity;  
float ypr[3];    

float old_mx = -200;
float old_my = -200;
float dx, dy, mx, my;

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);
  packetSize = mpu.dmpGetFIFOPacketSize();
}

void loop() {
  while (fifoCount < packetSize) {
    fifoCount = mpu.getFIFOCount();
  }
  if (fifoCount >= 1024) {
    mpu.resetFIFO();
    fifoCount = mpu.getFIFOCount();
  } else {
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    mx = ypr[0] * 180 / M_PI;
    my = ypr[1] * 180 / M_PI;
    if (old_mx > -200)
    {
      if ((old_mx < -100) && (mx > 100)) {
        dx = (-180 - old_mx) + (mx - 180);
      } else if ((old_mx > 100) && (mx < -100)) {
        dx = (180 - old_mx) + (180 + mx);
      } else {
        dx = mx - old_mx;
      }
      dy = my - old_my;
      Mouse.move(2000 / 60 * dx, -1000 / 30 * dy);
    }
    old_mx = mx;
    old_my = my;
  }
}
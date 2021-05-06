#include <M5Stack.h>
#include "Queue.h"
#include "utility/MPU9250.h"

MPU9250 imu;
const int queueSize = 250;
Queue qx = Queue();
Queue qy = Queue();
Queue qz = Queue();
int counter = 0;
int graphMode = 0;

void setup() {

  M5.begin();
  Wire.begin();
  imu.initMPU9250();
  //imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);

  //M5.Lcd.setBrightness(200);
  //draw();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 33);
  M5.Lcd.print("x");
  M5.Lcd.setCursor(0, 113);
  M5.Lcd.print("y");
  M5.Lcd.setCursor(0, 193);
  M5.Lcd.print("z");
}

void loop() {
  draw();
  if (M5.BtnA.read()) {
    init(1);
  }
  if (M5.BtnB.read()) {
    init(2);
  }
  if (M5.BtnC.read()) {
    init(3);
  }
}

void draw() {
  for (int i = 0; i < 3; i++) {
    //M5.Lcd.drawLine(20, 5 + 80 * i, 20, 75 + 80 * i, WHITE);
    M5.Lcd.drawFastVLine(20, 5 + 80 * i, 70, WHITE);
    //M5.Lcd.drawLine(20, 40 + 80 * i, 300, 40 + 80 * i, WHITE);
    M5.Lcd.drawFastHLine(20, 40 + 80 * i, 280, WHITE);
    M5.Lcd.drawLine(300, 40 + 80 * i, 290, 35 + 80 * i, WHITE);
    M5.Lcd.drawLine(300, 40 + 80 * i, 290, 45 + 80 * i, WHITE);
    M5.Lcd.drawLine(20, 5 + 80 * i, 15, 15 + 80 * i, WHITE);
    M5.Lcd.drawLine(20, 5 + 80 * i, 25, 15 + 80 * i, WHITE);
  }
  switch (graphMode) {
    case 0:
      M5.Lcd.setCursor(60, 75);
      M5.Lcd.setTextSize(1);
      M5.Lcd.print("Press Button to start tracking...\r\n");
      M5.Lcd.setCursor(50, 145);
      M5.Lcd.print("A : Linear acceleration sensor\r\n");
      M5.Lcd.setCursor(50, 160);
      M5.Lcd.print("B : Gyro sensor\r\n");
      M5.Lcd.setCursor(50, 175);
      M5.Lcd.print("C : Calibrate the MPU-6500(6-Axis sensor)");
      break;
    case 1:
      if (imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
        erase();
        imu.readAccelData(imu.accelCount);
        imu.getAres();
        imu.ax = (float)imu.accelCount[0] * imu.aRes * 20;
        imu.ay = (float)imu.accelCount[1] * imu.aRes * 20;
        imu.az = (float)imu.accelCount[2] * imu.aRes * 20;
        qx.push((int)imu.ax);
        qy.push((int)imu.ay);
        qz.push((int)imu.az);
        counter++;
        plot(20.0);
      }
      break;
    case 2:
      if (imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
        erase();
        imu.readGyroData(imu.gyroCount);
        imu.getGres();
        imu.gx = (float)imu.gyroCount[0] * imu.gRes * 0.2;
        imu.gy = (float)imu.gyroCount[1] * imu.gRes * 0.2;
        imu.gz = (float)imu.gyroCount[2] * imu.gRes * 0.2;
        qx.push((int)imu.gx);
        qy.push((int)imu.gy);
        qz.push((int)imu.gz);
        counter++;
        plot(0.2);
      }
      break;
    case 3:
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextSize(3);
      M5.Lcd.setCursor(0, 75);
      M5.Lcd.print(" Please place me\n on a level\n surface and then\n press C button!!");
      delay(500);
      while (!M5.BtnC.read());
      imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);
      break;
  }
  delay(50);
}

void init(int gMode) {
  M5.Lcd.fillRect(15, 0, 320, 240, BLACK);
  graphMode = gMode;
  counter = 0;
  qx.clear();
  qy.clear();
  qz.clear();
}

void plot(float mul) {
  for (int i = 0; i < queueSize; i++) {
    for (int j = 0; j < 3; j++) {
      M5.Lcd.drawPixel(i + 21, 40 + 80 * j - (j == 0 ? qx : j == 1 ? qy : qz).get(i), WHITE);
      if (i != 0) {
        int t1 = (j == 0 ? qx : j == 1 ? qy : qz).get(i);
        int t2 = (j == 0 ? qx : j == 1 ? qy : qz).get(i - 1);
        M5.Lcd.drawFastVLine(i + 21, 40 + 80 * j - (t1 > t2 ? t1 : t2), t1 > t2 ? t1 - t2 : t2 - t1, WHITE);
      }
    }
  }
  for (int i = 0; i < 3; i++) {
    M5.Lcd.drawFastVLine(counter < queueSize ? 21 + counter : 21 + queueSize, 5 + 80 * i, 70, GREEN);
  }
}

void erase() {
  for (int i = 0; i < queueSize; i++) {
    for (int j = 0; j < 3; j++) {
      M5.Lcd.drawPixel(i + 21, 40 + 80 * j - (j == 0 ? qx : j == 1 ? qy : qz).get(i), BLACK);
      if (i != 0) {
        int t1 = (j == 0 ? qx : j == 1 ? qy : qz).get(i);
        int t2 = (j == 0 ? qx : j == 1 ? qy : qz).get(i - 1);
        M5.Lcd.drawFastVLine(i + 21, 40 + 80 * j - (t1 > t2 ? t1 : t2), t1 > t2 ? t1 - t2 : t2 - t1, BLACK);
      }
    }
  }
  for (int i = 0; i < 3; i++) {
    M5.Lcd.drawFastVLine(counter < queueSize ? 21 + counter : 21 + queueSize, 5 + 80 * i, 70, BLACK);
  }
}

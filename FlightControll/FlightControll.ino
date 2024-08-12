///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//                      Полетный контролер                                       //
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////










////////////////////////////////////////////////////////////////////////
// Гироскоп + аксель
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;
volatile bool mpuFlag = false;  // флаг прерывания готовности
uint8_t fifoBuffer[45];         // буфер


////////////////////////////////////////////////////////////////////////
// Компас
#include <Arduino.h>
#include <Wire.h>
#include <HMC5883L_Simple.h>

HMC5883L_Simple Compass;

////////////////////////////////////////////////////////////////////////
// Глобальные переменные (почти)
float ypr[3];


////////////////////////////////////////////////////////////////////////
// Сетап
////////////////////////////////////////////////////////////////////////
void setup() {
  // Общее
  Serial.begin(9600);
  Wire.begin();

  // инициализация DMP и прерывания для гиро-акселя
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);
  attachInterrupt(1, dmpReady, RISING);

  // Инициализация компаса
  Compass.SetDeclination(10, 15, 'E');                // Магнитное склонение
  Compass.SetSamplingMode(COMPASS_SINGLE);            // РЕжим работы (по запросам/постоянный)
  Compass.SetScale(COMPASS_SCALE_088);                // Чувствительность
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH); // Ориентация (норм работает торлько это, но  Y - NORTH)
}

// прерывание готовности. Поднимаем флаг
void dmpReady() {
  mpuFlag = true;
}

void loop() {
  GetPosDev();
  Serial.println("pos");
  Serial.print(ypr[0]); // вокруг оси Z
  Serial.print(',');
  Serial.print(ypr[1]); // вокруг оси Y
  Serial.print(',');
  Serial.print(ypr[2]); // вокруг оси X
  Serial.println();
  
  Serial.println(GetAzDev());

  delay(1000);
}

// Get Position deviation
void GetPosDev() {
  if (mpuFlag & mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    Quaternion q;
    VectorFloat gravity;

    // расчёты
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    mpuFlag = false;

    // выводим результат в радианах (-3.14, 3.14)
    Serial.println("pos");
    Serial.print(ypr[0]); // вокруг оси Z
    Serial.print(',');
    Serial.print(ypr[1]); // вокруг оси Y
    Serial.print(',');
    Serial.print(ypr[2]); // вокруг оси X
    Serial.println();
  }
}

float GetAzDev() {
  return Compass.GetHeadingDegrees();
}

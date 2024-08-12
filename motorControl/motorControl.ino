#include <Servo.h>

Servo motor;



#include <SoftwareSerial.h>
#include "EBYTE22.h"

// Константы для Arduino

#define PIN_TX 4
#define PIN_RX 5
#define PIN_M0 6
#define PIN_M1 7
#define PIN_AX 3


SoftwareSerial E22Serial(PIN_TX, PIN_RX, false);    // Создаем объект SoftwareSerial для соединения с модулем через программный UART

EBYTE22 E22(&E22Serial, PIN_M0, PIN_M1, PIN_AX);   // Создаем экземпляр класса EBYTE22

struct controlData
{
  int thrust;
} controlData;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  E22Serial.begin(9600);                    // Устанавливаем соединение с модулем (скорость 9600).

  Serial.println("Starting Sender");
  delay(200);

  // При инициализации и некоторых других операциях (применение настроек, установка ключа), модуль переходит в режим конфигурации, в этом режиме UART модуля работает только на скорости 9600!!!
  E22.init();                        // Инициализируем модуль(конфигурируются выводы контроллера, считываютс
  
  motor.attach(2);
  motor.writeMicroseconds(2300);
  delay(2000);

  motor.writeMicroseconds(800);
  delay(6000);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (E22.available()) {
    // Если есть (E22.available() вернуло "true"), начинаем читать данные.
    E22.getStruct(&controlData, sizeof(controlData));
    
  motor.writeMicroseconds(controlData.thrust);


}
}

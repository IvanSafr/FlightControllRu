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

struct droneData {
  float xDev;
  float yDev;
  float zDev;
  unsigned int azimuth;
} droneData;


struct controlData
{
  int thrust;
} controlData;



void setup() {
  Serial.begin(9600);                       // Устанавливаем соединение с компьютером (телефоном) для отладки.
  E22Serial.begin(9600);                    // Устанавливаем соединение с модулем (скорость 9600).

  Serial.println("Starting Sender");
  delay(200);

  // При инициализации и некоторых других операциях (применение настроек, установка ключа), модуль переходит в режим конфигурации, в этом режиме UART модуля работает только на скорости 9600!!!
  E22.init();                        // Инициализируем модуль(конфигурируются выводы контроллера, считываются настройки модуля).

}

void loop() {
  controlData.thrust = map(analogRead(0), 0, 1023, 800, 2300);
  
  if (E22.getBusy()) {                       // Если модуль чем-то занят (на выводе AUX логический 0),
    E22.completeTask(50);                     // подождем, пока освободится с таймаутом в 5 секунд.
  }
  
  Serial.println(controlData.thrust);
  
  E22.sendStruct(&controlData, sizeof(controlData));
  E22.flush();
 
}

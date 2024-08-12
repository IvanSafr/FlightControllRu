/*
  В этом примере показано, как настроить модуль на конкретный адрес и принимать строковые сообщения.
*/

#include <SoftwareSerial.h>
#include "EBYTE22.h"

// Константы для Arduino

#define PIN_TX 2
#define PIN_RX 3
#define PIN_M0 4
#define PIN_M1 5
#define PIN_AX 6

// Константы для плат на ESP8266
/*
#define PIN_RX 14
#define PIN_TX 12
#define PIN_M0 5
#define PIN_M1 4
#define PIN_AX 13
*/



SoftwareSerial E22Serial(PIN_TX, PIN_RX, false);  // Создаем объект SoftwareSerial для соединения с модулем через программный UART

EBYTE22 E22(&E22Serial, PIN_M0, PIN_M1, PIN_AX);  // Создаем экземпляр класса EBYTE22

// Строка, в которую считаем полученное сообщение.

String message;

void setup() {
 Serial.begin(9600);                              // Устанавливаем соединение с компьютером (телефоном) для отладки.
 E22Serial.begin(9600);                           // Устанавливаем соединение с модулем (скорость 9600).
 Serial.println("Starting Sender");
 delay(200);
 
  // При инициализации и некоторых других операциях (применение настроек, установка ключа), модуль переходит в режим конфигурации, в этом режиме UART модуля работает только на скорости 9600!!!
  if(E22.init())                           // Инициализируем модуль(конфигурируются выводы контроллера, считываются настройки модуля).
 {Serial.println("init OK");} else{
  Serial.println("init Error");}           // Если что-то пошло не так, выводим сообщение об ошибке.
 
// В случае ошибки, проверьте правильность подключения и перезапустите модуль с контроллером сбросом питания (кнопка сброса контроллера НЕ ПОМОГАЕТ).

 E22.setAddress(5);                        // Установим адрес модуля - 5.
 E22.writeSettings(TEMPORARY);             // Применяем настройки временно (до отключения питания)
  // Теперь модуль может принимать данные, отправленные на адрес 5 и 65535(широковещательный).
}

void loop() {

// Проверяем, есть ли непрочитанные данные.
if (E22.available()){  // Если есть (E22.available() вернуло "true"), начинаем читать данные.

message = E22Serial.readString();         // Считываем полученные данные в строку

Serial.println("Message: "+ message);   // Выводим строку в монитор порта
}


delay(1000);  // Подождем немного.

}

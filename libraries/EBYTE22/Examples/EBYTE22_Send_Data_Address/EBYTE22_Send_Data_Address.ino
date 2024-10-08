/*
  В этом примере показано, как отправлять данные на конкретные адреса.
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

  // Сообщения, которые будут отправлены на соответствующий адрес.

String testString0 = "TEST Addres: 0";
String testString5 = "TEST Addres: 5";
String testString65535 = "TEST Addres: 65535";

void setup() {
 Serial.begin(9600);                             // Устанавливаем соединение с компьютером (телефоном) для отладки.
 E22Serial.begin(9600);                          // Устанавливаем соединение с модулем (скорость 9600).
 Serial.println("Starting Sender");
delay(200);
 
  // При инициализации и некоторых других операциях (применение настроек, установка ключа), модуль переходит в режим конфигурации, в этом режиме UART модуля работает только на скорости 9600!!!
  if(E22.init())                                 // Инициализируем модуль(конфигурируются выводы контроллера, считываются настройки модуля).
 {Serial.println("init OK");} else{
  Serial.println("init Error");}                 // Если что-то пошло не так, выводим сообщение об ошибке.
 
  // В случае ошибки, проверьте правильность подключения и перезапустите модуль с контроллером сбросом питания (кнопка сброса контроллера НЕ ПОМОГАЕТ).

  // Настроим модуль на передачу с указанием адреса. 
 E22.setTransmissionMode(TXM_FIXED);             // Режим фиксированной передачи
 E22.writeSettings(TEMPORARY);                   // Применяем настройки временно (до отключения питания)
}

void loop() {

if (E22.getBusy()){                              // Если модуль чем-то занят (на выводе AUX логический 0), 
  E22.completeTask(5000);                        // подождем, пока освободится с таймаутом в 5 секунд.
}

  // В режиме фиксированной передачи, перед каждой отправкой данных, нужно вызвать метод для установки цели.
  // Первый аргумент - целевой адрес в диапазоне 0x0000 - 0xFFFF или 0 - 65535, второй - канал (от которого зависит частота).
  // Частота рассчитывается как 410.125 + канал (для модулей на 433МГц), в нашем случае 410.125 + 23 = 433.125МГц.
  // Метод возвращает значение типа bool (true - успех, false - ошибка, модуль чем - то занят или функция фиксированной передачи отключена).
  // Зададим цель - адрес 0, канал 23.
E22.sendTarget(0, 23); // Метод для установки целевого адреса, передает модулю 3 байта данных (2 - адрес и 1 - канал).

  // Теперь, без каких либо задержек, передаем данные, предназначенные целевому адресату.
E22Serial.print(testString0);
delay(2000);                  // подождем 2 секунды

  // После того, как модуль получит все данные для отправки и "увидит", что больше ничего не поступает, он будет ожидать новый адрес.
  // Даже если адресат не изменяется, вам все равно нужно вывзвать метод sendTarget, иначе первые 3 байта данных будут расценены как новый адрес, 
  // и все уйдет неизвестно куда.
  // С другой стороны, перед тем как задать новый адрес и начать передачу, нужно дождаться завершения передачи на старый адрес, иначе модуль расценит
  // новый адрес как обычные данные, которые будут отправлены на старый адрес.

  // Отправим еще 2 сообщения, на адрес 5 и 65535(широковещательный).
E22.sendTarget(5, 23);
E22Serial.print(testString5);
delay(2000);

E22.sendTarget(65535, 23);
E22Serial.print(testString65535);
delay(5000);

}

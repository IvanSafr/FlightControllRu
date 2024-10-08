/*
  Этот пример показывает, как настроить радиомодуль EBYTE на передачу данных в режиме "WOR".
  Этот режим служит для экономии энергии приемника (но не передатчика), что особо актуально при работе от аккумулятора.
  Минусом данного режима является то, что приемник может только принимать данные, а передавать не может. То есть связь односторонняя.
  Тем не менее контроллер приемника может переключить модуль в другой режим для передачи данных, а потом вернуться в режим "WOR".
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

int count = 0;

SoftwareSerial E22Serial(PIN_TX, PIN_RX, false);  // Создаем объект SoftwareSerial для соединения с модулем через программный UART

EBYTE22 E22(&E22Serial, PIN_M0, PIN_M1, PIN_AX);  // Создаем экземпляр класса EBYTE22

void setup() {
 Serial.begin(9600);                              // Устанавливаем соединение с компьютером (телефоном) для отладки.
 E22Serial.begin(9600);                           // Устанавливаем соединение с модулем (скорость 9600).
 Serial.println("Starting Sender");
delay(200);
 
 // При инициализации и некоторых других операциях (применение настроек, установка ключа), модуль переходит в режим конфигурации, в этом режиме UART модуля работает только на скорости 9600!!!
  if(E22.init())                                  // Инициализируем модуль(конфигурируются выводы контроллера, считываются настройки модуля).
 {Serial.println("init OK");} else{
  Serial.println("init Error");}                  // Если что-то пошло не так, выводим сообщение об ошибке.
 
// В случае ошибки, проверьте правильность подключения и перезапустите модуль с контроллером сбросом питания (кнопка сброса контроллера НЕ ПОМОГАЕТ).
   
 E22.setWOR(WOR_TRANSMITTER);                     // Конфигурируем модуль как передатчик
 E22.setWORCycle(WOR2000);                        // Устанавливаем время цикла WOR равным 2000мс (2сек). У приемника и передатчика этот параметр должен быть одинаковым.
 E22.writeSettings(TEMPORARY);                    // Применяем настройки временно (до потери питания).
 E22.setMode(MODE_WOR);                           // Устанавливает режим работы WOR.

}

void loop() {

count++;                                          // Добавляем единицу к счетчику.
E22Serial.print("Message №: " + String(count));  // Отправляем сообщение с номером.

// Так как мы установили время цикла WOR - 2000мс (2сек), передатчик будет транслировать маркер 2 секунды, после чего начнет передачу нашего сообщения, и так каждый раз.
// Приемник, у которого время цикла WOR такое же, будет "просыпаться" каждые 2 секунды и "слушать" маркер, при обнаружении которого будет готов к приему сообщения.
// Таким образом приемник не будет работать все время, а лишь какой-то момент, каждые 2 секунды. Это позволит значительно сэкономить заряд батареи.

delay(30000); // Подождем пол минуты и отправим следующее сообщение.





}

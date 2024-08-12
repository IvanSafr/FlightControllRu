/*
  В этом примере показано, как принимать данные различных типов, используя радиомодули от EBYTE.
  При использовании разнотипных контроллеров, пример может не работать (из-за различного представления структур).

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



SoftwareSerial E22Serial(PIN_TX, PIN_RX, false);        // Создаем объект SoftwareSerial для соединения с модулем через программный UART

EBYTE22 E22(&E22Serial, PIN_M0, PIN_M1, PIN_AX);        // Создаем экземпляр класса EBYTE22

  // Переменные, в которые будут записаны полученные значения (байт, строка и структура, которая содержит несколько объектов разного типа).

byte testByte;
String testString;

struct myStruct
{
  int days;
  float pi;
  long lightSpeed;
  char ch;
} testStruct;


void setup() {
 Serial.begin(9600);                       // Устанавливаем соединение с компьютером (телефоном) для отладки.
 E22Serial.begin(9600);                    // Устанавливаем соединение с модулем (скорость 9600).
 Serial.println("Starting Sender");
 delay(200);
 
  // При инициализации и некоторых других операциях (применение настроек, установка ключа), модуль переходит в режим конфигурации, в этом режиме UART модуля работает только на скорости 9600!!!
  if(E22.init())                           // Инициализируем модуль(конфигурируются выводы контроллера, считываются настройки модуля).
 {Serial.println("init OK");} else{
  Serial.println("init Error");}           // Если что-то пошло не так, выводим сообщение об ошибке.
 
// В случае ошибки, проверьте правильность подключения и перезапустите модуль с контроллером сбросом питания (кнопка сброса контроллера НЕ ПОМОГАЕТ).
   
}

void loop() {

  // Проверяем, есть ли непрочитанные данные.
if (E22.available()){ 
  // Если есть (E22.available() вернуло "true"), начинаем читать данные.

/*
 Читаем структуру. В качестве первого аргумента должен быть адрес на структуру.
 Для этого перед именем переменной пишется символ "&", подробнее см. "C++ работа с указателями".
 В качестве второго аргумента - размер структуры в байтах. Используйте метод sizeof, как это показано ниже.
 Если ваша структура содержит строки (String), метод sizeof не сможет правильно определить размер структуры,
 что приведет к ошибкам. Поэтому используйте строки отдельно.
 Метод "getStruct" возвращает значение типа bool (true - успех, false - ошибка).
*/
E22.getStruct(&testStruct, sizeof(testStruct));

// Читаем один байт
testByte = E22.getByte();

/*
 Читаем строку. Этот метод считывает в строку все непрочитанные данные, если бы он стоял первым,
 то в строку считалась бы структура и байт. Есть еще один выход, использовать метод, который считывает
 данные в строку до определенного символа, например "*".  В таком случае, нужно добавить этот символ в конец строки в скетче передатчика.
 Пример: testString = E22Serial.readStringUntil('*');
 Как вы заметили, здесь мы обращаемся не к объекту бибилотеки EBYTE22, а к объекту SoftwareSerial.
 Я не стал дублировать чтение строк в библиотеку, так как это делает ее "тяжелее", но не добавляет ничего нового.
 При работе с модулем, вы можете использовать и другие методы класса SoftwareSerial.
*/
testString = E22Serial.readString();

// Теперь выведем все данные в монитор порта, конвертируя числовые переменные в строку.

Serial.println("--------");
Serial.println("Структура:");
Serial.println("Дней в году: " + String(testStruct.days));
Serial.println("Число Пи: " + String(testStruct.pi));
Serial.println("Скорость света м/c: " + String(testStruct.lightSpeed));
Serial.println("Символ: " + String(testStruct.ch));
Serial.println("--------");

Serial.println("Байт: " + String(testByte));

Serial.println("Строка: "+ testString);

if(E22.getRSSIAmbient() == RSSI_ENABLE){                                        // Проверяем, включена ли фунция замера RSSI в настройках модуля.
Serial.println("Уровень сигнала: " + String(E22.getRSSI(RSSI_LAST_RECEIVE)));   // Если да, выводим уровень сигнала последнего приема в монитор порта.
Serial.println("Уровень шума: " + String(E22.getRSSI(RSSI_AMBIENT)));           // И уровень шума.
  } else {                                                                      // Значение RSSI выдается "в попугаях" от 0 до 255 (чем больше число, тем лучше сигнал).
    Serial.println("Функция RSSI отключена");                                   // Если функция замера RSSI отключена, выводим сообщение.
    }

}

delay(1000);    // Подождем немного.

}

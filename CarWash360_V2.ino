/**
   Контроллер-исполнительное устройство ДЛЯ МОЙКИ CARWASH360

   Version - 2.0
   Platform: Arduino MEGA  + EthernetShield W5100
   IDE: Arduino 1.8.11

   !!!!!!!!!!!!! версия для Arduino MEGA!!!!!!

   Написана для мойки на ул Свердловский проспект

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   исполнительные устройства (реле) подключены к Digital 44 - 51

   обращение по http://xx.xx.xx.xx/ выдаст справочную информацию по этому устройству (нужно для того, чтобы когда обращаешься
   по IP к устройству понять что это за контроллер и пр.)

   /state - состояние всез портов
   /command - выполнение команды
           команды можно вызывать серией в 1 запросе. Например http://xx.xx.xx.xx/command?3=CLICK&4=CLICK&5=ON&6=OFF
           только длинна строки запроса не должна привышать maxLength
   /getdev - получить список всех устройст на 1-wire
           формат вывода:
                  T<номер устройства на шине>:<HEX адрес устройства>:<текущая температура в градусах цельсия>;[...]
                  (пример T0:1060CF59010800E3:24.06;T1:109ABE59010800FE:24.56;)

 **/
#define DEBUG 0 // вывод в консоль

#define TELEGRAMM 1 // Отправка в телегу

#include "SPI.h"
#include "Ethernet.h"
//#include "Arduino.h"
#include "WebServer.h" // Webduino (https://github.com/sirleech/Webduino)
#include <avr/wdt.h>

#include "DHT.h"
#define DHTPIN1 2     // Digital pin connected to the DHT sensor
#define DHTPIN2 3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
float humid ;
float temp;
float hic;

float humid2 ;
float temp2;
float hic2;


// мак адрес
static uint8_t mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// ip-адрес устройства
static uint8_t ip[] = { 192, 168, 1, 200 };
//static uint8_t ip[] = { 10, 170, 1, 180 };
// ip-адрес удалённого сервера
//byte rserver[] = {10, 170, 1, 1 };
byte rserver[] = {192, 168, 1, 1 };
//byte myDns[] = {10, 170, 1, 1 };
byte myDns[] = {192, 168, 1, 1 };
// Настройки выходов
int startPin = 29;
int endPin = 45;
// Настройки входов
int startinPin = 46;
int endinPin = 52;

/*
  44 - мойка - режим 1
  45 - мойка - режим 2
  46 - ?
  47 - ?
  48 - ?
  49 - Врата вверх
  50 - врата вниз
  51 - свет

*/
//Массив названий Операций
char* operateStrings[] = {"Мойка-1", "Мойка-2", "Пауза", "Стоп", "пусто", "Врата вверх", "Врата вниз", "Свет", "Вытяжка"};
//массив Пинов
int operatePins[] = {31, 33, 35, 37, 39, 41, 43, 45,29};
//массив Клик или ON/OFF
int operateOnOff[] = {0, 0, 0, 0, 0, 0, 0, 1,1};
//Массив инвертирована ли логика - зажигаем НУЛЕМ. Все пины на модуле. 
int inverceLogic[] = {1, 1, 1, 1, 1, 1, 1, 1,0};
int busy = 0;
int busytimer = 0;
int lightSwitch = 0;

//Массив названий Статусов
char* washStatusStrings[] = {"Вперед", "Стоп", "Назад", "Эмульсия", "Мойка", "Шампунь", "Воск", "Сушка"};
char* washStatusEngStrings[] = {"wash", "stop", "goback", "emulse", "realwash", "shampoo", "wax", "dry"};
//Пороговое значение показывающее что на пине Логическая 1
//int borderValue = 200;
int borderValue = 35;
//массив Пинов
//используется массив а не простой перебор для того чтобы можно было просто перенести пины в случае чего
//char* washStatusPins[] = {"A8", "A9", "A10", "A11", "A12", "A13", "A14", "A15"};
int washStatusPins[] = {62, 63, 64, 65, 66, 67, 68, 69};
//массив Клик или ON/OFF
int washStatusOnOff[] = {0, 0, 0, 0, 0, 0, 0, 0};
int gateLongClick;

// Настройка переменных отображения

String contentText;
String endText;
String htmlText;

const char data_message[] PROGMEM = {"Hello!"};
//#define BASE_TEXT "<!DOCTYPE html><style>html{background: #cee2e1; /* Old browsers */background: -webkit-linear-gradient(top,  #6ec1e4 0%,#ffffff 100%); background: linear-gradient(to bottom,  #6ec1e4 0%,#ffffff 100%); background-repeat:  no-repeat;background-size:  cover;div.buttons{float: left;}font-family: Verdana,Helvetica,Sans;color: #666;}a{text-decoration: none; color: #666; }.bold{color:  #000;}a:visited{color: #ffffff;}.base{max-width: 900px; margin: 0 auto;}.header{height: 120px;}.logo{float: left;  font-size: 22px;}.menu{float:right;color: #ffffff; margin-top: 56px}.content{border:  #666 solid 1px;border-radius: 0px;padding: 6px;background-color:  #ffffff;}.inset {color: #ffffff;text-shadow: -1px -1px 1px #000, 1px 1px 1px #fff;}ul.hr {margin: 0; padding: 2px; }ul.hr li {display: inline; border-right: 1px solid #000; padding-right: 6px;text-transform:  uppercase;font-weight:  400;}ul.hr li:last-child { border-right: none;}a.knopka {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1.5em; margin-right: 10px; margin-bottom: 10px; outline: none; } a.label {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1em; margin-right: 3px; margin-bottom: 10px; outline: none; } a.knopka:hover { background: rgb(232,95,76); } .blue {background: rgb(56, 162, 212); } .red { background: rgb(212,75,56); } a.knopka:active { background: rgb(152,15,0); } </style><html><head><title>CARWASH360</title><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><div class=\"base\"><div class=\"header\"><div class=\"logo\"><h1 class=\"inset\">CARWASH360</h1></div><div class=\"menu\"><ul class=\"hr inset\"><a href=\"/\"><li>Главная</li></a><a href=\"/help\"><li>Помощь</li></a><a href=\"/contacts\"><li>Контакты</li></a></ul></div></div><div class=\"content\">"
#define VERSION_STRING "2.0"
#define COMPILE_DATE_STRING "2021-03-25"


P(location_info) = "Controller - CarWash360";
P(pin_info) = "D2 - DHT-11, D3 - DHT-11 <br> <br>D44-D51 - outputs";
P(version_info) = VERSION_STRING ". Compile date: " COMPILE_DATE_STRING;
P(footer) = "</div></body></html>";


String url = String(25);
int maxLength = 25; // Максимальная длинна строки запроса

#define delayClick 1000 // задержка при обычном CLICK
#define delayLClick 2000 // задержка при длинном LCLICK
#define MAX_COMMAND_LEN             (10)
#define MAX_PARAMETER_LEN           (10)
#define COMMAND_TABLE_SIZE          (8)
#define PREFIX ""

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
WebServer webserver(PREFIX, 80);


// Для поиска
float tempC;
long previousMillis;
long previousMillisBusy1;
long previousMillisBusy2;

#define NAMELEN 32
#define VALUELEN 32

char gCommandBuffer[MAX_COMMAND_LEN + 1];
char gParamBuffer[MAX_PARAMETER_LEN + 1];
long gParamValue;

typedef struct {
  char const    *name;
  void          (*function)(WebServer &server);
}
command_t;
/**********************************************************************************************************************/


//http://www.2.toutlem.z8.ru/scripts/SendToTelegram.php?site=simple&token=638757195:AAFe2wBhVmsNdZnwVv4a3FOKcNGsDD1Helk&chat_id=330614598&message=test


/**********************************************************************************************************************/
void setup() {
  wdt_disable(); // бесполезная строка до которой не доходит выполнение при bootloop
  // Для дебага будем выводить отладочные сообщения в консоль
  //TODO Убрать вывод в консоль "за дабаг" (т.е. вывод только если скимпилированно с поддержкой дебага)
#if (DEBUG == 1)
  Serial.begin(9600);
  Serial.println("Start");
#endif

  dht1.begin();
  dht2.begin();
  //  Ethernet.begin(mac, ip); // Инициализируем Ethernet Shield
  // give the Ethernet shield a second to initialize:
  delay(5000);

  //DHCP НЕ потребовался. Пока отключим
  // Но оставим код.
#if (DEBUG == 1)
  Serial.println(F("Initialize Ethernet with DHCP:"));
#endif

  if (Ethernet.begin(mac) == 0) {
#if (DEBUG == 1)
    Serial.println(F("Failed to configure Ethernet using DHCP"));
#endif

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
#if (DEBUG == 1)
      Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :("));
#endif

      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
#if (DEBUG == 1)
      Serial.println(F("Ethernet cable is not connected."));
#endif

    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
#if (DEBUG == 1)
    Serial.print(F("  DHCP assigned IP "));
    Serial.println(Ethernet.localIP());
#endif

  }



  webserver.setDefaultCommand(&infoRequest); // дефолтная страница вывода (информация о контроллере)
  webserver.addCommand("command", &parsedRequest); // команды
  webserver.addCommand("state", &stateRequest); // выдать состояния всех устройств
  webserver.addCommand("wash", &washRequest); // Обработка комманд на включение и выключение режимов мойки
  webserver.addCommand("gate", &gateRequest); // Обработка комманд на открытие и закрытие ворот
  webserver.addCommand("light", &lightRequest); // Обработка комманд на включение и выключение света
  webserver.addCommand("help", &helpRequest); // Обработка комманд на включение и выключение света


  webserver.begin();
#if (DEBUG == 1)
  Serial.print( F("server is at ") );
  Serial.println(Ethernet.localIP());
#endif


  char server[] = "www.2.toutlem.z8.ru";

  EthernetClient client;
  // give the Ethernet shield a second to initialize:
  delay(3000);
#if (DEBUG == 1)
  Serial.println(F("connecting..."));
#endif
  // if you get a connection, report back via serial:

  int connCount = 0;
  int statusConn;
  while (connCount < 20) {
    statusConn = client.connect(server, 80);
#if (DEBUG == 1)
    Serial.println(statusConn);
    Serial.print(".");
#endif
    delay(100);

    if (statusConn == 1) {
      break;
#if (DEBUG == 1)
      Serial.println(".");
#endif
    }
#if (DEBUG == 1)
    Serial.print(connCount);
#endif
    connCount++;
  }
  if (statusConn == 1) {
#if (TELEGRAMM == 1)
    sendIpToTlegramm(client);
#endif


  }
  else {
    // kf you didn't get a connection to the server:
#if (DEBUG == 1)
    Serial.println(F("connection failed"));
#endif
  }

  wdt_enable (WDTO_8S); // Для тестов не рекомендуется устанавливать значение менее 8 сек.

  // Настройка портов на вывод
  for (int thisPin = startPin; thisPin <= endPin; thisPin++)  {
    pinMode(thisPin, OUTPUT);
    digitalWrite(thisPin, HIGH);
#if (DEBUG == 1)
    Serial.print(thisPin);
    Serial.print(" - ");
    Serial.println("HIGH");
#endif
  }

  // Настройка 29ого пина для реле
   pinMode(29, OUTPUT);
   digitalWrite(29, LOW);
   Serial.print("29");
   Serial.print(" - ");
    Serial.println("LOW");
  // Настройка портов на вывод
  /*  for (int thisPin = startinPin; thisPin <= endinPin; thisPin++)  {
      pinMode(thisPin, OUTPUT);
      digitalWrite(thisPin, HIGH);
      Serial.println(thisPin);
    }
  */
  /*tat;
    int testPin = 52;
    pinMode(testPin, INPUT);
    stat = digitalRead(testPin);
    Serial.print(testPin);
    Serial.print(" - ");
    Serial.println(stat);
    Serial.print("server Ready ");
  */
  int sensorValue = analogRead(62);
#if (DEBUG == 1)
  Serial.print("A0 - ");
  Serial.println(sensorValue);
#endif
  previousMillis = millis();

}

char* ip2CharArray(IPAddress ip) {
  static char a[16];
  sprintf(a, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  return a;
}
String ipToString(IPAddress ip) {
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
/**********************************************************************************************************************/
void loop() {
  char buff[64];
  int len = 64;
  webserver.processConnection(buff, &len);  // process incoming connections one at a time forever
  delay(200);
  if (millis() - previousMillis > 5000) {
    checkDHT11();
    previousMillis = millis();
  }
  if (millis() - previousMillisBusy1 > 1000) {

    if (busytimer != 0) {
      busy = 1;
      //Пин отвечающий за освещение
      //Включаем свет если состояние бизи
      //Вообще неплохо бы это сделать из настроек...
      //Если не будешь ленивой жопой - прикрути им Епром и сохранение настроек
      pinOn(operatePins[7]);
      busytimer = busytimer - 1;
    } else {
      //выключаем свет если не занят
      if (lightSwitch == 0) {
        pinOff(operatePins[7]);
      }
      busy = 0;
    }
    int i;
    int stvalue;
    for (i = 0; i < 8; i++) {
      stvalue = analogRead(washStatusPins[i]);
      if (stvalue > borderValue) {
        if (i == 2) {
          // Serial.println("1");
          busytimer = 180;
        } else {
          //Serial.println("1");
          busytimer = 60;
        }


      } else {
        //Serial.println("0");
      }
    }
    previousMillisBusy1 = millis();
  }
  wdt_reset();
}

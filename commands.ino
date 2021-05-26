/* Обработчики команд */

void programm(WebServer &server) {

  server.print(gParamValue);

}

void commandsOn(WebServer &server) {
  if (gParamValue >= startPin && gParamValue <= endPin) {
    digitalWrite(gParamValue, LOW);
    if(gParamValue == operatePins[7]){
      lightSwitch =1;
    }
    reloadPage(server);
  }
  else ErrorMessage(server);
}

void pinOn(int pin) {
  if (pin >= startPin && pin <= endPin) {
    digitalWrite(pin, LOW);

  }
}
void commandsOff(WebServer &server) {
  if (gParamValue >= startPin && gParamValue <= endPin) {
    digitalWrite(gParamValue, HIGH);
    if(gParamValue == operatePins[7]){
      lightSwitch = 0;
    }
    reloadPage(server);
  }
  else ErrorMessage(server);
}
void pinOff(int pin) {
  if (pin >= startPin && pin <= endPin) {
    digitalWrite(pin, HIGH);

  }
}
void commandsClick(WebServer &server) {
  if (gParamValue >= startPin && gParamValue <= endPin) {
    digitalWrite(gParamValue, LOW);
    delay(delayClick);
    digitalWrite(gParamValue, HIGH);
    reloadPage(server);
  }
  else ErrorMessage(server);
}

void pinClick(int pin) {
  if (pin >= startPin && pin <= endPin) {
    digitalWrite(pin, LOW);
    delay(delayClick);
    digitalWrite(pin, HIGH);
#if (DEBUG == 1)
    Serial.print("Clicked");
#endif
  } else {
#if (DEBUG == 1)
    Serial.print("WrongPin");
#endif
  }

}
void commandsLClick(WebServer &server) {
  if (gParamValue >= startPin && gParamValue <= endPin) {
    digitalWrite(gParamValue, LOW);
    delay(delayLClick);
    digitalWrite(gParamValue, HIGH);
    reloadPage(server);
  }
  else ErrorMessage(server);
}
void pinLClick(int pin, int longTime) {
  if (pin >= startPin && pin <= endPin) {
    digitalWrite(pin, LOW);
    delay(longTime);
    digitalWrite(pin, HIGH);

  }
  //Serial.print("WrongPin");
}

void commandsGateUp(WebServer &server) {
  //Короче тут надо находить какие пины сейчас отвечают за ворота и кликать или
  //Пины прописать жестко но вот их выбор храним в конфиге.
    digitalWrite(gParamValue, HIGH);

}
void washOperating(WebServer &server) {
  int i;
  int count = sizeof(operatePins) / sizeof(char *);
  for (i = 0; i < count; i++) {
    generateButton(server, operatePins[i], operateStrings[i], operateOnOff[i],inverceLogic[i]);
  }
  server.print("<BR>");
}
void washStatuses(WebServer &server) {
  int i;
  int count = sizeof(washStatusPins) / sizeof(char *);
  for (i = 0; i < count; i++) {
    generateLabel(server, washStatusPins[i], washStatusStrings[i], washStatusOnOff[i]);
  }
  server.print("<BR>");

}


void checkDHT11() {


  humid = dht1.readHumidity();
  temp = dht1.readTemperature();
  humid2 = dht2.readHumidity();
  temp2 = dht2.readTemperature();

  if (isnan(humid) || isnan(temp)) {
#if (DEBUG == 1)
    Serial.println(F("Failed to read from DHT sensor!"));
#endif
    return;
  } else {
    hic = dht1.computeHeatIndex(temp, humid, false);
    hic2 = dht2.computeHeatIndex(temp2, humid2, false);
#if (DEBUGTEMP == 1)
    Serial.print(F("Temp -"));
    Serial.println(temp);
    
    Serial.print(F("humid -"));
    Serial.println(humid);
    
    Serial.print(F("hic -"));
    Serial.println(hic);

    Serial.print(F("Temp2 -"));
    Serial.println(temp2);
    
    Serial.print(F("humid2 -"));
    Serial.println(humid2);
    
    Serial.print(F("hic2 -"));
    Serial.println(hic2);
#endif
  }


}
/**********************************************************************************************************************/

command_t const gCommandTable[COMMAND_TABLE_SIZE] = {
  //  {"LED",     commandsLed, },

  {
    "ON",     commandsOn,
  }
  , // Устанавливает "1" на заданном цифровом порту (вызов http://xx.xx.xx.xx/command?8=ON )
  {
    "programm",     programm,
  }
  , // Устанавливает "1" на заданном цифровом порту (вызов http://xx.xx.xx.xx/command?8=ON )
  {
    "OFF",     commandsOff,
  }
  , // Устанавливает "0" на заданном цифровом порту (вызов http://xx.xx.xx.xx/command?8=OFF )
  {
    "STATUS",     commandsStatus,
  }
  , // Получить состояние цифрового порта (1 или 0) (вызов http://xx.xx.xx.xx/command?8=STATUS ),
  // если вместо номера порта передать ALL (вызов http://xx.xx.xx.xx/command?ALL=STATUS ), то получим состояние всех портов (Пример вывода P3=0;P4=0;P5=0;P6=0;P7=0;P8=1;P9=1;)
  {
    "CLICK",     commandsClick,
  }
  , // Кратковременная "1" на порту 1сек (время настраивается) (вызов http://xx.xx.xx.xx/command?8=CLICK )
  {
    "LCLICK",     commandsLClick,
  }
  , // Кратковременная "1" на порту 3сек (время настраивается) (вызов http://xx.xx.xx.xx/command?8=LCLICK )
  {
    NULL,      NULL
  }
};

/**********************************************************************************************************************

   Function:    cliProcessCommand

   Description: Look up the command in the command table. If the
                command is found, call the command's function. If the
                command is not found, output an error message.

   Notes:

   Returns:     None.

 **********************************************************************/
void cliProcessCommand(WebServer &server)
{
  int bCommandFound = false;
  int idx;

  gParamValue = strtol(gParamBuffer, NULL, 0);  // Convert the parameter to an integer value. If the parameter is empty, gParamValue becomes 0.
  for (idx = 0; gCommandTable[idx].name != NULL; idx++) {  // Search for the command in the command table until it is found or the end of the table is reached. If the command is found, break out of the loop.
    if (strcmp(gCommandTable[idx].name, gCommandBuffer) == 0) {
      bCommandFound = true;
      break;
    }
  }

  if (bCommandFound == true) {  // Если команда найдена (в массиве команд), то выполняем ее. Если нет - игнорируем
    (*gCommandTable[idx].function)(server);
  }
  else { // Command not found
    server.print("ERROR: Command not found");
  }
}



void ErrorMessage(WebServer &server) {
  server.print("ERROR: This Pin is not configured for I/O");
}

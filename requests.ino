/**********************************************************************************************************************
   Разбор запроса
 **/
void parsedRequest(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  int  name_len;
  char value[VALUELEN];
  int value_len;

  server.httpSuccess();  // this line sends the standard "we're all OK" headers back to the browser

  /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;

  if (strlen(url_tail))
  {
    while (strlen(url_tail)) // Разбор URI на составные части (выборка параметров)
    {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS) {
        //      server.printP(Params_end);
      } else // Получили параметр (name) и его значение (value)
      {
        // Выполняем команды
        strcpy (gCommandBuffer, value); // параметры (значение)
        strcpy (gParamBuffer, name); // команда
        cliProcessCommand(server);
      }
    }
  }

  if (type == WebServer::POST)
  {
    server.printP("Post_params_begin");
    while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
    {
      server.print(name);
      server.printP("Parsed_item_separator");
      server.print(value);
      server.printP("Tail_end");
    }
  }


}




void washRequest(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  int  name_len;
  int validRequest = 0;
  char value[VALUELEN];
  int value_len;
  int chousenPin;

  server.httpSuccess();  // this line sends the standard "we're all OK" headers back to the browser

  /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;

  if (strlen(url_tail)) {
    // //Serial.println(url_tail);
    while (strlen(url_tail)) // Разбор URI на составные части (выборка параметров)
    {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      // //Serial.println(rc);
      if (rc == URLPARAM_EOS) {
        //server.printP(Params_end);
      } else // Получили параметр (name) и его значение (value)
      {
        if (strcmp(name, "token") == 0) {
          // Serial.println("Token finded");
          if (strcmp(value, "ElChupacabra") == 0) {
            validRequest = 1;

            // Serial.println("Token Valid");
          } else {
            errorResponse(server, "wrong token");
          }
        }
        if (validRequest == 1) {
          //   Serial.println("validRequest");
          if (strcmp(name, "prog") == 0) {
            chousenPin = operatePins[atoi(value)];
            // Serial.print("chousenPin - ");
            //  Serial.println(chousenPin);

          }
          if (strcmp(name, "action") == 0) {
            // Serial.print("action");
            // Serial.print(" - ");
            // Serial.println(value);
            if (strcmp(value, "start") == 0) {
              if ((chousenPin == operatePins[0]) || (chousenPin == operatePins[1])) {
                pinClick(chousenPin);
                goodResponse(server, "complete");
              }
            } else  if (strcmp(value, "stop") == 0) {
              pinClick(operatePins[3]);
              goodResponse(server, "complete");
            } else  if (strcmp(value, "pause") == 0) {
              pinClick(operatePins[2]);
              goodResponse(server, "complete");
            } else {
              errorResponse(server, "wrong action");
            }
          }
        }
      }
    }
  }
}
/*
  отработка запроса http://10.170.1.180/gate?token=ElChupacabra&direction=down&time=5

*/
void gateRequest(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{

  URLPARAM_RESULT rc;
  char name[NAMELEN];
  int  name_len;
  int validRequest = 0;
  char value[VALUELEN];
  int value_len;
  int chousenPin;

  server.httpSuccess();  // this line sends the standard "we're all OK" headers back to the browser

  /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;

  if (strlen(url_tail)) {
    // Serial.println(url_tail);
    while (strlen(url_tail)) // Разбор URI на составные части (выборка параметров)
    {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS) {
      } else // Получили параметр (name) и его значение (value)
      {
        if (strcmp(name, "token") == 0) {
          // Serial.println("Token finded");
          if (strcmp(value, "ElChupacabra") == 0) {
            validRequest = 1;
            //   Serial.println("Token Valid");
          } else {
            errorResponse(server, "wrong token");
          }
        }
        if (validRequest == 1) {
          if (strcmp(name, "direction") == 0) {
            if (strcmp(value, "up") == 0) {
#if (DEBUG == 1)
              Serial.print("direction is set to - ");
              Serial.println(value);
#endif
              chousenPin = operatePins[5];
              goodResponse(server, "complete");
            } else if (strcmp(value, "down") == 0) {
#if (DEBUG == 1)
              Serial.print("direction is set to - ");
              Serial.println(value);
#endif
              chousenPin = operatePins[6];
              goodResponse(server, "complete");
            } else {
              errorResponse(server, "Wrong direction");

            }
          }
          if (strcmp(name, "time") == 0) {
#if (DEBUG == 1)
            Serial.print("time - ");
#endif
            gateLongClick = atoi(value);
#if (DEBUG == 1)
            Serial.println(gateLongClick);
#endif
            if (gateLongClick > 0) {
              gateLongClick = gateLongClick * 1000;
              pinLClick(chousenPin, gateLongClick);
              goodResponse(server, "complete");
            }

          }
        }
      }
    }
  }
}
void lightRequest(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  URLPARAM_RESULT rc;
  char name[NAMELEN];
  int  name_len;
  int validRequest = 0;
  char value[VALUELEN];
  int value_len;
  int chousenPin;

  server.httpSuccess();  // this line sends the standard "we're all OK" headers back to the browser

  /* if we're handling a GET or POST, we can output our data here.
    For a HEAD request, we just stop after outputting headers. */
  if (type == WebServer::HEAD)
    return;

  if (strlen(url_tail)) {
    // Serial.println(url_tail);
    while (strlen(url_tail)) // Разбор URI на составные части (выборка параметров)
    {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      if (rc == URLPARAM_EOS) {
      } else // Получили параметр (name) и его значение (value)
      {
        if (strcmp(name, "token") == 0) {
          if (strcmp(value, "ElChupacabra") == 0) {
            validRequest = 1;
          } else {
            errorResponse(server, "wrong token");
          }
        }
        if (validRequest == 1) {

          chousenPin = operatePins[7];

          if (strcmp(name, "action") == 0) {
            if (strcmp(value, "on") == 0) {
              pinOn(chousenPin);
              goodResponse(server, "complete");
            } else  if (strcmp(value, "off") == 0) {
              pinOff(chousenPin);
              goodResponse(server, "complete");
            } else {
              errorResponse(server, "wrong action");
            }
          }
        }
      }
    }
  }
}
void stateRequest(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  strcpy (gParamBuffer, "ALL");
  commandsStatus(server);
}
void helpRequest(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  strcpy (gParamBuffer, "ALL");
  commandsHelp(server);
}

/**********************************************************************************************************************
   Генерация и вывод информации об устройстве
 **/
void infoRequest(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{

  server.print(F("<!DOCTYPE html><style>html{background: #cee2e1; /* Old browsers */background: -webkit-linear-gradient(top,  #6ec1e4 0%,#ffffff 100%); background: linear-gradient(to bottom,  #6ec1e4 0%,#ffffff 100%); background-repeat:  no-repeat;background-size:  cover;div.buttons{float: left;}font-family: Verdana,Helvetica,Sans;color: #666;}a{text-decoration: none; color: #666; }.bold{color:  #000;}a:visited{color: #ffffff;}.base{max-width: 900px; margin: 0 auto;}.header{height: 120px;}.logo{float: left;  font-size: 22px;}.menu{float:right;color: #ffffff; margin-top: 56px}.content{border:  #666 solid 1px;border-radius: 0px;padding: 6px;background-color:  #ffffff;}.inset {color: #ffffff;text-shadow: -1px -1px 1px #000, 1px 1px 1px #fff;}ul.hr {margin: 0; padding: 2px; }ul.hr li {display: inline; border-right: 1px solid #000; padding-right: 6px;text-transform:  uppercase;font-weight:  400;}ul.hr li:last-child { border-right: none;}a.knopka {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1.25em; margin-right: 2px; margin-bottom: 10px; outline: none; } a.label {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1em; margin-right: 0px; margin-bottom: 10px; outline: none; } a.knopka:hover { background: rgb(232,95,76); } .blue {background: rgb(56, 162, 212); } .red { background: rgb(212,75,56); } a.knopka:active { background: rgb(152,15,0); } </style><html><head><title>CARWASH360</title><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><div class=\"base\"><div class=\"header\"><div class=\"logo\"><h1 class=\"inset\">CARWASH360</h1></div><div class=\"menu\"><ul class=\"hr inset\"><a href=\"/\"><li>Главная</li></a><a href=\"/help\"><li>Помощь</li></a><a href=\"/contacts\"><li>Контакты</li></a></ul></div></div><div class=\"content\">"));
  server.print(F("IP:"));
  server.print(Ethernet.localIP());
  server.print("<br>Location:");
  server.printP(location_info);


  server.print(F("<h4>Управление и состояние реле.</h4> "));
  server.print(F("<div style=\"width: 100%;height: 50px;\" class=\"buttons\">")); //style=\"float:left;\" style=\"width: 100%;height: 50px;\"
  strcpy (gParamBuffer, "ALL");
  washOperating(server);
  //commandsStatus(server);
  server.print(F("</div>"));

  server.print(F("<h4>Состояние мойки.</h4> "));
  server.print(F("<div style=\"width: 100%;height: 50px;\" class=\"buttons\">")); //style=\"float:left;\"
  washStatuses(server);
  server.print(F("</div>"));
  server.print(F("<h4>Температура и влажность.</h4> "));
  server.print(F("<div style=\"width: 45%;height: 100px; float: left; \" >"));
  server.print("<ul>");
  server.print("<li>");
  server.print(F("Влажность - "));
  server.print(humid);
  server.print("</li>");
  server.print("<li>");
  server.print("Температура - ");
  server.print(temp);
  server.print("</li>");
  server.print("<li>");
  server.print(F("Индекс температуры - "));
  server.print(hic);
  server.print("</li>");
  server.print("</ul>");
  server.print("</div>");

  server.print(F("<div style=\"width: 45%;height: 100px; float: left; \" >"));
  server.print("<ul>");
  server.print("<li>");
  server.print(F("Влажность Внешний - "));
  server.print(humid2);
  server.print("</li>");
  server.print("<li>");
  server.print("Температура Внешний- ");
  server.print(temp2);
  server.print("</li>");
  server.print("<li>");
  server.print(F("Индекс температуры Внешний- "));
  server.print(hic2);
  server.print("</li>");
  server.print("</ul>");
  server.print("</div>");
  server.print(F("<div style=\"width: 100%;height: 10px; clear: both; \" >"));
  server.print(F("<HR>"));
  server.print(F("</div>"));


  server.print(F("<hr><br>Version info: "));
  server.printP(version_info);
  server.printP(footer);

}

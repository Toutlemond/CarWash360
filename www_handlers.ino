void reloadPage(WebServer &server) {
  server.print(F("<HTML><HEAD>"));
  server.print(F(" <META HTTP-EQUIV=\"REFRESH\" CONTENT=\"1; URL=http://"));
  server.print(Ethernet.localIP());
  server.print(F("\">   "));
  server.print(F("</HEAD><BODY></BODY></HTML>"));
}
void generateButton(WebServer &server, int i, String text, int myOnOff, int inverseLogic) {

  int st = digitalRead(i);
  String colorblue = "";
  String colorred = "";
  if (inverseLogic == 0) {
    colorblue = "red";
    colorred = "blue";
  } else {
    colorblue = "blue";
    colorred = "red";
  }
  if (st == 1) {
    if (myOnOff == 1) {
      server.print(F("<a href=\"/command?"));
      server.print(i);
      server.print(F("=ON \" class=\"knopka "));
      server.print(colorblue);
      server.print(F(" \">"));
    }
    else {
      server.print(F("<a href=\"/command?"));
      server.print(i);
      server.print(F("=LCLICK \" class=\"knopka "));
      server.print(colorblue);
      server.print(F(" \">"));
    }
  }
  else {
    if (myOnOff == 1) {
      server.print(F("<a href=\"/command?"));
      server.print(i);
      server.print(F("=OFF \" class=\"knopka "));
      server.print(colorred);
      server.print(F(" \">"));
    }
    else {
      server.print(F("<a href=\"/command?"));
      server.print(i);
      server.print(F("=CLICK \" class=\"knopka "));
      server.print(colorred);
      server.print(F(" \">"));
    }

  }

  server.print(text);
  server.print("</a>");
}
void generateLabel(WebServer &server, int i, String text, int myOnOff) {

  int sensorValue = analogRead(i);
  char my_st[5];
  itoa(sensorValue, my_st, 10);
  if (sensorValue > borderValue) {
    server.print(F("<a href=\"/command?"));
    server.print(i);
    server.print(F("=LCLICK \" class=\"label red \">"));
  }
  else {
    server.print(F("<a href=\"/command?"));
    server.print(i);
    server.print(F("=CLICK \" class=\"label blue  \">"));
  }
  //Serial.print(i);
  //Serial.print("=");
  //Serial.print(my_st);

  server.print(text);
  server.print(F("="));
  server.print(my_st);
  server.print(F("</a>"));
}
void commandsStatus(WebServer &server) {
  server.print(F("{"));
  server.print(F(" \"statuses\": {"));
  int i;
  int stvalue;

  server.print(F("\""));
  server.print(F("busy"));
  server.print(F("\""));
  server.print(F(": "));
  server.print(busy);
  server.print(F(","));
  server.print(F("\""));
  server.print(F("busytimer"));
  server.print(F("\""));
  server.print(F(": "));
  server.print(busytimer);
  server.print(F(","));
  for (i = 0; i < 8; i++) {
    server.print("\"");
    server.print(washStatusEngStrings[i]);
    server.print("\"");
    server.print(": ");
    stvalue = analogRead(washStatusPins[i]);
    if (stvalue > borderValue) {
      server.print("1");
    } else {
      server.print("0");
    }
    server.print(",");
  }
  server.print(F("}"));
  server.print(F(" \"environment\": {"));

  server.print(F("\"temp\": "));
  server.print(temp);
  server.print(F(","));
  server.print(F("\"humid\": "));
  server.print(temp);
  server.print(F(","));
  server.print(F("\"hic\": "));
  server.print(hic);
  server.print(F(","));
  server.print(F("\"gas\": "));
  server.print(F("0"));
  server.print(F(","));
  server.print(F("}"));
  server.print(F("}"));
}
void commandSetupForm(WebServer &server) {
  server.printP(header);
  server.print(F("<form method=\"POST\" action=\"/setup\">\n"));
  server.print(F("<h4>Настройки контроллера</h4>\n"));
  server.print(F("<hr>\n"));

  //server.print(F("<p>Новый ip адрес терминала: <span class=\"bold\"><input style=\"width: 22px;\" type=\"text\" name=\"srvip1\">.\n"));
  //server.print(F("<input style=\"width: 22px;\" type=\"text\" name=\"srvip2\">.\n"));
  //server.print(F("<input style=\"width: 22px;\" type=\"text\" name=\"srvip3\">.\n"));
  //server.print(F("<input style=\"width: 22px;\" type=\"text\" name=\"srvip4\"></span></p>\n"));
  //server.print(F("</span></p>\n"));

  server.print(F("<input hidden style=\"width: 120px;\" value =\"ElChupacabra\" type=\"text\" name=\"token\">\n"));

  server.print(F("<p>Название контроллера 10 символов lat -\n"));
  server.print(F("<input style=\"width: 120px;\" value =\""));
  server.print(cname);
  server.print(F("\" type=\"text\" name=\"cname\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Время короткого BusyTimer (сек) - \n"));
  server.print(F("<input style=\"width: 120px;\" value =\""));
  server.print(bts);
  server.print(F("\" type=\"text\" name=\"bts\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Время длинного BusyTimer (сек) -  \n"));
  server.print(F("<input style=\"width: 120px;\" value =\""));
  server.print(btl);
  server.print(F("\" type=\"text\" name=\"btl\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Пороговое значение Логической единицы -   \n"));
  server.print(F("<input style=\"width: 120px;\" value =\""));
  server.print(lo);
  server.print(F("\" type=\"text\" name=\"lo\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Влажность включения вытяжки (%) -   \n"));
  server.print(F("<input style=\"width: 120px;\" value =\""));
  server.print(hon);
  server.print(F("\" type=\"text\" name=\"hon\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Температура закрытия ворот (гр.С.) - \n"));
  server.print(F("<input style=\"width: 120px;\" value =\""));
  server.print(gt);
  server.print(F("\" type=\"text\" name=\"gt\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Режим работы ворот/шлакбаума -  \n"));
  server.print(F("<select name=\"gatetype\" style=\"width: 220px;\" >"));
  server.print(F("<option value=\"1\">Ворота</option>\n"));
  server.print(F("<option value=\"2\">Шлакбаум </option>\n"));
  server.print(F("<option value=\"3\">Ворота и шлакбаум</option>\n"));
  server.print(F("</select >"));

  server.print(F("</p>\n"));


  server.print(F("<p>Отправка события старт на телеграм -   \n"));
  server.print(F("<input "));
  if (telerst == 1) {
    server.print(F(" CHECKED "));
  }
  server.print(F(" style=\"width: 120px;\" value =\"1\" type=\"checkbox\" name=\"telerst\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Отправка закрытия ворот по температуре  -   \n"));
  server.print(F("<input "));
  if (telegt == 1) {
    server.print(F(" CHECKED "));
  }
  server.print(F(" style=\"width: 120px;\" value =\"1\" type=\"checkbox\" name=\"telegt\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Отправка включения вытяжки по влажности  -   \n"));
  server.print(F("<input "));
  if (telehumon == 1) {
    server.print(F(" CHECKED "));
  }
  server.print(F(" style=\"width: 120px;\" value =\"1\" type=\"checkbox\" name=\"telehumon\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Отправка начала мойки  -   \n"));
  server.print(F("<input "));
  if (telestrt == 1) {
    server.print(F(" CHECKED "));
  }
  server.print(F(" style=\"width: 120px;\" value =\"1\" type=\"checkbox\" name=\"telestrt\">\n"));
  server.print(F("</p>\n"));

  server.print(F("<p>Отправка окончания мойки  -   \n"));
  server.print(F("<input "));
  if (telestop == 1) {
    server.print(F(" CHECKED "));
  }
  server.print(F(" style=\"width: 120px;\" value =\"1\" type=\"checkbox\" name=\"telestop\">\n"));
  server.print(F("</p>\n"));

  // Далее завершение формы
  server.print(F("<input type=\"submit\" value=\"Сохранить\">\n"));
  server.print(F("</form>\n"));
  server.printP(version_info);
  server.printP(footer);

}
void commandsHelp(WebServer &server) {
  //server.print("<html><head><title>CARWASH360</title><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head>\n");

  server.print(F("<!DOCTYPE html><style>html{background: #cee2e1; /* Old browsers */background: -webkit-linear-gradient(top,  #6ec1e4 0%,#ffffff 100%); background: linear-gradient(to bottom,  #6ec1e4 0%,#ffffff 100%); background-repeat:  no-repeat;background-size:  cover;div.buttons{float: left;}font-family: Verdana,Helvetica,Sans;color: #666;}a{text-decoration: none; color: #666; }.bold{color:  #000;}a:visited{color: #ffffff;}.base{max-width: 900px; margin: 0 auto;}.header{height: 120px;}.logo{float: left;  font-size: 22px;}.menu{float:right;color: #ffffff; margin-top: 56px}.content{border:  #666 solid 1px;border-radius: 0px;padding: 6px;background-color:  #ffffff;}.inset {color: #ffffff;text-shadow: -1px -1px 1px #000, 1px 1px 1px #fff;}ul.hr {margin: 0; padding: 2px; }ul.hr li {display: inline; border-right: 1px solid #000; padding-right: 6px;text-transform:  uppercase;font-weight:  400;}ul.hr li:last-child { border-right: none;}a.knopka {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1.5em; margin-right: 10px; margin-bottom: 10px; outline: none; } a.label {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1em; margin-right: 3px; margin-bottom: 10px; outline: none; } a.knopka:hover { background: rgb(232,95,76); } .blue {background: rgb(56, 162, 212); } .red { background: rgb(212,75,56); } a.knopka:active { background: rgb(152,15,0); } </style><html><head><title>CARWASH360</title><link rel=\"icon\" href=\"http://carwash360.ru/wp-content/uploads/2019/10/cropped-favicon-32x32.png\" sizes=\"32x32\"><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><div class=\"base\"><div class=\"header\"><div class=\"logo\"><h1 class=\"inset\">CARWASH360</h1></div><div class=\"menu\"><ul class=\"hr inset\"><a href=\"/\"><li>Главная</li></a><a href=\"/help\"><li>Помощь</li></a><a href=\"/setup\"><li>Настройки</li></a></ul></div></div><div class=\"content\">"));
  server.print(F("<h2>"));
  server.print(F("Памятка по get командам"));
  server.print(F("</h2>"));
  server.print(F("<HR>"));
  server.print(F("<ul>"));
  //http://10.170.1.180/wash?token=ElChupacabra&prog=1&action=pause
  server.print(F("<h3>"));
  server.print(F("Wash"));
  server.print(F("</h3>"));
  server.print(F("<h4>"));
  server.print(F("http://host/wash?token=*********&prog=1&action=pause"));
  server.print(F("</h4>"));
  server.print(F("<p>"));
  server.print(F("params:"));
  server.print(F("</p>"));

  server.print(F("<li>"));
  server.print(F("token - ключ для работы"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("prog - программа мойки"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("0 - мойка номер 1"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("1 - мойка номер 2"));
  server.print(F("</li>"));
  server.print(F("</ul>"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("action - Действие с программой мойки"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("start - запуск режима мойки. Работает только если передан режим в свойстве prog"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("stop - остановка работающего режима"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("pause - пауза работающего режима"));
  server.print(F("</li>"));
  server.print(F("</ul>"));
  server.print(F("</li>"));
  server.print(F("</ul>"));

  server.print(F("<HR>"));
  server.print(F("<ul>"));
  //http://10.170.1.180/gate?token=ElChupacabra&direction=down&time=5
  server.print(F("<h3>"));
  server.print(F("Gate"));
  server.print(F("</h3>"));
  server.print(F("<h4>"));
  server.print(F("http://gate?token=****&direction=down&time=5"));
  server.print(F("</h4>"));
  server.print(F("<p>"));
  server.print(F("params:"));
  server.print(F("</p>"));

  server.print(F("<li>"));
  server.print(F("token - ключ для работы"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("direction - Направление движения ворот"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("up - открытие ворот"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("down - закрытие ворот"));
  server.print(F("</li>"));
  server.print(F("</ul>"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("time - Время движения ворот в секундах"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("например 5 - пять секунд движения- нужно подобрать эмпирическим путем"));
  server.print(F("</li>"));
  server.print(F("</ul>"));
  server.print(F("</li>"));
  server.print(F("</ul>"));

  server.print(F("<HR>"));
  server.print(F("<ul>"));
  //http://10.170.1.180/light?token=ElChupacabra&action=off
  server.print(F("<h3>"));
  server.print(F("Light"));
  server.print(F("</h3>"));
  server.print(F("<h4>"));
  server.print(F("http://host/light?token=****&action=on"));
  server.print(F("</h4>"));
  server.print(F("<p>"));
  server.print(F("params:"));
  server.print(F("</p>"));

  server.print(F("<li>"));
  server.print(F("token - ключ для работы"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("action - программа мойки"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("on - Включить свет"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("off - Выключить свет"));
  server.print(F("</li>"));
  server.print(F("</ul>"));
  server.print(F("</li>"));
  server.print(F("</ul>"));

  server.print(F("<HR>"));
  server.print(F("<ul>"));
  //http://10.170.1.180/state
  server.print(F("<h3>"));
  server.print(F("State"));
  server.print(F("</h3>"));
  server.print(F("<h4>"));
  server.print(F("http://host/state"));
  server.print(F("</h4>"));
  server.print(F("<p>"));
  server.print(F("Выдает состояние мойки на данный момент в формате JSON. Параметров нет"));
  server.print(F("</p>"));
  server.print(F("</ul>"));

  server.print(F("<HR>"));
  server.print(F("<ul>"));
  //http://10.170.1.180/help
  server.print(F("<h3>"));
  server.print(F("Help"));
  server.print(F("</h3>"));
  server.print(F("<h4>"));
  server.print(F("http://host/help"));
  server.print(F("</h4>"));
  server.print(F("<p>"));
  server.print(F("Вывод данной справки"));
  server.print(F("</p>"));
  server.print(F("</ul>"));
  // server.print("</html>");
  server.print(F("<hr><br>Version info: "));
  server.printP(version_info);
  server.printP(footer);
}
void goodResponse(WebServer &server, String error) {
  server.print(F("{"));
  server.print(F(" \"response\": "));
  server.print(error);
  server.print(F("}"));
}
void errorResponse(WebServer &server, String error) {
  server.print(F("{"));
  server.print(F(" \"error\": "));
  server.print(error);
  server.print(F("}"));
}

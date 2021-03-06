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
void commandsHelp(WebServer &server) {
  //server.print("<html><head><title>CARWASH360</title><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head>\n");

  server.print(F("<!DOCTYPE html><style>html{background: #cee2e1; /* Old browsers */background: -webkit-linear-gradient(top,  #6ec1e4 0%,#ffffff 100%); background: linear-gradient(to bottom,  #6ec1e4 0%,#ffffff 100%); background-repeat:  no-repeat;background-size:  cover;div.buttons{float: left;}font-family: Verdana,Helvetica,Sans;color: #666;}a{text-decoration: none; color: #666; }.bold{color:  #000;}a:visited{color: #ffffff;}.base{max-width: 900px; margin: 0 auto;}.header{height: 120px;}.logo{float: left;  font-size: 22px;}.menu{float:right;color: #ffffff; margin-top: 56px}.content{border:  #666 solid 1px;border-radius: 0px;padding: 6px;background-color:  #ffffff;}.inset {color: #ffffff;text-shadow: -1px -1px 1px #000, 1px 1px 1px #fff;}ul.hr {margin: 0; padding: 2px; }ul.hr li {display: inline; border-right: 1px solid #000; padding-right: 6px;text-transform:  uppercase;font-weight:  400;}ul.hr li:last-child { border-right: none;}a.knopka {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1.5em; margin-right: 10px; margin-bottom: 10px; outline: none; } a.label {float: left;color: #fff;text-decoration: none; user-select: none; padding: .7em 1em; margin-right: 3px; margin-bottom: 10px; outline: none; } a.knopka:hover { background: rgb(232,95,76); } .blue {background: rgb(56, 162, 212); } .red { background: rgb(212,75,56); } a.knopka:active { background: rgb(152,15,0); } </style><html><head><title>CARWASH360</title><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body><div class=\"base\"><div class=\"header\"><div class=\"logo\"><h1 class=\"inset\">CARWASH360</h1></div><div class=\"menu\"><ul class=\"hr inset\"><a href=\"/\"><li>??????????????</li></a><a href=\"/help\"><li>????????????</li></a><a href=\"/contacts\"><li>????????????????</li></a></ul></div></div><div class=\"content\">"));
  server.print(F("<h2>"));
  server.print(F("?????????????? ???? get ????????????????"));
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
  server.print(F("token - ???????? ?????? ????????????"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("prog - ?????????????????? ??????????"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("0 - ?????????? ?????????? 1"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("1 - ?????????? ?????????? 2"));
  server.print(F("</li>"));
  server.print(F("</ul>"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("action - ???????????????? ?? ???????????????????? ??????????"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("start - ???????????? ???????????? ??????????. ???????????????? ???????????? ???????? ?????????????? ?????????? ?? ???????????????? prog"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("stop - ?????????????????? ?????????????????????? ????????????"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("pause - ?????????? ?????????????????????? ????????????"));
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
  server.print(F("token - ???????? ?????? ????????????"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("direction - ?????????????????????? ???????????????? ??????????"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("up - ???????????????? ??????????"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("down - ???????????????? ??????????"));
  server.print(F("</li>"));
  server.print(F("</ul>"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("time - ?????????? ???????????????? ?????????? ?? ????????????????"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("???????????????? 5 - ???????? ???????????? ????????????????- ?????????? ?????????????????? ???????????????????????? ??????????"));
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
  server.print(F("token - ???????? ?????? ????????????"));
  server.print(F("</li>"));

  server.print(F("<li>"));
  server.print(F("action - ?????????????????? ??????????"));
  server.print(F("<ul>"));
  server.print(F("<li>"));
  server.print(F("on - ???????????????? ????????"));
  server.print(F("</li>"));
  server.print(F("<li>"));
  server.print(F("off - ?????????????????? ????????"));
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
  server.print(F("???????????? ?????????????????? ?????????? ???? ???????????? ???????????? ?? ?????????????? JSON. ???????????????????? ??????"));
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
  server.print(F("?????????? ???????????? ??????????????"));
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

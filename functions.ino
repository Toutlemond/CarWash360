void sendIpToTlegramm() {
EthernetClient client = getClient();
  char charBufVar[20];
  cname.toCharArray(charBufVar, 20);
  char myStr1[200] ;
  //Ethernet.localIP();
  char myStr10[] = "GET /scripts/SendToTelegram.php?site=CarWash360&token=1020424408:AAExCPMpCBu1pyPekhLx0GKOhV9Vli6SI-0&chat_id=-112519786&message=";
  char myStr12[] = "%20is%20rebooted!%20New%20ip:";
  sprintf(myStr1, "%s%s%s", myStr10, charBufVar, myStr12);
  char myStr2[16];


  String ipadd;

  ipadd = ipToString(Ethernet.localIP());
  ipadd.toCharArray(myStr2, 16);
#if (DEBUG == 1)
  Serial.println("ip ");
  Serial.println(myStr2);
#endif
  char myStr3[] = " HTTP/1.1";
  char myStr4[200] ;
  sprintf(myStr4, "%s%s%s", myStr1, myStr2, myStr3);
#if (DEBUG == 1)
  Serial.println("connected");
  Serial.println(myStr4);
#endif
  //http://www.2.toutlem.z8.ru/scripts/SendToTelegram.php?site=CarWash360&token=1020424408:AAExCPMpCBu1pyPekhLx0GKOhV9Vli6SI-0&chat_id=-112519786&message=New%20controller%20ip:10.170.1.119
  //client.println("GET /scripts/SendToTelegram.php?site=CarWash360&token=1020424408:AAExCPMpCBu1pyPekhLx0GKOhV9Vli6SI-0&chat_id=-112519786&message=New%20controller%20ip:10.170.1.122 HTTP/1.1");
  client.println(myStr4);
  client.println(F("Host: www.2.toutlem.z8.ru"));
  client.println(F("Connection: close"));
  client.println();

}

void sendMsgToTlegramm(String message) {
EthernetClient client = getClient();
  char myStr1[] = "GET /scripts/SendToTelegram.php?site=CarWash360&token=1020424408:AAExCPMpCBu1pyPekhLx0GKOhV9Vli6SI-0&chat_id=-112519786&message=";
  char myStr3[] = " HTTP/1.1";
  char myStr4[200] ;

  char charBufVar[50];
  message.toCharArray(charBufVar, 50);

  sprintf(myStr4, "%s%s%s", myStr1, charBufVar, myStr3);
#if (DEBUG == 1)
  Serial.println("connected");
  Serial.println(myStr4);
#endif
  //http://www.2.toutlem.z8.ru/scripts/SendToTelegram.php?site=CarWash360&token=1020424408:AAExCPMpCBu1pyPekhLx0GKOhV9Vli6SI-0&chat_id=-112519786&message=New%20controller%20ip:10.170.1.119
  //client.println("GET /scripts/SendToTelegram.php?site=CarWash360&token=1020424408:AAExCPMpCBu1pyPekhLx0GKOhV9Vli6SI-0&chat_id=-112519786&message=New%20controller%20ip:10.170.1.122 HTTP/1.1");
  client.println(myStr4);
  client.println(F("Host: www.2.toutlem.z8.ru"));
  client.println(F("Connection: close"));
  client.println();

}


EthernetClient getClient() {


  char server[] = "www.2.toutlem.z8.ru";

  EthernetClient client;
  // give the Ethernet shield a second to initialize:
  delay(100);
#if (DEBUG == 1)
  Serial.print(F("connecting"));
#endif
  // if you get a connection, report back via serial:

  int connCount = 0;
  int statusConn;
  while (connCount < 10) {
    statusConn = client.connect(server, 80);
#if (DEBUG == 1)
    Serial.print(".");
#endif
    delay(100);
    if (statusConn == 1) {
      break;
#if (DEBUG == 1)
      Serial.println(".");
#endif
    }
    connCount++;
  }
  if (statusConn == 1) {
    return client;
  }
  else {
    // kf you didn't get a connection to the server:
#if (DEBUG == 1)
    Serial.println(F("connection failed"));
#endif
     return client;
  }
}
void CheckHumid() {


  if (humid2 >= hon) {
    //Там обратная логика... а мне так лень сейчас это расписывать
    pinOff(29);
    if (humidOnMsgWasSended == 0) {
      humidOnMsgWasSended = 1;
      humidOffMsgWasSended = 0;
      sendMsgToTlegramm("Ventilation%20turnON");
    }

  } else {
    pinOn(29);
    if (humidOffMsgWasSended == 0) {
      humidOffMsgWasSended = 1;
      humidOnMsgWasSended = 0;
      sendMsgToTlegramm("Ventilation%20turnOFF");
    }

  }
}
void CheckGateTempHumid() {
  if (temp2 <= gt) {
    //Там обратная логика... а мне так лень сейчас это расписывать
    pinOff(29);
  } else {
    pinOn(29);
  }
}

void sendToTlegramm(EthernetClient &client){
  }

  void sendIpToTlegramm(EthernetClient &client){
    
    char myStr1[] = "GET /scripts/SendToTelegram.php?site=CarWash360&token=1020424408:AAExCPMpCBu1pyPekhLx0GKOhV9Vli6SI-0&chat_id=-112519786&message=Controller%20V-2.0%20is%20rebooted!%20New%20ip:";
    //Ethernet.localIP();

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

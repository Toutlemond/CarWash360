String stringEpromRead (int EStart, int Estop) {

  String stringFromEprom;
  for (int i = EStart; i < Estop; ++i)
  {
    char ch = char(EEPROM.read(i));
    if (ch != 0 ) {
      stringFromEprom += ch;
    }
  }
  return stringFromEprom;
}
void stringEpromWrite(String contName, int EStart, int Estop) {
  if (contName.length() > 0) {

    for (int i = EStart; i < Estop; ++i) {
      EEPROM.write(i, 0);
    }
#if (DEBUG == 1)
    Serial.println(contName);
    Serial.println("");
    Serial.println("writing eeprom Controller Name:");
#endif
    for (int i = 0; i < contName.length(); ++i)
    {
      EEPROM.write(EStart + i, contName[i]);
#if (DEBUG == 1)
      Serial.print("Wrote: ");
      Serial.println(contName[i]);
#endif
    }
  }
}
void updateSettingFromEprom() {
  EEPROM.get(100, bts);
#if (DEBUG == 1)
  Serial.print("bts - ");
  Serial.println(bts);
#endif
  EEPROM.get(102, btl);
#if (DEBUG == 1)
  Serial.print("btl - ");
  Serial.println(btl);
#endif
  EEPROM.get(104, lo);
#if (DEBUG == 1)
  Serial.print("los - ");
  Serial.println(lo);
#endif
  EEPROM.get(106, hon);
#if (DEBUG == 1)
  Serial.print("hon - ");
  Serial.println(hon);
#endif
  EEPROM.get(108, gt);
#if (DEBUG == 1)
  Serial.print("gt - ");
  Serial.println(gt);
#endif
  EEPROM.get(110, telerst);
#if (DEBUG == 1)
  Serial.print("telerst - ");
  Serial.println(telerst);
#endif
  EEPROM.get(112, telegt);
#if (DEBUG == 1)
  Serial.print("telegt - ");
  Serial.println(telegt);
#endif
  EEPROM.get(114, telehumon);
#if (DEBUG == 1)
  Serial.print("telehumon - ");
  Serial.println(telehumon);
#endif
  EEPROM.get(116, telestrt);
#if (DEBUG == 1)
  Serial.print("telestrt - ");
  Serial.println(telestrt);
#endif
  EEPROM.get(118, telestop);
#if (DEBUG == 1)
  Serial.print("telestop - ");
  Serial.println(telestop);
#endif
  cname = stringEpromRead(150, 169);
#if (DEBUG == 1)
  Serial.print("cname - ");
  Serial.println(cname);
#endif
}
void alleeprom() {
  // Перебор всего епрома.
  byte eevalue;
  for (int ee = 0; ee <= 512; ee++) {
    eevalue = EEPROM.read(ee);

    Serial.print(ee);
    Serial.print("\t");
    Serial.print(eevalue, DEC);
    Serial.println();
  }
}

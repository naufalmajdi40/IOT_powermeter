void initConfig() {
  String JsonConfig = readFileStr("/config.json");
  DeserializationError error = deserializeJson(doc2, JsonConfig);
  int baud = doc2["baud"];
  String sercfg = doc2["serial"];
  uint32_t SerialConf = serialConfigFromString(sercfg.c_str());

  //Serial.println(SerialConf);
//  Serial2.begin(baud, SerialConf);
    Serial2.begin(baud, SerialConf);
  
  int md, interval;
  //String ip,nmsk,dns,gtw,mqtt_server,mqtt_name,mqtt_pass;
  int modecfg = doc2["mode"];
  eventTime2 = doc2["interval"];
  md = modecfg;
  ids = doc2["id"],HEX;
  String kd = doc2["kode"];
  String ipcfg = doc2["ip"];
  String gtwcfg = doc2["gtw"];
  String nmskcfg = doc2["netmask"];
  String dnscfg = doc2["dns"];
  String mqttServer = doc2["mqttServer"];
  String mqttName = doc2["mqttName"];
  String mqttPass = doc2["mqttPass"];
  type = doc2["type"];
  int wifiEncfg = doc2["wifiEn"];
  wifiEn = wifiEncfg;
  if (wifiEncfg == 1) {
    Serial.print("connect WIFI");
    //WiFi.mode(WIFI_AP); //Optional
     WiFi.softAP(ssid, password);
  }
  uint16_t index = millis() % NUMBER_OF_MAC;
  ETH.begin( MISO_GPIO, MOSI_GPIO, SCK_GPIO, CS_GPIO, INT_GPIO, SPI_CLOCK_MHZ, ETH_SPI_HOST, mac[index] );
  kode = kd;
  ip = ipcfg;
  nmsk = nmskcfg;
  gtw = gtwcfg;
  nmsk = nmskcfg;
  dns = dnscfg;
  mqtt_server = mqttServer;
  mqtt_name = mqttName;
  mqtt_pass = mqttPass;
  if (type == 0) {
    byte perintah2[3][6] = {
      {ids, 0x03, 0x40, 0x00, 0x00, 0x40},
      {ids, 0x03, 0x40, 0x00, 0x00, 0x40},
      {ids, 0x03, 0x40, 0x00, 0x00, 0x40}
    };
    for (int i = 0 ; i < 3; i++) {
      for (int j = 0; j < 6; j++) {
        perintah[i][j] = perintah2[i][j];
      }
    }
  }
  else if (type == 1) {
    byte perintah2[3][6] = {
      {ids, 0x03, 0x0B, 0xB7, 0x00, 0x40},
      {ids, 0x03, 0x0C, 0x05, 0x00, 0x08},
      {ids, 0x03, 0x0C, 0x25, 0x00, 0x2}
    };
    for (int i = 0 ; i < 3; i++) {
      for (int j = 0; j < 6; j++) {
        perintah[i][j] = perintah2[i][j];
      }
    }
  }


  String *IP = split(ipcfg, '.');
  IPAddress myIP(IP[1].toInt(), IP[2].toInt(), IP[3].toInt(), IP[4].toInt());
  delay(1);
  String *GTW = split(gtwcfg, '.');
  IPAddress myGW(GTW[1].toInt(), GTW[2].toInt(), GTW[3].toInt(), GTW[4].toInt());
  delay(1);
  String *NMS = split(nmskcfg, '.');
  IPAddress mySN(NMS[1].toInt(), NMS[2].toInt(), NMS[3].toInt(), NMS[4].toInt());
  delay(1);
  String *DNS = split(dnscfg, '.');
  IPAddress myDNS(DNS[1].toInt(), DNS[2].toInt(), DNS[3].toInt(), DNS[4].toInt());
  delay(1);
  String *MQTTS = split(mqttServer, '.');
  IPAddress server2(MQTTS[1].toInt(), MQTTS[2].toInt(), MQTTS[3].toInt(), MQTTS[4].toInt());
  delay(1);
  //IPAddress server2(203, 194, 112, 238);
  client.setServer(server2, 1883);
  if (modecfg == 1) {
    ETH.config(myIP, myGW, mySN, myDNS);
  }

  //Serial.println(cfgmode);
  /////////////////////
  client.setCallback(callback);
  ESP32_W5500_onEvent();
  server.onNotFound(handleNotFound);
  //server.begin();
  delay(1000);
}
uint32_t serialConfigFromString(const char* configs) {
  if (0 == strcmp(configs, "SERIAL_5N1")) {
    return SERIAL_5N1;
  } else if (0 == strcmp(configs, "SERIAL_6N1")) {
    return SERIAL_6N1;
  } else if (0 == strcmp(configs, "SERIAL_7N1")) {
    return SERIAL_7N1;
  } else if (0 == strcmp(configs, "SERIAL_8N1")) {
    return SERIAL_8N1;
  }

  else if (0 == strcmp(configs, "SERIAL_5N2")) {
    return SERIAL_5N2;
  } else if (0 == strcmp(configs, "SERIAL_6N2")) {
    return SERIAL_6N2;
  } else if (0 == strcmp(configs, "SERIAL_7N2")) {
    return SERIAL_7N2;
  } else if (0 == strcmp(configs, "SERIAL_8N2")) {
    return SERIAL_8N2;
  }

  else if (0 == strcmp(configs, "SERIAL_5E1")) {
    return SERIAL_5E1;
  } else if (0 == strcmp(configs, "SERIAL_6E1")) {
    return SERIAL_6E1;
  } else if (0 == strcmp(configs, "SERIAL_7E1")) {
    return SERIAL_7E1;
  } else if (0 == strcmp(configs, "SERIAL_8E1")) {
    return SERIAL_8E1;
  }

  else if (0 == strcmp(configs, "SERIAL_5E2")) {
    return SERIAL_5E2;
  } else if (0 == strcmp(configs, "SERIAL_6E2")) {
    return SERIAL_6E2;
  } else if (0 == strcmp(configs, "SERIAL_7E2")) {
    return SERIAL_7E2;
  } else if (0 == strcmp(configs, "SERIAL_8E2")) {
    return SERIAL_8E2;
  }
  else if (0 == strcmp(configs, "SERIAL_5O1")) {
    return SERIAL_5O1;
  } else if (0 == strcmp(configs, "SERIAL_6O1")) {
    return SERIAL_6O1;
  } else if (0 == strcmp(configs, "SERIAL_7O1")) {
    return SERIAL_7O1;
  } else if (0 == strcmp(configs, "SERIAL_8O1")) {
    return SERIAL_8O1;
  }

  else if (0 == strcmp(configs, "SERIAL_5O2")) {
    return SERIAL_5O2;
  } else if (0 == strcmp(configs, "SERIAL_6O2")) {
    return SERIAL_6O2;
  } else if (0 == strcmp(configs, "SERIAL_7O2")) {
    return SERIAL_7O2;
  } else if (0 == strcmp(configs, "SERIAL_8O2")) {
    return SERIAL_8O2;
  }
  else  {
    return SERIAL_8N1;
  }
}

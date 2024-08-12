void handleNotFound(AsyncWebServerRequest *request)
{
  String message = "File Not Found\n\n";

  message += "URI: ";
  //message += server.uri();
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->args();
  message += "\n";

  for (uint8_t i = 0; i < request->args(); i++)
  {
    message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
  }

  request->send(404, "text/plain", message);
}
String getrandomid() {
  char    randomId[9];
  for (int i = 0; i < 8; i++) {
    randomId[i] = alphanum[random(50)];
  }
  randomId[7] = '\0';
  return (randomId);
}
String *split(String data, char separate) {
  // int pnjg = data.length();
  static String dtp[70] ;
  String dataparsing = "@";
  dataparsing += String(separate);
  dataparsing += data;
  dataparsing += '#';
  int j = 0;
  int i;
  for (i = 1; i < dataparsing.length(); i++) {
    if ((dataparsing[i] == separate) || (dataparsing[i] == '#')) {
      j++;
      dtp[j] = "";
    }
    else {
      dtp[j] = dtp[j] + dataparsing[i];
    }
  }
  return dtp;
}


void readSerialData() {
  String dtSerial = Serial.readString();
  if (dtSerial.indexOf("AT") > -1) {
    String *SerParse = split(dtSerial, ' ');
    //   Serial.println(SerParse[2]);
    if (SerParse[2].indexOf("GETIP") > -1) {
      Serial.println(ETH.localIP());
    }
    else if (SerParse[2].indexOf("GETDNS") > -1) {
      Serial.println(dns);
    }
    else if (SerParse[2].indexOf("GETGTW") > -1) {
      Serial.println(gtw);
    }
    else if (SerParse[2].indexOf("GETNTMSK") > -1) {
      Serial.println(nmsk);
    }

    else if (SerParse[2].indexOf("CFGIP") > -1) {
      if ((sizeof(SerParse[3]) > 0) && (SerParse[4].indexOf("OK") > -1)) {
        Serial.print("Old IP:");
        Serial.print(ip);
        Serial.print(" New IP:");
        Serial.println(SerParse[3]);
        ip = (SerParse[3]);
      }
    }
    else if (SerParse[2].indexOf("ENWIFI") > -1) {
      if ((sizeof(SerParse[3]) > 0) && (SerParse[4].indexOf("OK") > -1)) {
        Serial.print("WiFi status:");
        Serial.print(wifiEn);
        Serial.print(" New WiFi status:");
        Serial.println(SerParse[3]);
        wifiEn = (SerParse[3]).toInt();
        
      }
    }
    else if (SerParse[2].indexOf("CFGDNS") > -1) {
      if ((sizeof(SerParse[3]) > 0) && (SerParse[4].indexOf("OK") > -1)) {
        Serial.print("Old DNS:");
        Serial.print(dns);
        Serial.print(" New DNS:");
        Serial.println(SerParse[3]);
        dns = (SerParse[3]);
      }
    }
    else if (SerParse[2].indexOf("CFGGTW") > -1) {
      if ((sizeof(SerParse[3]) > 0) && (SerParse[4].indexOf("OK") > -1)) {
        Serial.print("Old GATEWAY:");
        Serial.print(gtw);
        Serial.print(" New GATEWAY:");
        Serial.println(SerParse[3]);
        gtw = (SerParse[3]);
      }
    }
    else if (SerParse[2].indexOf("CFGNMSK") > -1) {
      if ((sizeof(SerParse[3]) > 0) && (SerParse[4].indexOf("OK") > -1)) {
        Serial.print("Old NETMASK:");
        Serial.print(nmsk);
        Serial.print(" New NETMASK:");
        Serial.println(SerParse[3]);
        nmsk = (SerParse[3]);
      }
    }
    else if (SerParse[2].indexOf("CFGMODE") > -1) {
      if ((sizeof(SerParse[3]) > 0) && (SerParse[4].indexOf("OK") > -1)) {
        Serial.print("Old MODE:");
        Serial.print(md);
        Serial.print(" New MODE:");
        Serial.println(SerParse[3]);
        md = (SerParse[3]).toInt();
      }
    }
    else if (SerParse[2].indexOf("SAVE") > -1) {
      saveConfig();
    }

    else {
      Serial.println("OK");
    }


  }
}
void resetData(){
  for(int i=0;i<1;i++){
    for(int j =0;j<50;j++){
      dtReal[i][j]=0;
      }
    }
  }

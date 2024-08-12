void sendModbus() {
  uint16_t crc = calcCRC(perintah[flag], sizeof(perintah[flag]));
  Serial2.write(perintah[flag], sizeof(perintah[flag]));
  Serial2.write(lowByte(crc));
  Serial2.write(highByte(crc));
  memcmp(bufferDataModbus, perintah[flag], 2);
  ptr = bufferDataModbus;
  //ptr[2]=>jumlah data modbus
  int n = 3;
  if (type == 1) {
    if (ptr[2] == 128) {
      for (int i = 0; i < 40; i++) {
        dtReal[0][i] = getDataModbus(ptr[n], ptr[n + 1], ptr[n + 2], ptr[n + 3]);
        n += 4;
      }
      countdt[0]++;
    }
    else if (ptr[2] == 16) {
      for (int i = 0; i < 4; i++) {
        dtReal[1][i] = getDataModbus(ptr[n], ptr[n + 1], ptr[n + 2], ptr[n + 3]);
        n += 4;
      }
      countdt[0]++;
    }
    else if (ptr[2] == 4) {
      ;
      for (int i = 0; i < 8; i++) {
        dtReal[2][i] = getDataModbus(ptr[n], ptr[n + 1], ptr[n + 2], ptr[n + 3]);
        n += 4;
      }
    countdt[0]++;
    }
    saveSensorToJSONDM6000H();
    //resetData();
  }
  else if (type == 0) {
    if (ptr[2] > 39) {
      for (int i = 0; i < 32; i++ ) {
        if (i < 26) {
          dtReal[0][i] = getDataModbus(ptr[n], ptr[n + 1], ptr[n + 2], ptr[n + 3]);
          n += 4;
        }
        else {
          dtReal[0][i] = getDataModbus(ptr[n], ptr[n + 1], 0, 0);
          delay(1);
          n += 4;
        }
        countdt[0]++;
      }
    
    saveSensorToJSON();
    //resetData();
    }
  }

  memset(bufferDataModbus, 0x00, sizeof(bufferDataModbus));
  flag++;
  if (flag > 2) {
    flag = 0;
  }
}
uint16_t calcCRC(byte *data, byte panjang)
{
  int i;
  uint16_t crc = 0xFFFF;
  for (byte p = 0; p < panjang; p++)
  {
    crc ^= data[p];
    for (i = 0; i < 8; ++i)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc = (crc >> 1);
    }
  }
  return crc;
}
double getDataModbus(int a, int b, int c, int d)
{ char s[100];
  String dt = String(String(a, HEX));
  dt += String(String(b, HEX));
  dt += String(String(c, HEX));
  dt += String(String(d, HEX));
  long int result = 0;
  dt.toUpperCase();
  //Serial.println(dt);
  if (dt.length() < 8) {
    for (int i = dt.length(); i < 8; i++)
      dt += "0";
  }

  for (int i = 0; i < dt.length(); i++) {
    result <<= 4;
    result |= hexDigits.indexOf(dt[i]);
  }
  dtostrf(ConvertB32ToFloat(result), 1, 9, s);
  //double res = strtod(s, NULL);
  dt = "";
  return  strtod(s, NULL);
}

void saveSensorToJSON() {
  doc["kode"] = kode;
  doc["type"] = type;
  doc["F"] = dtReal[0][0];
  doc["VRN"] = dtReal[0][1];
  doc["VSN"] = dtReal[0][2];
  doc["VTN"] = dtReal[0][3];
  doc["VAVG"] = dtReal[0][4];
  doc["VRS"] = dtReal[0][5];
  doc["VST"] = dtReal[0][6];
  doc["VRT"] = dtReal[0][7];
  doc["IR"] = dtReal[0][9];
  doc["IS"] = dtReal[0][10];
  doc["IT"] = dtReal[0][11];
  //doc["IAVG"] = dtReal[0][12];
  doc["IN"] = dtReal[0][13];
  doc["PFR"] = dtReal[0][26];
  doc["PFS"] = dtReal[0][27];
  doc["PFT"] = dtReal[0][28];
  doc["PFAVG"] = dtReal[0][17];
}
void saveSensorToJSONDM6000H() {
  doc["kode"] = kode;
  doc["type"] = type;
  doc["F"] = dtReal[2][0], 3;
  doc["VRN"] = dtReal[0][14];
  doc["VSN"] = dtReal[0][15];
  doc["VTN"] = dtReal[0][16];
  doc["VAVG"] = dtReal[0][18];
  doc["VRS"] = dtReal[0][10];
  doc["VST"] = dtReal[0][11];
  doc["VRT"] = dtReal[0][12];
  doc["IR"] = dtReal[0][0];
  doc["IS"] = dtReal[0][1];
  doc["IT"] = dtReal[0][2];
  // doc["IAVG"] = dtReal[0][12];
  doc["IN"] = dtReal[0][3];
  doc["PFR"] = getPfVal(dtReal[1][0]);
  doc["PFS"] = getPfVal(dtReal[1][1]);
  doc["PFT"] = getPfVal(dtReal[1][2]);
  doc["PFAVG"] = getPfVal(dtReal[1][3]);
}

void saveConfig() {
  doc2["ip"] = ip;
  doc2["gtw"] = gtw;
  doc2["netmask"] = nmsk;
  doc2["dns"] = dns;
  doc2["mode"] = md;
  doc2["wifiEn"] = wifiEn;
  doc2["type"] = type;
  String out;
  serializeJson(doc2, out);
  Serial.println(out);
  writefileSTR("/config.json", out);
  Serial.println("Device is Restart");
  restart = 1;
}
float getPfVal(float x) {

  if (x > 1) {
    return 2 - x;
  }
  else if (x < -1) {
    return -2 - x;
  }
  else if ( abs(x) == 1) {
    return x;

  }
  else {
    return x;
  }
}

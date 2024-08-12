String readFileStr(String str) {
  String dataFs;
  File file = SPIFFS.open(str, "r");
  Serial.println(file);
  if (!file) {
    Serial.println("Failed to open file for reading");
  }
  while (file.available()) {
    dataFs += (char)file.read();
    delayMicroseconds(100);
  }
  file.close();
  return dataFs;
}
void writefileSTR(String s, String y) {
  File file = SPIFFS.open(s, "w");
  if (!file) {
    Serial.println("Error opening file for writing");
    return;
  }
  int bytesWritten = file.print(y);
}

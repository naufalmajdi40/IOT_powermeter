void initWebServer() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    // Serial.println("ok");
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(401);
  });
  server.on("/logout-web", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/logout-web.html", String(), false, processor);

  });
  server.on("/index", HTTP_GET, [](AsyncWebServerRequest * request) {
    // Serial.println("ok");
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/live", HTTP_GET, [](AsyncWebServerRequest * request) {
    // Serial.println("ok");
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/live.html", String(), false, processor);

  });
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/JsonConfig", HTTP_GET, [](AsyncWebServerRequest * request) {
    String dtJson2;
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    serializeJson(doc2,  dtJson2);
    request->send(200, "application/json", dtJson2);
  });
  
  server.on("/JsonData", HTTP_GET, [](AsyncWebServerRequest * request) {
    String dtJson;
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    serializeJson(doc,  dtJson);
    request->send(200, "application/json", dtJson);
  });
  server.on("/saveConfig", HTTP_POST, [](AsyncWebServerRequest * request) {
    int params = request->params();
    //if (!request->authenticate(http_username, http_password))
    //return request->requestAuthentication();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      Serial.println(p->value().length());
      if (p->value().length() > 0) {
        doc2[p->name().c_str()] = p->value().c_str();
      }
    }
    String output;
    serializeJson(doc2, output);
    Serial.println(output);
    writefileSTR("/config.json", output);
    request->send(SPIFFS, "/saved.html", String(), false, processor);
    restart = 1;
  });


  // Start server
  server.begin();
}

void connectWiFi()
{
  int count = 0;
  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);
  Serial.println(mySSID);
  WiFi.begin(mySSID, myPW);

  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(mySSID, myPW);
    delay(500);
    Serial.print(".");
    count++;
    if (count > 15) ESP.restart();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

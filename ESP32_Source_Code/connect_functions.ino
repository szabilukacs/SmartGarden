void connectWiFi()
{
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.print("connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void send_data_to_website()
{
  // POST request
  String postData = "in_temp=" + String(in_temp) + "&out_temp=" + String(out_temp) + "&in_hum=" + String(in_hum) + "&out_hum=" + String(out_hum) + "&in_heat_index=" + String(in_heat_index) + "&out_heat_index=" + String(out_heat_index) + "&ligth=" + String(lux) + "&rain=" + String(rain_value) + "&window_is_open=" + String(door_open);

  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.POST(postData);
  String payload = http.getString();

  // Print data
  Serial.print("URL : ");
  Serial.println(serverUrl);
  Serial.print("Data: ");
  Serial.println(postData);
  Serial.print("httpCode: ");
  Serial.println(httpCode);
  Serial.print("payload : ");
  Serial.println(payload);
  Serial.println("--------------------------------------------------");

  Serial.println("humidity : ");
  Serial.println(in_hum);
  // Serial.println("Humidity temp: "); Serial.println(temphum);
  Serial.println("Heat index: ");
  Serial.println(in_heat_index);

  Serial.println("humidity : ");
  Serial.println(out_hum);
  // Serial.println("Humidity temp: "); Serial.println(temphum);
  Serial.println("Heat index: ");
  Serial.println(out_heat_index);

  Serial.println("Rain value: ");
  Serial.println(rain_value);

  Serial.println("lux value: ");
  Serial.println(lux);

  // End POSTData
}

// Measure temperatures inside and outside
void measure_temperature()
{
  // Temperature
  sensors.begin();                            // Start up the library
  numberOfDevices = sensors.getDeviceCount(); // Grab a count of devices on the wire

  if ((error_counter < 3) && (numberOfDevices == 2))
  {
    // Found device 0 with address: 28D61396F0013CE6
    // Found device 1 with address: 2845E15704E13CFB obsolete

    print_temperatures_devices_found();
    // Temperatures
    sensors.requestTemperatures(); // Send the command to get temperatures

    // TODO: ezeket meg tisztazni, hogy cserelhetnek e helyet
    in_temp = sensors.getTempCByIndex(1);
    out_temp = sensors.getTempCByIndex(0);
    print_temperatures();
  }
  else
  {
    error_counter++;
    measure_temperature();
  }

  // End Temperatures
}

// Measure humidity
void measure_humidity()
{
  dht_in.begin(); // Humidity sensor begin
  dht_out.begin();
  // Start humidity
  in_hum = dht_in.readHumidity();
  out_hum = dht_out.readHumidity();

  // Compute heat index in Celsius (isFahreheit = false)
  in_heat_index = dht_in.computeHeatIndex(in_temp, in_hum, false);
  out_heat_index = dht_out.computeHeatIndex(out_temp, out_hum, false);
  // End humidity
}

void measure_rain()
{
  rain_value = RAIN_OFFSET - analogRead(AN_PIN_RAIN); // mapping 0-1024
}

int measure_ligth()
{
  float lux = analogRead(AN_PIN_LIGTH) * 5 / 1024.0; // Convert reading to VOLTS
  Serial.println("lux value: ");
  Serial.println(lux);
  // lux = lux / 10000.0;
  Serial.println("lux value: ");
  Serial.println(lux);
  lux = lux * 100 * 2.0;
  Serial.println("lux value: ");
  Serial.println(lux);
  return lux;
}

bool measure_door()
{
  int doorState = digitalRead(D_PIN_DOOR);

  if (doorState == HIGH)
    door_open = true;
  else
    door_open = false;

  return door_open;
}

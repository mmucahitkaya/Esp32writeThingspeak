#include <WiFi.h>
#include <HTTPClient.h>
#include <Turta_TempRH_Module.h>




Turta_TempRH_Module th;

String apiKey = "Thingspeak Write API Key";
const char* ssid = "Your WiFi SSID";
const char* password = "Your WiFi Password";
const char* server = "api.thingspeak.com";



void setup() {
  // put your setup code here, to run once:
  // Initialize Temperature & RH Sensor
  th.begin();

  // Initialize Serial Port
  Serial.begin(115200);
  delay(100);

  // Configure Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Print connection data
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected.");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  // Read temperature
  double temp = th.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(temp);

  // Read relative humidity
  double humidity = th.readHumidity();
  Serial.print("Humidity: ");
  Serial.println(humidity);

  
  Serial.println("Prepare to send data");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(server, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = apiKey;
    data_to_send += "&field1=";
    data_to_send += String(temp);
    data_to_send += "&field2=";
    data_to_send += String(humidity);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(1000);
  }

  client.stop();


  delay(1000);

}

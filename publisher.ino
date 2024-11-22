#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>

#define DHTPIN 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// WiFi
const char *ssid = "POCO X3 NFC";
const char *password = "wzdig263";
// MQTT Broker
const char *mqtt_broker = "mqtt.eclipseprojects.io";
const char *topic = "prueba001";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());

    Serial.printf("The client %s connects to mosquitto mqtt broker\n", client_id.c_str());

    if (client.connect(client_id.c_str())) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  dht.begin();
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");

  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }

  Serial.println();
  Serial.println(" - - - - - - - - - - - -");
}

void loop() {
  client.loop();  // Se encarga de mantener activa la conexión con el broker
  
  delay(2000);  // Delay de 2 segundos

  float h = dht.readHumidity();   // Lectura de humedad
  float t = dht.readTemperature();  // Lectura de temperatura

  if (isnan(h) || isnan(t)) { // Verificación de lectura correcta
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Impresión de valores en el monitor serial
  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.print(F("% Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));

  // Publicación de valores en el broker
  String msg = String(h) + " " + String(t);
  char char_array[msg.length()];
  msg.toCharArray(char_array, msg.length());
  
  client.publish(topic, char_array);
}
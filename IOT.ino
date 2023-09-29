#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


char ssid[] = "Fatec111";
char password[] = "123654789";
char server[] = "https://iot-phi.vercel.app";
int serverPort = 8080;

const char* http_site = "iot-phi.vercel.app";
const int http_port = 443;
const char* http_path = "/cadastrar"; 


DHTesp dht;


void setup() {
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando no WiFi...");
  }

  Serial.println("WiFi conectado!");

  dht.setup(3, DHTesp::DHT11);
}

void loop() {
  WiFiClientSecure client;
  client.setInsecure();

  delay(dht.getMinimumSamplingPeriod());

  float umidade = dht.getHumidity();
  float temperatura = dht.getTemperature();

  Serial.print("Umidade: ");
  Serial.print(umidade, 1);
  Serial.print("%\tTemperatura: ");
  Serial.print(temperatura, 1);
  Serial.println("°C");

  String url = "https://" + String(http_site) + http_path + "/" + String(temperatura) + "/" + String(umidade);

  Serial.println("fazendo request");
  Serial.println(url);

  if (!client.connect(http_site, http_port)) {
    Serial.println("Falha na conexão com o servidor");
    delay(5000);
    return;
  }

   client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + http_site + "\r\n" + "Connection: close\r\n\r\n");
 
    client.stop();

  delay(5000);  //teste

  //delay(36000); produção
}

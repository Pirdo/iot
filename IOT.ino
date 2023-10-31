#include <DHTesp.h>
#include <ESP8266WiFi.h>


char ssid[] = "Fatec111";
char password[] = "123456789";

const char* http_site = "iot-phi.vercel.app";
const int http_port = 443;
const char* http_path = "/cadastrar"; 


DHTesp dht;

static bool b = false;

void setup() {
  Serial.begin(115200);
  Serial.println();
  int wifiCounter = 0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && wifiCounter < 20) {
    delay(1000);
    Serial.println("Conectando no WiFi...");
    wifiCounter++;
  }

  if(wifiCounter >= 20) {
    Serial.println("Falha ao conectar o Wifi...");
    b = true;
  } else {
    Serial.println("WiFi conectado!");
    dht.setup(3, DHTesp::DHT11);
  }

}

void loop() {
  WiFiClientSecure client;
  if (b) {
    return;
  }
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

  Serial.println("Fazendo request!");

  if (!client.connect(http_site, http_port)) {
    Serial.println("Falha na conexão com o servidor");
    delay(5000);
    return;
  }

   client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + http_site + "\r\n" + "Connection: close\r\n\r\n");
 
    client.stop();

    delay(60000);
}

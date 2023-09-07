#include <DHTesp.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

char ssid[] = "Pires";
char password[] = "familia";

IPAddress server_addr(localhost);
char user[] = "root";
char password_db[] = "";
char db_name[] = "IOT_SENSOR";

DHTesp dht;
WiFiClient client;
MySQL_Connection conn((Client *)&client);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando no WiFi...");
  }
  
  Serial.println("WiFi conectado!");

  dht.setup(3, DHTesp::DHT11);
  
  Serial.println("Conectando ao banco de dados");
  if (conn.connect(server_addr, 3306, user, password_db)) {
    Serial.println("Banco de dados conectado");
  } else {
    Serial.println("Erro na conexão ao banco de dados");
    while (1);
  }
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print("Humidity: ");
  Serial.print(humidity, 1);
  Serial.print("%\tTemperature: ");
  Serial.print(temperature, 1);
  Serial.println("°C");
  
  char query[128];
  sprintf(query, "INSERT INTO DADOS (humidade, temperatura) VALUES (%.1f, %.1f)", humidity, temperature);
  Serial.println(query);
  
  MySQL_Cursor * cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(query);
  delete cur_mem;

  delay(5000);
}

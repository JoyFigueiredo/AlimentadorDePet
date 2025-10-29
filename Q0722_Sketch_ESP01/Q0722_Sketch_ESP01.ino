#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// ----------------------------------------------------------
// Configuração de IP estático
// ----------------------------------------------------------
IPAddress local_IP(192, 168, 2, 166);  // IP fixo desejado para o ESP
IPAddress gateway(192, 168, 1, 1);     // gateway do roteador
IPAddress subnet(255, 255, 255, 0);    // máscara de sub-rede

// ----------------------------------------------------------
// Servidor HTTP
// ----------------------------------------------------------
ESP8266WebServer server(8090);

// LED azul (integrado no ESP-12 / NodeMCU)
#define LED_WIFI 2   // GPIO2 (LED azul, ativo em nível baixo)
unsigned long ultimoPisca = 0;
bool estadoLed = false;

// ----------------------------------------------------------
// NTP Client
// ----------------------------------------------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600);  // GMT-3

// ----------------------------------------------------------
// Variável de controle: Arduino solicitou hora?
// ----------------------------------------------------------
bool pedirHora = false;

// ----------------------------------------------------------
// Função para inicializar Wi-Fi usando WiFiManager
// ----------------------------------------------------------
void initWiFi() {
  Serial.println("Resetando credenciais Wi-Fi antigas...");
  WiFi.disconnect(true);  // Esquece redes antigas
  delay(1000);

  // Configura IP fixo
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falha ao configurar IP fixo");
  }

  WiFiManager wifiManager;
  Serial.println("Iniciando Wi-FiManager...");

  // Cria AP para configurar Wi-Fi
  wifiManager.setConfigPortalTimeout(180); // opcional: timeout de 3 minutos
  if (!wifiManager.autoConnect("AutoAlimentador-AP", "03121995")) {
      Serial.println("Falha ao conectar. Reinicie o ESP");
      delay(3000);
      ESP.restart();
  }

  Serial.print("IP Local: ");
  Serial.println(WiFi.localIP());
}

// ----------------------------------------------------------
// Rota para alimentar
// Ex: /alimentar?voltas=0.25
// ----------------------------------------------------------
void handleAlimentar() {
  if (!server.hasArg("voltas")) {
    server.send(400, "text/plain", "Erro: parametro 'voltas' ausente");
    return;
  }

  String voltas = server.arg("voltas");
  Serial.println("ALIMENTAR:" + voltas);  // envia para Arduino
  server.send(200, "text/plain", "OK: Alimentando " + voltas + " voltas");
}

// ----------------------------------------------------------
// Rota padrão
// ----------------------------------------------------------
void handleNotFound() {
  server.send(404, "text/plain", "Rota nao encontrada");
}

// ----------------------------------------------------------
// Obtem hora e configura para enviar ao arduino
// ----------------------------------------------------------
String obterHora() {
  time_t now = time(nullptr);
  struct tm *p_tm = localtime(&now);

  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
  return String(buffer);
}

// ----------------------------------------------------------
// Varias requisiçoes de alimentar, generico
// ----------------------------------------------------------
void alimentar(float voltas) {
  Serial.print("ALIMENTAR:");
  Serial.println(voltas);
  // Aqui você pode enviar para o Arduino via Serial se quiser:
  Serial.println("ALIMENTAR:" + String(voltas));
}

// ----------------------------------------------------------
// Pisca LED quando conectado ao Wi-Fi
// ----------------------------------------------------------
void piscaLedWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    unsigned long agora = millis();
    if (agora - ultimoPisca > 500) {  // alterna a cada 0,5s
      estadoLed = !estadoLed;
      digitalWrite(LED_WIFI, estadoLed ? LOW : HIGH); // LED invertido
      ultimoPisca = agora;
    }
  } else {
    digitalWrite(LED_WIFI, HIGH); // LED apagado se desconectado
  }
}

// ----------------------------------------------------------
// Setup
// ----------------------------------------------------------
void setup() {
  Serial.begin(9600);
   pinMode(LED_WIFI, OUTPUT);
  digitalWrite(LED_WIFI, HIGH);  // LED apagado

  // Inicializa Wi-Fi
  initWiFi();

  // Inicializa NTP
  timeClient.begin();

  // Define rotas HTTP
  server.on("/alimentar", handleAlimentar);
  server.onNotFound(handleNotFound);

  server.onNotFound([]() {
    String url = server.uri();  // captura a URL que foi chamada
    if (url.startsWith("/alimentar/")) {
      String valorStr = url.substring(11);  // pega o que vem depois de /alimentar/
      float voltas = valorStr.toFloat();
      alimentar(voltas);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(404, "text/plain", "Rota não encontrada");
    }
  });

  server.begin();
  Serial.println("Servidor HTTP iniciado!");
}

// ----------------------------------------------------------
// Loop principal
// ----------------------------------------------------------
void loop() {
  server.handleClient();  // processa requisições HTTP
  piscaLedWifi();         // LED pisca se conectado


  // Recebe comandos do Arduino via Serial
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "PEDIR_HORA") {
      String hora = obterHora();
      Serial.println("HORA:" + hora);  // envia para o Arduino
    }
  }

  // Envia hora NTP somente quando solicitado
  if (pedirHora) {
    timeClient.update();
    String hora = timeClient.getFormattedTime();  // "HH:MM:SS"
    Serial.println("HORA:" + hora);               // envia para Arduino
    pedirHora = false;
  }
}

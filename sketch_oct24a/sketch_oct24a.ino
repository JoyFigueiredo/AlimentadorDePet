#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>   
#include <WiFiClient.h> 

ESP8266WebServer server(80);

// ===========================
// HARDWARE CONFIG
// ===========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Botões
#define BTN_LEFT D5
#define BTN_RIGHT D6
#define BTN_SELECT D7
#define BTN_BACK D4

// Motor de passo
#define STEP D3
#define DIR D8
const int PASSOS_POR_VOLTA = 800; // 1/4 de passo

// ===========================
// VARIÁVEIS DE MENU
// ===========================
String opcoes[] = {"Alimentar", "Horario"};
const int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);

String subAlimentar[5];
int totalSubAlimentar = 0;

float personalizado = -1.0;
float girosPersonalizados = 1.0;

int opcaoAtual = 0;
int subOpcaoAtual = 0;
bool noSubmenu = false;
bool noPersonalizar = false;
bool mostrandoHorario = false;
bool mostrandoMenu = true;

unsigned long ultimaAcao = 0;
const unsigned long steps_base = 200;

// ===========================
// WIFI E HORA (NTP)
// ===========================
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800, 60000); // UTC-3 (Brasil)

// Variável global para armazenar o horário atual
String horaAtual = "";

/*---------------------------------------------------------------------------------------------
*                                      FUNÇÕES DE MENU
---------------------------------------------------------------------------------------------*/
void atualizarSubmenu()
{
  int idx = 0;
  subAlimentar[idx++] = "1/4 volta";
  subAlimentar[idx++] = "2/4 volta";
  subAlimentar[idx++] = "1 volta completa";
  if (personalizado > 0)
    subAlimentar[idx++] = String(personalizado, 1) + " voltas";
  subAlimentar[idx++] = "Personalizar";
  totalSubAlimentar = idx;
}

void mostrarMenu()
{
  lcd.clear();
  int par = (opcaoAtual / 2) * 2;
  for (int i = 0; i < 2; i++)
  {
    int idx = par + i;
    if (idx >= totalOpcoes)
      break;
    lcd.setCursor(0, i);
    lcd.print(idx == opcaoAtual ? "> " : "  ");
    lcd.print(opcoes[idx]);
  }
}

void mostrarSubmenu()
{
  lcd.clear();
  int par = (subOpcaoAtual / 2) * 2;
  for (int i = 0; i < 2; i++)
  {
    int idx = par + i;
    if (idx >= totalSubAlimentar)
      break;
    lcd.setCursor(0, i);
    lcd.print(idx == subOpcaoAtual ? "> " : "  ");
    lcd.print(subAlimentar[idx]);
  }
}

void mostrarMenuPersonalizar()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Giros: ");
  lcd.print(girosPersonalizados, 1);
  lcd.setCursor(0, 1);
  lcd.print("< Confirmar >");
}

/*---------------------------------------------------------------------------------------------
*                                      FUNÇÕES HORA (CORRIGIDO)
---------------------------------------------------------------------------------------------*/

// Atualiza e exibe a hora apenas quando solicitado no menu
void mostrarHoraAtual()
{
  yield();
  timeClient.update();
  yield();
  horaAtual = timeClient.getFormattedTime();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hora atual:");
  lcd.setCursor(3, 1);
  lcd.print(horaAtual);
}

/*---------------------------------------------------------------------------------------------
*                                      FUNÇÕES MOTOR
---------------------------------------------------------------------------------------------*/

void motorExecuta(int passos)
{
  digitalWrite(DIR, HIGH);
  for (int i = 0; i < passos; i++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(8000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(8000);
    yield();
  }
  digitalWrite(STEP, LOW);
}

void motorVoltas(float voltas) {
  int passos = voltas * PASSOS_POR_VOLTA;
  motorExecuta(passos);
}


void executarSubOpcao(float voltas)
{
  // Apenas para uso com botões (LCD ativo)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alimentando...");
  lcd.setCursor(0, 1);
  lcd.print(voltas, 2);

  motorVoltas(voltas);

  enviarHistoricoBackend(voltas);

  delay(1000);
  noSubmenu = false;
  mostrandoMenu = true;
  mostrarMenu();
}

/*---------------------------------------------------------------------------------------------
*                                      Historico para Backend
---------------------------------------------------------------------------------------------*/
void enviarHistoricoBackend(float voltas)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi não conectado. Histórico não enviado.");
    return;
  }

  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://192.168.2.114:9090/historico");
  http.addHeader("Content-Type", "application/json");

  // O backend espera "qnt", mas você enviava "quantidade" !!!
  // Corrigido abaixo:
  String json = "{\"qnt\": \"" + String(voltas, 2) + "\"}";

  int httpResponseCode = http.POST(json);

  Serial.print("Envio ao backend: ");
  Serial.println(httpResponseCode);

  http.end();
}


/*---------------------------------------------------------------------------------------------
*                                      SETUP
---------------------------------------------------------------------------------------------*/
void setup()
{
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  // Configura botões
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  // Configura motor
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(STEP, LOW);

  lcd.setCursor(3, 0);
  lcd.print("Conectando...");
  delay(1000);

  // -------------------------
  // WiFi com IP fixo
  // -------------------------
  IPAddress local_IP(192, 168, 2, 117);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  // Inicia WiFiManager
  WiFiManager wm;
  WiFi.config(local_IP, gateway, subnet);
  if (!wm.autoConnect("AutoPet", "03121995"))
  {
    lcd.clear();
    lcd.print("Falha WiFi!");
    delay(3000);
    ESP.restart();
  }

  lcd.clear();
  lcd.print("WiFi conectado!");
  delay(1000);

  // Inicia NTP
  timeClient.begin();

  atualizarSubmenu();
  mostrarMenu();

  /*----------------------------------------
   * INICIA SERVIDOR HTTP
   *----------------------------------------*/
  server.on("/alimentar", []()
            {
    String quantidade = server.arg("quantidade");
    Serial.println("Quantidade recebida: " + quantidade);

    // Habilita CORS
    server.sendHeader("Access-Control-Allow-Origin", "*");

    float voltas = quantidade.toFloat();
    motorVoltas(voltas);  // reutiliza a função original

    server.send(200, "text/plain", "Alimentado: " + quantidade); });

  server.on("/hora", []()
            {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    timeClient.update();
    String hora = timeClient.getFormattedTime();
    server.send(200, "application/json", "{\"hora\":\"" + hora + "\"}"); });

  // Página para reconfigurar Wi-Fi
  server.on("/wifi", []()
            {
    String html = "<!DOCTYPE html>"
                  "<html lang='pt-BR'>"
                  "<head><meta charset='UTF-8'><title>Configuração Wi-Fi</title></head>"
                  "<body style='font-family:sans-serif;text-align:center;margin-top:50px;'>"
                  "<h2>Configuração Wi-Fi</h2>"
                  "<p>Rede atual: "
                  + WiFi.SSID() + "</p>"
                                  "<button onclick=\"location.href='/reconfigurar'\" "
                                  "style='padding:10px 20px;font-size:16px;'>Trocar Wi-Fi</button>"
                                  "</body></html>";
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", html); });

  server.on("/reconfigurar", []()
            {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", "<!DOCTYPE html><html lang='pt-BR'><meta charset='UTF-8'>"
                                  "<body style='font-family:sans-serif;text-align:center;margin-top:50px;'>"
                                  "<h3>Reiniciando para reconfigurar Wi-Fi...</h3>"
                                  "</body></html>");
    delay(1000);
    WiFiManager wm;
    wm.resetSettings();  // Apaga a rede salva
    ESP.restart(); });

  server.begin();
  Serial.println("Servidor HTTP iniciado!");
}

/*---------------------------------------------------------------------------------------------
*                                      LOOP
---------------------------------------------------------------------------------------------*/

void loop()
{
  server.handleClient(); // processa requisições HTTP
  unsigned long agora = millis();

  // ==========================
  // Controle de menu via botões
  // ==========================
  if ((agora - ultimaAcao) > steps_base)
  {

    // --- SE ESTIVER MOSTRANDO HORA ---
    if (mostrandoHorario)
    {
      // Permite voltar com o botão BACK
      if (!digitalRead(BTN_BACK))
      {
        mostrandoHorario = false;
        mostrarMenu();
        ultimaAcao = agora;
      }
      return; // ignora o resto do menu enquanto mostra hora
    }

    // --- MODO PERSONALIZAR ---
    if (noPersonalizar)
    {
      if (!digitalRead(BTN_LEFT))
      {
        girosPersonalizados -= 0.1;
        if (girosPersonalizados < 0.1)
          girosPersonalizados = 0.1;
        mostrarMenuPersonalizar();
        ultimaAcao = agora;
      }
      if (!digitalRead(BTN_RIGHT))
      {
        girosPersonalizados += 0.1;
        if (girosPersonalizados > 10.0)
          girosPersonalizados = 10.0;
        mostrarMenuPersonalizar();
        ultimaAcao = agora;
      }
      if (!digitalRead(BTN_SELECT))
      {
        personalizado = girosPersonalizados;
        atualizarSubmenu();
        noPersonalizar = false;
        mostrarSubmenu();
        ultimaAcao = agora;
      }
      if (!digitalRead(BTN_BACK))
      {
        noPersonalizar = false;
        mostrarSubmenu();
        ultimaAcao = agora;
      }
      return;
    }

    // --- NAVEGAÇÃO ESQUERDA ---
    if (!digitalRead(BTN_LEFT))
    {
      if (noSubmenu)
      {
        subOpcaoAtual--;
        if (subOpcaoAtual < 0)
          subOpcaoAtual = totalSubAlimentar - 1;
        mostrarSubmenu();
      }
      else
      {
        opcaoAtual--;
        if (opcaoAtual < 0)
          opcaoAtual = totalOpcoes - 1;
        mostrarMenu();
      }
      ultimaAcao = agora;
    }

    // --- NAVEGAÇÃO DIREITA ---
    if (!digitalRead(BTN_RIGHT))
    {
      if (noSubmenu)
      {
        subOpcaoAtual++;
        if (subOpcaoAtual >= totalSubAlimentar)
          subOpcaoAtual = 0;
        mostrarSubmenu();
      }
      else
      {
        opcaoAtual++;
        if (opcaoAtual >= totalOpcoes)
          opcaoAtual = 0;
        mostrarMenu();
      }
      ultimaAcao = agora;
    }

    // --- SELECIONAR ---
    if (!digitalRead(BTN_SELECT))
    {
      if (noSubmenu)
      {
        atualizarSubmenu();
        if (subAlimentar[subOpcaoAtual] == "Personalizar")
        {
          noPersonalizar = true;
          mostrarMenuPersonalizar();
        }
        else
        {
          float voltas = 0;
          if (subOpcaoAtual == 0)
            voltas = 0.25;
          else if (subOpcaoAtual == 1)
            voltas = 0.5;
          else if (subOpcaoAtual == 2)
            voltas = 1.0;
          else if (subAlimentar[subOpcaoAtual].indexOf("voltas") != -1)
            voltas = personalizado;

          executarSubOpcao(voltas);
        }
      }
      else
      {
        if (opcoes[opcaoAtual] == "Alimentar")
        {
          noSubmenu = true;
          subOpcaoAtual = 0;
          atualizarSubmenu();
          mostrarSubmenu();
        }
        else if (opcoes[opcaoAtual] == "Horario")
        {
          mostrandoHorario = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Obtendo hora...");
          delay(500);
          mostrarHoraAtual();
          yield();
        }
      }
      ultimaAcao = agora;
    }

    // Botão BACK
    if (!digitalRead(BTN_BACK))
    {
      if (noSubmenu)
      {
        noSubmenu = false;
        mostrarMenu();
      }
      else if (noPersonalizar)
      {
        noPersonalizar = false;
        mostrarSubmenu();
      }
      ultimaAcao = agora;
    }
  }
}

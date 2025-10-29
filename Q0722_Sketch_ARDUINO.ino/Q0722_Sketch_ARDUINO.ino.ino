#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*---------------------------------------------------------------------------------------------
*                                      VARIÁVEIS
---------------------------------------------------------------------------------------------*/

// LCD 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Botões
#define BTN_LEFT 2
#define BTN_RIGHT 3
#define BTN_SELECT 4
#define BTN_BACK 5

// Motor de passo
#define STEP 6
#define DIR 7
const int PASSOS_POR_VOLTA = 800; // 1/4 step (200 passos × 4)

// Menu principal
String opcoes[] = {"Alimentar", "Horario"};
const int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);

// Submenu Alimentar
String subAlimentar[5];
int totalSubAlimentar = 0;

// Giro Personalizado
float personalizado = -1.0;
float girosPersonalizados = 1.0;

// Controle de menu
int opcaoAtual = 0;
int subOpcaoAtual = 0;
bool mostrandoMenu = true;
bool noSubmenu = false;
bool noPersonalizar = false;

// Controle de tempo
const unsigned long steps_base = 200;
unsigned long ultimaAcao = 0;
String horaAtual = "";
bool mostrandoHorario = false;

// Comunicação ESP8266 usando SoftwareSerial
#define esp8266 Serial
#define DEBUG true

/*---------------------------------------------------------------------------------------------
*                                      SETUP
---------------------------------------------------------------------------------------------*/
void setup()
{
  lcd.init(); // Inicializa LCD
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

  // Mensagem inicial
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("HELLO !!!");

  Serial.begin(9600);
  esp8266.begin(9600);
  delay(1000);

  atualizarSubmenu();
  mostrarMenu();
}

/*---------------------------------------------------------------------------------------------
*                                      FUNÇÕES DE MENU
---------------------------------------------------------------------------------------------*/
void mostrarMenu()
{
  lcd.clear();
  int par = (opcaoAtual / 2) * 2;
  for (int i = 0; i < 2; i++)
  {
    lcd.setCursor(0, i);
    int idx = par + i;
    if (idx >= totalOpcoes)
      break;
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
    lcd.setCursor(0, i);
    int idx = par + i;
    if (idx >= totalSubAlimentar)
      break;
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

void atualizarSubmenu()
{
  int idx = 0;
  subAlimentar[idx++] = "1/4 volta";
  subAlimentar[idx++] = "2/4 volta";
  subAlimentar[idx++] = "1 volta completa";
  if (personalizado > 0)
  {
    subAlimentar[idx++] = String(personalizado, 1) + " voltas";
  }
  subAlimentar[idx++] = "Personalizar";
  totalSubAlimentar = idx;
}

/*---------------------------------------------------------------------------------------------
*                                      FUNÇÕES MOTOR
---------------------------------------------------------------------------------------------*/
void executarSubOpcao(float voltas)
{
  // Apenas para uso com botões (LCD ativo)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alimentando...");
  lcd.setCursor(0, 1);
  lcd.print(voltas, 2);

  int passos = voltas * PASSOS_POR_VOLTA;
  motorExecuta(passos);

  delay(1000);
  noSubmenu = false;
  mostrandoMenu = true;
  mostrarMenu();
}

void executarSubOpcaoApp(float voltas)
{
  // Apenas para uso via app (sem LCD)
  int passos = voltas * PASSOS_POR_VOLTA;
  motorExecuta(passos);
}

void motorExecuta(int passos)
{
  digitalWrite(DIR, HIGH);
  for (int i = 0; i < passos; i++)
  {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(4000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(4000);
  }
  digitalWrite(STEP, LOW);
}

/*---------------------------------------------------------------------------------------------
*                                      LOOP
---------------------------------------------------------------------------------------------*/
void loop()
{
  unsigned long agora = millis();

  // Recebe dados do ESP (aplicativo)
  if (esp8266.available())
  {
    String linha = esp8266.readStringUntil('\n');
    linha.trim();

    if (linha.startsWith("ALIMENTAR:"))
    {
      float voltas = linha.substring(10).toFloat();
      executarSubOpcaoApp(voltas); // motor acionado sem LCD
    }

    if (linha.startsWith("HORA:"))
    {
      horaAtual = linha.substring(5); // "HH:MM:SS"
    }
  }

  // Controle de menu via botões
  if ((agora - ultimaAcao) > steps_base)
  {
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

          executarSubOpcao(voltas); // botões acionam LCD
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
          esp8266.println("PEDIR_HORA"); // pede a hora pro ESP
          delay(1500); // tempo do ESP responder
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Hora atual:");
          lcd.setCursor(0, 1);
          lcd.print(horaAtual);
        }
      }
      ultimaAcao = agora;
    }

    if (!digitalRead(BTN_BACK))
    {
      if (mostrandoHorario)
      {
        mostrandoHorario = false;
        mostrarMenu(); // volta ao menu principal
      }
      else if (noSubmenu)
      {
        noSubmenu = false;
        mostrarMenu();
      }
      else
      {
        mostrarMenu();
      }
      ultimaAcao = agora;
    }
  }
}

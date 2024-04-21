#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(32, 16, 2); // Inicializa o LCD

// Declaração das constantes para os pinos
const int sensor = A0; // Pino do sensor de temperatura
const int ledOn = 13; // LED indicando que a máquina está ligada
const int ledResfriamento = 12; // LED que indica o sistema de resfriamento
const int ledlimite = 11; // LED indicando limite de temperatura
const int acionarResfriamento = 60; // Temperatura para ligar o sistema de resfriamento 
const int limite = 90; // Temperatura limite
const int acionarDesligamento = 110; // Temperatura para desligar a máquina
const int maquinaResfriamento = 3; // Pino sistema de resfriamento
const int ledRele = 2; // Pino máquina
bool maquinaLigada = true; // Indica se a máquina está ligada
int ligarMaquina = 70; // Temperatura para ligar a máquina novamente
int tempC_anterior = 0; // Temperatura anterior
int tempC_display = 0; // Temperatura atual a ser exibida no display

void setup () {
  // Configura os pinos como saída
  pinMode(ledOn, OUTPUT);
  pinMode(ledResfriamento, OUTPUT);
  pinMode(ledlimite, OUTPUT);
  
  // Liga o LED indicando que a máquina está ligada
  digitalWrite(ledOn, HIGH);

  // Inicializa a comunicação serial
  Serial.begin(9600);
  
  // Inicializa o LCD e liga o backlight
  lcd.init();
  lcd.backlight();
  
  // Exibe o texto no LCD
  lcd.setCursor(2, 0);
  lcd.print("Temperatura:");
  lcd.setCursor(10, 1); 
  lcd.print("C");
}

void loop () {
  // Lê a tensão do sensor de temperatura e converte para temperatura em Celsius
  float tensao = analogRead(sensor) * (5.0 / 1023.0); 
  int tempC = round ((tensao-0.5)*100);
  
  // Exibe a temperatura no monitor serial
  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.println(" C");

  // Atualiza o display apenas se a temperatura mudou
  if (tempC != tempC_anterior) {
    lcd.setCursor(5, 1); 
    lcd.print("    "); 
    lcd.setCursor(6, 1); 
    lcd.print(tempC); 
    tempC_anterior = tempC;
  }

  // Controla o resfriamento da máquina com base na temperatura
  if (tempC <= acionarResfriamento) {
    digitalWrite(ledResfriamento, LOW);
    digitalWrite(maquinaResfriamento, LOW);
  } else {
    digitalWrite(ledResfriamento, HIGH);
    digitalWrite(maquinaResfriamento, HIGH);
  }
  
  // Liga o LED de limite se a temperatura ultrapassar o limite
  if (tempC >= limite) {
    digitalWrite(ledlimite, HIGH);
  } else {
    digitalWrite(ledlimite, LOW);
  }
  
  // Desliga a máquina se a temperatura ultrapassar o limite de desligamento
  if (tempC >= acionarDesligamento) {
    maquinaLigada = false;
  }
  
  // Controla o relé da máquina com base na variável maquinaLigada
  if (maquinaLigada) {
    digitalWrite(ledRele, HIGH);
  } else {
    digitalWrite(ledRele, LOW);
  }
  
  // Liga a máquina se estiver desligada e a temperatura for inferior ao limite de ligar
  if (!maquinaLigada && tempC <= ligarMaquina) {
    maquinaLigada = true;
  }
  
  // Aguarda um curto período antes de fazer a próxima leitura
  delay(200);
}

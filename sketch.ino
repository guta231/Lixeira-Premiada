
//importando bibliotecas

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"


LiquidCrystal_I2C lcd(0x27,16,2); // criando objeto do display lcd


HX711 scale; // criando objeto do módulo HX711

//variaveis globais

const int DT_pin = 3;
const int SCK_pin = 2;
int green = A0;
int red = A1;
int blue = A2;
int button = 7;



//função de inicialização
void setup() {

  scale.begin(DT_pin, SCK_pin);//inicializando HX711
  // Calibração inicial
  scale.tare(); // Definir o ponto zero da balança
  float valor_calibragem = 420; //valor de calibragem
  scale.set_scale(valor_calibragem); // setando valor de calibragem
  //configuração pinos lcd
  lcd.backlight();
  lcd.begin(16,2);
  //configurando pinos para o sensor de cores/potenciometros
  pinMode(green,INPUT);
  pinMode(red,INPUT);
  pinMode(blue,INPUT);
  pinMode(button, INPUT_PULLUP); // configurando pino do botão
}

void loop() {
  //printando o peso no display
  lcd.setCursor(8,1);
  float peso = scale.get_units(10);
  lcd.print(peso);
  lcd.print("KG");
  delay(1000);
  lcd.clear();

  //variaveis de leitura dos potenciometros

  int p_green = analogRead(green);
  int p_red = analogRead(red);
  int p_blue = analogRead(blue);

  //convertendo os valores maximos do potenciometro para igualar ao sensor de cores
  p_green = map(p_green,0,1023,0,256);
  p_red = map(p_red,0,1023,0,256);
  p_blue = map(p_blue,0,1023,0,256);

  //configurações do display para os valores de cores
  lcd.setCursor(0,0);
  lcd.print("R: ");
  lcd.print(p_red);
  lcd.setCursor(6,0);
  lcd.print(" G: ");
  lcd.print(p_green);
  lcd.setCursor(0,1);
  lcd.print("B: ");
  lcd.print(p_blue);

  //processo de identificação do plástico ao clicar o botão

  if (digitalRead(button) == 0){
      //plástico PET
      if (p_blue > 200 && p_green > 200 && p_red > 200){
        lcd.clear();
        lcd.setCursor(0,0);
        float valor_PET = peso*100;
        lcd.print("PET ");
        lcd.print(valor_PET);
      }
      //plástico HDPE
      else if (p_red < 50 && p_green < 50 && p_blue < 50){
        lcd.clear();
        lcd.setCursor(0,0);
        float valor_HDPE = peso*80;
        lcd.print("HDPE ");
        lcd.print(valor_HDPE);
      }
      //plástico PVC
      else if(p_red > 200 && p_green > 200 && p_blue < 100){
        lcd.clear();
        lcd.setCursor(0,0);
        float valor_PVC = peso*60;
        lcd.print("PVC ");
        lcd.print(valor_PVC);
      }
      //plástico LDPE
      else if (p_red > 150 && p_green > 150 && p_blue > 150){
        lcd.clear();
        lcd.setCursor(0,0);
        float valor_LDPE = peso*70;
        lcd.print("LDPE ");
        lcd.print(valor_LDPE);
      }
      //plástico PP
      else if (p_red > 100 && p_green > 100 && p_blue > 200){
        lcd.clear();
        lcd.setCursor(0,0);
        float valor_PP = peso*80;
        lcd.print("PP ");
        lcd.print(valor_PP);
      }
      //plástico PS
      else if (p_red < 100 && p_green < 100 && p_blue > 200){
        lcd.clear();
        lcd.setCursor(0,0);
        float valor_PS = peso*50;
        lcd.print("PS ");
      }
      //Outros plásticos
      else if (p_red > 200 && p_green < 100 && p_blue < 100){
        lcd.clear();
        lcd.setCursor(0,0);
        float valor_OUTROS = peso*90;
        lcd.print("OUTROS ");
        lcd.print(valor_OUTROS);
      }
      //não identificação de plástico
      else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("nao identificado");
    }
    //reset do display
    delay(5000);
    lcd.clear();
  }
}
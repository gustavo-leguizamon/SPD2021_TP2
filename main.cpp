// C++ code
//
#include <LiquidCrystal.h>

//CONFIG LCD - INDICA PINES UTILIZADOS
#define LCD_RS     2
#define LCD_ENABLE 3
#define LCD_DB4    4
#define LCD_DB5    5
#define LCD_DB6    6
#define LCD_DB7    7

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

void setup()
{
  lcd.begin(16, 2);
  lcd.print("Hola");
}

void loop()
{
  startGame();
}

//INICIA EL JUEGO
void startGame(){
  lcd.setCursor(13, 0);
  lcd.print(":");

  drawLives();
}

//DIBUJA TODAS LAS VIDAS DEL JUGADOR
void drawLives(){
  for (int i = 0; i < 3; i++){
    lcd.setCursor(14 + i, 0);
    lcd.print("*");
  }
}
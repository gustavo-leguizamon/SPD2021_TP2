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
/*
#define BUTTON_UP   10
#define BUTTON_DOWN 8
#define BUTTON_TEST 9
*/

#define AMOUNT_WORDS 5

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

char hiddenWords[AMOUNT_WORDS][20] = {
  "GUSTAVO",
  "AILEN",
  //"HOLA MIGUEL",
  "PERRO",
  "CEREBRON",
  "CUCA"
};

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  startGame();
}

void loop()
{
}

//INICIA EL JUEGO
void startGame(){
  lcd.setCursor(6, 0);
  lcd.print("A");

  lcd.setCursor(12, 0);
  lcd.print(":");

  drawLives();

  int word = selectRandomWord();
  //Serial.println(word);
  drawHiddenWord(hiddenWords[word]);
}

//DIBUJA TODAS LAS VIDAS DEL JUGADOR
void drawLives(){
  for (int i = 0; i < 3; i++){
    lcd.setCursor(13 + i, 0);
    lcd.print("*");
  }
}

//DIBUJA LOS CARACTERES CORRESPONDIENTES A LA PALABRA OCULTA
void drawHiddenWord(char word[]){
  //Serial.println(word);
  int wordLength = strlen(word);
  int posStart = (int)((16 - wordLength) / 2);
  for (int i = 0; i < wordLength; i++){
    lcd.setCursor(posStart + i, 1);
    lcd.print("_");
  }
}

//SELECCIONA UNA PALABRA ALEATORIA DEL ARRAY DE PALABRAS MEDIANTE SU POSICION
int selectRandomWord(){
  return random(0, AMOUNT_WORDS);
}
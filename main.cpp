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

#define BUTTON_UP   10
#define BUTTON_DOWN 8
#define BUTTON_TEST 9


#define AMOUNT_WORDS 5

//void antiBounces(int pinButton, int* stateBefore, void (*fun)());

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
int stateButtonUpBefore = LOW;

char currentLetter = 'A';

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

  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_TEST, INPUT);

  //COLOCA UNA SEMILLA ALEATORIA
  randomSeed(millis());

  startGame();
}

void loop()
{
  /*
  //Si presiona el boton SUBIR LETRA, revisa si se encuentra previamente presionado
  int stateButtonUpNow = digitalRead(BUTTON_UP);
  if (stateButtonUpNow == HIGH && stateButtonUpBefore == LOW)
  {
    incrementLetter();
    printCurrentLetter();
  }
  stateButtonUpBefore = stateButtonUpNow;  //Actualiza el estado del boton
  */

  //Si presiona el boton SUBIR LETRA, revisa si se encuentra previamente presionado
  //antiBounces(BUTTON_UP, &stateButtonUpBefore);
  if (newButtonPress(BUTTON_UP, &stateButtonUpBefore)){
    incrementLetter();
  }
  printCurrentLetter();
}

//INICIA EL JUEGO
void startGame(){
  printCurrentLetter();

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

//Muestra en ell display la letra actual para probar
void printCurrentLetter(){
  lcd.setCursor(6, 0);
  lcd.print(currentLetter);
}

//Incrementa la letra actual
void incrementLetter(){
  if (currentLetter >= 'Z'){ //Z
    currentLetter = 'A';
  }
  else{
    currentLetter += 1;
  }
}

//Revisa si un boton ya se encuentra presionado
/*
void antiBounces(int pinButton, int* stateBefore, void (*fun)()){
  int stateButtonUpNow = digitalRead(pinButton);
  if (stateButtonUpNow == HIGH && *stateBefore == LOW){
    //incrementLetter();
    fun();
    printCurrentLetter();
  }
  *stateBefore = stateButtonUpNow;  //Actualiza el estado del boton
}
*/
//Revisa si se presiono un boton y si el mismo ya se encuentra presionado de antes
int newButtonPress(int pinButton, int* stateBefore){
  int stateButtonUpNow = digitalRead(pinButton);
  //Serial.println(stateButtonUpNow == HIGH && *stateBefore == LOW);
  int wasPressed = stateButtonUpNow == HIGH && *stateBefore == LOW;
  *stateBefore = stateButtonUpNow;  //Actualiza el estado del boton

  return wasPressed;
}
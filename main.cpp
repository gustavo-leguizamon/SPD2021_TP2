// C++ code
//
#include <LiquidCrystal.h>
#include <string.h>

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
#define TOTAL_LIVES  5

void antiBounces(int pinButton, int* stateBefore, void (*callback)());

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

  //Coloca una semilla aleatoria leyendo el ruido del pin A0 conectado al aire
  randomSeed(analogRead(A0));

  startGame();
}

void loop()
{
  /*
  //Si presiona el boton SUBIR LETRA, revisa si se encuentra previamente presionado
  int stateButtonUpNow = digitalRead(BUTTON_UP);
  if (stateButtonUpNow == HIGH && stateButtonUpBefore == LOW)
  {
    nextLetter();
    printCurrentLetter();
  }
  stateButtonUpBefore = stateButtonUpNow;  //Actualiza el estado del boton
  */

  //Si presiona el boton SUBIR LETRA, revisa si se encuentra previamente presionado
  antiBounces(BUTTON_UP, &stateButtonUpBefore, nextLetter);
  /*
  if (newButtonPress(BUTTON_UP, &stateButtonUpBefore)){
    nextLetter();
  }
  */
  printCurrentLetter();
}

//Inicia el juego configurando valores por defecto
void startGame(){
  printCurrentLetter();

  drawLives(TOTAL_LIVES);

  //int word = selectPosRandomWord();
  //Serial.println(word);
  drawHiddenWord(hiddenWords[selectPosRandomWord()]);
}

//Dibuja todas las vidas del jugador, maximo 6
void drawLives(int total){
  if (total > 6){
    total = 6;
  }
  int posInit = 16 - total - 1;
  lcd.setCursor(posInit, 0);
  lcd.print(":");

  posInit++;

  for (int i = 0; i < total; i++){
    lcd.setCursor(posInit + i, 0);
    lcd.print("*");
  }
}

//Dibuja los caracteres correspondientes a la palabra oculta
void drawHiddenWord(char word[]){
  //Serial.println(word);
  int wordLength = strlen(word);
  int posStart = (int)((16 - wordLength) / 2);
  for (int i = 0; i < wordLength; i++){
    lcd.setCursor(posStart + i, 1);
    lcd.print("_");
  }
}

//Selecciona una palabra aleatoria del array de palabras mediante su posicion
int selectPosRandomWord(){
  return random(0, AMOUNT_WORDS);
}

//Muestra en el display la letra actual para probar
void printCurrentLetter(){
  int posLetter = 16 - TOTAL_LIVES * 2 - 2;

  lcd.setCursor(posLetter, 0);
  lcd.print(currentLetter);
}

//Mueve la letra actual a la siguiente en el diccionario
//Si se llega a la Z reinicia a la letra A
void nextLetter(){
  if (currentLetter >= 'Z'){
    currentLetter = 'A';
  }
  else{
    currentLetter++;
  }
}

//Revisa si un boton ya se encuentra presionado
//Si esta presionado de antes no hace nada, si no lo esta ejecuta la funcion pasada como callback
//Esto para lograr el efecto antirebote
void antiBounces(int pinButton, int* stateBefore, void (*callback)()){
  int stateButtonUpNow = digitalRead(pinButton);
  if (stateButtonUpNow == HIGH && *stateBefore == LOW){
    callback();
  }
  *stateBefore = stateButtonUpNow;  //Actualiza el estado del boton
}

//Indica si se presiono un boton y si el mismo ya se encuentra presionado de antes para lograr efecto antirebote.
//Si se presiono, y es la premera vez, retorna 1. Si se presiono, pero ya estaba presionado, retona 0.
int newButtonPress(int pinButton, int* stateBefore){
  int stateButtonUpNow = digitalRead(pinButton);
  //Serial.println(stateButtonUpNow == HIGH && *stateBefore == LOW);
  int wasPressed = stateButtonUpNow == HIGH && *stateBefore == LOW;
  *stateBefore = stateButtonUpNow;  //Actualiza el estado del boton

  return wasPressed;
}
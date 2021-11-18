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


#define AMOUNT_WORDS      5
#define TOTAL_LIVES       5
#define SECONDS_TO_REBOOT 3

void antiBounces(int pinButton, int* stateBefore, void (*callback)());
int checkFinish();
void startGame();
void drawLives(int total);
void clearLives();
void drawHiddenWord(char word[]);
void testLetter();
void drawMatchingLetters(char word[]);
int setPosRandomWord();
void printCurrentLetter();
void nextLetter();
void previousLetter ();
void antiBounces(int pinButton, int* stateBefore, void (*callback)());
int newButtonPress(int pinButton, int* stateBefore);
int secondsPassed(int seconds);
void rebootGame();

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

//Variables para mantener el estado previo de los botones
int stateButtonUpBefore = LOW;
int stateButtonTestBefore = LOW;
int stateButtonDownBefore = LOW;

char currentLetter;
int posSelectedWord;
int flagRebootGame;
int lives = TOTAL_LIVES;

unsigned long previousMillis = 0;

//Palabras para usar en el juego
char hiddenWords[AMOUNT_WORDS][20] = {
  "GUSTAVO",
  "AILEN",
  //"HOLA MIGUEL",
  "PERRO",
  "CEREBRON",
  "CUCA"
};

char auxWord [20];

//Se configura el lcd y los pines
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
  if (flagRebootGame){
    if (secondsPassed(SECONDS_TO_REBOOT)){
      startGame();
    }
  }
  else{
    //Si presiona el boton SUBIR LETRA, revisa si se encuentra previamente presionado
  	//Escucha si el boton esta apretado o no. Si esta apretado llama a nextLetter
    antiBounces(BUTTON_UP, &stateButtonUpBefore, nextLetter);

    //Si presiona el boton PROBAR LETRA, revisa si se encuentra previamente presionado
    //Escucha si el boton esta apretado o no. Si esta apretado llama a testLetter
    antiBounces(BUTTON_TEST, &stateButtonTestBefore, testLetter);
    
    //Si presiona el boton BAJAR LETRA, revisa si se encuentra previamente presionad
    //Escucha si el boton esta apretado o no. Si esta apretado llama a previousLetter
    antiBounces(BUTTON_DOWN, &stateButtonDownBefore , previousLetter);
    
    printCurrentLetter();
    
    if(checkFinish() ==1){
    	//ganaste
    	lcd.setCursor(4, 1);
    	lcd.print ("GANASTE");
    	rebootGame();
    }
    else{
    	if(checkFinish()== -1){
    		//perdiste
    		lcd.setCursor(4, 1);
    		lcd.print ("PERDISTE");
    		rebootGame();
    		
    	}
    }
	
    
  }
}

//Esta funcion devuelve 1 cuando las palabra ingresada coincide con la palabra seleccionada.
//Devuelve -1 cuando vidas es igual a 0.
//Devuelve 0 cuando no sucede ninguna de las condiciones anteriores.
int checkFinish(){
	int value = 0; //lo iniciamos en 0 para que no retorne basura.
	if(lives == 0){
		value = -1;
	}
	else{
		if(strcmp(hiddenWords[posSelectedWord] , auxWord) == 0){
			value = 1;
		}	 
	}
	return value;
}

//Inicia el juego configurando valores por defecto
void startGame(){
  flagRebootGame = 0;   //Desactiva el flag de reinicio
  currentLetter = 'A';  //Inicializa en la letra A
  lives = TOTAL_LIVES;

  lcd.clear(); //Limpia completo el display para dejarlo en blanco

  printCurrentLetter();
  setPosRandomWord(); //genera un numero random y lo guarda en una variable global llamada posSeledtWord

  drawLives(lives);

  drawHiddenWord(hiddenWords[posSelectedWord]); //dibuja las lineas para completar palabra
}

//Dibuja todas las vidas del jugador, maximo 6
void drawLives(int total){
  int posInit = 16 - total;
  lcd.setCursor(posInit, 0);
  clearLives(); // limpiamos todas las vidas para volver a dibujarlas sin tener que borrar 1 por 1 
  for (int i = 0; i < total; i++){
    lcd.setCursor(posInit + i, 0);
    lcd.print("*"); 
  }
}

// Limpia las vidas que se muestran en el lcd.
void clearLives(){
	for(int i = 16 - TOTAL_LIVES; i <= 16; i++){
		lcd.setCursor(i, 0);
		lcd.print(" ");
	}
}

//Dibuja las lineas correspondientes a la palabra oculta
//Recibe la palabra que debe adividar el usuario.
void drawHiddenWord(char word[]){
  int wordLength = strlen(word); //Se queda con la longitud de la palabra para saber cuantos espacios dibujar
  int posLcd = (int)((16 - wordLength) / 2); // //Posicion inicial de la palabra en el lcd
  for (int i = 0; i < wordLength; i++){
    lcd.setCursor(posLcd + i, 1);
    lcd.print("_");
  }
}

//Delegado encargado de probar la letra seleccionada
void testLetter(){
  drawMatchingLetters(hiddenWords[posSelectedWord]);
}

//Primero corrobora si la letra (currentLetter) va en esa posicion y en el caso de que si la dibuja 
void drawMatchingLetters(char word[]){
  int flagMatch = 0;
  int wordLength = strlen(word);
  int posLcd = (int)((16 - wordLength) / 2); //Posicion inicial de la palabra en el lcd
  for (int i = 0; i < wordLength; i++){
    if (word[i] == currentLetter){
      flagMatch = 1;
      lcd.setCursor(posLcd, 1); // 1 es el renglon del led
      lcd.print(currentLetter);   
      auxWord[i]= currentLetter; //guardo la palabra que se esta formando.
    }
    posLcd++;
  }
  if (flagMatch == 0){
	  //Si entra al if es porque no macheo con ninguna letra y le debe descontar una vida
	  lives--;
  }
  drawLives(lives);
}

//Arroja un numero random y lo guarda en la variable global postSelectedWord que nos va a servir para elegir la palabra del listado
int setPosRandomWord(){
  posSelectedWord = random(0, AMOUNT_WORDS);
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

void previousLetter (){
	if (currentLetter <= 'A'){
		currentLetter = 'Z';
	}
	else{
		currentLetter--;
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

//Indica si pasaron la cantidad de segundos indicada en el parametro
int secondsPassed(int seconds){
  int passed = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000 * seconds){
    previousMillis = currentMillis;
    passed = 1;
  }
  return passed;
}

//Marca el juego para reiniciar
void rebootGame(){
  flagRebootGame = 1;
  previousMillis = millis();
}
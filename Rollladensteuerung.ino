
#include <BitBool.h>



bool invertState = true;

// constants won't change. They're used here to
const int buttonPinsLen = 18;
const int relaisNumber = 8;   // number of relais
const int allUpButtonPin = 11; //pin for button to move up
const int allDownButtonPin = 12; //pin for button to move down
const int upButtonPins[relaisNumber] = {3, 4, 5, 6, 7, 8, 9, 10};
const int downButtonPins[relaisNumber] = {22, 23, 24, 25, 26, 27, 28, 29};
const int upRelaisPins[relaisNumber] = {38, 40, 42, 44, 46, 48, 50, 52}; // array of relais pins
const int downRelaisPins[relaisNumber] = {39, 41, 43, 45, 47, 49, 51, 53};


// Variables will change:
int buttonPins[2*relaisNumber +2] = {}; // to be initialized in setup
int relaisPins[2*relaisNumber] = {};
int relaisPinsLen = 2 * relaisNumber;
bool buttonStates[buttonPinsLen] = {};
bool relaisStates[buttonPinsLen] = {}; //has to contain statusflags for special buttons too
bool reading[buttonPinsLen] = {};

// the following variables are long's because the time, measured in miliseconds
long lastActionTime = 0;  // the last time the output pin was toggled
long debounceDelay = 200;    // the debounce time; increase if the output flickers

void setup() {

  Serial.begin(115200);
  delay(0.1);
  Serial.println();Serial.println();
  Serial.println("#######################    BEGIN SETUP    ############################");

  
  Serial.print("buttonPinsLen : ");
  Serial.println(buttonPinsLen);  // devide by 2 because sizeof gives number of bytes

  // INIT RELAIS
  for (int i = 0; i < 2 * relaisNumber; i++)
  {
    if (i < relaisNumber) {relaisPins[i] = upRelaisPins[i];}
    if (i >= relaisNumber) {relaisPins[i] = downRelaisPins[i-relaisNumber];}
    pinMode(relaisPins[i], OUTPUT);
    relaisStates[i] = !invertState;
  }

  // PRINT RELAIS 
  Serial.println("relaisPins: ");
  for ( int i = 0; i < relaisPinsLen; i++) {Serial.print(relaisPins[i]);Serial.print(", ");}
  Serial.println();
  Serial.println("relaisStates: ");
  for ( int i = 0; i < relaisPinsLen; i++) {Serial.print(relaisStates[i]);Serial.print(", ");}
  Serial.println();

  // INIT INPUT 1
  for (int i = 0; i < buttonPinsLen; i++)
  {
    if (i < relaisNumber)
    {
      buttonPins[i] = upButtonPins[i];
    }
    if (i >= relaisNumber && i < 2 * relaisNumber)
    {
      buttonPins[i] = downButtonPins[i-relaisNumber];
    }
    if (i >= 2* relaisNumber)
    {
      if ( i == 2 * relaisNumber ) { buttonPins[i] = allUpButtonPin;}
      if ( i == 2 * relaisNumber + 1 ) { buttonPins[i] = allDownButtonPin;}
    }    
  } // at this point buttonPins is filled, so lets see it ;) 
  
  // INIT INPUT 2
  for (int i = 0; i < buttonPinsLen; i ++ )
  {
    pinMode(buttonPins[i], INPUT);
    reading[i] = digitalRead(buttonPins[i]);
    buttonStates[i] = reading[i]; 
  }
   // PRINT INPUT 
  Serial.println("buttonPins: ");
  for ( int i = 0; i < buttonPinsLen; i++) {Serial.print(buttonPins[i]);Serial.print(", ");}
  Serial.println();
  Serial.println("buttonStates: ");
  for ( int i = 0; i < buttonPinsLen; i++) {Serial.print(buttonStates[i]);Serial.print(", ");}
  Serial.println();
  Serial.println("readings    : ");
  for ( int i = 0; i < buttonPinsLen; i++) {Serial.print(reading[i]);Serial.print(", ");}
  Serial.println();

  Serial.println("#######################    SETUP COMPLETE    ############################");
  

}
//###################################################################################################
//###########################    LOOP   #############################################################
void loop() {
  
  // READ BUTTON STATES
  for (int i = 0; i < buttonPinsLen; i ++ )
  {
    reading[i] = digitalRead(buttonPins[i]);
  }


  // COMPARE ACTUAL READING WITH BUTTON STATES
  // if changed -> set lastActionTime and 
  for (int i = 0; i < buttonPinsLen; i ++ )
  {


    // if actual action happened
    if (buttonStates[i] != reading[i])
    {
Serial.print("i");
      lastActionTime = millis();
      buttonStates[i] = reading[i];

    }
  }


  // SET RELAIS STATES DUE TO DEBOUNCED BUTTON STATE
  for (int i = 0; i < buttonPinsLen; i++)
  {    
    // if time since last action is greater than debounce delay
    if (buttonStates[i] == reading[i] && (millis() - lastActionTime > debounceDelay))
    { 
      relaisStates[i] = buttonStates[i];
    }
  }


  // if special FLAG! in relais array is set:
  if (relaisStates[buttonPinsLen - 2] == invertState)
  {
    // set every relaisState to ture
    for (int i = 0; i < relaisNumber; i++)
    {
      relaisStates[i] = !invertState;    
    }
    // set last half to false
    for (int i = relaisNumber ;  i < 2 * relaisNumber; i++)
    {
      relaisStates[i] = invertState;    
    }
    
  }

    // if special FLAG2 in relais array is set:
    // maybe do some functioning to avoid code copy
  if (relaisStates[buttonPinsLen - 1] == invertState)
  {
    // set every relaisState to ture
    for (int i = 0; i < relaisNumber; i++)
    {
      relaisStates[i] = invertState;    
    }
    // set last half to false
    for (int i = relaisNumber ;  i < 2 * relaisNumber; i++)
    {
      relaisStates[i] = !invertState;    
    }
    
  }
  

  // DISPLAY RELAIS STATES
//  Serial.print("readings:     ");
//  for ( int i = 0; i < buttonPinsLen; i++) {Serial.print(reading[i]);Serial.print(", ");}
//  Serial.println();
//  Serial.print("buttonSt:     ");
//  for ( int i = 0; i < buttonPinsLen; i++) {Serial.print(buttonStates[i]);Serial.print(", ");}
//  Serial.println();
//  Serial.print("relaisSt:     ");
//  for ( int i = 0; i < 2 * relaisNumber; i++) {Serial.print(relaisStates[i]);Serial.print(", ");}
//  Serial.println();

  for (int i = 0; i < 2 * relaisNumber; i++)
  {
    digitalWrite(relaisPins[i], relaisStates[i]); 
  }


}



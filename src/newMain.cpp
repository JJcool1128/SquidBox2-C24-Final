// Includeing all the neccessary libraries for the cpp file
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Defining all the pin values for the LCD screen
#define TFT_CS         7
#define TFT_RST        5
#define TFT_DC         6
#define TFT_MOSI       11
#define TFT_SCLK       13
#define TFT_MISO       -1 // Not used
#define TFT_BL         -1 // Not used

// Defining the size of each piano key 
#define WHITE_KEY_WIDTH  20
#define WHITE_KEY_HEIGHT 100
#define BLACK_KEY_WIDTH  15
#define BLACK_KEY_HEIGHT 60

// Creating tft object which controls the LCD screen
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int menu = 0;
int menu_select = 1;
int mainKey = 0;
int mainVolume = 100;
int newVolume = 100;

// setting the pin values for all electronics
const uint8_t upButton = A0;
const uint8_t selectButton = A1;
const uint8_t downButton = A2;
const uint8_t chord1 = A3;
const uint8_t chord2 = A4;
const uint8_t chord3 = 3;
const uint8_t chord4 = 4;
const uint8_t volumeKnob = A5;

//methods written by A23 team
void startupAnimation();
void printMenu();

//methods written by C24 team
void printKeyboard(int rootNote, int chordKey);
void diatonic(int key);
void printVolume(int volume);
void printKey(int key);
String intToChordLetter(int chordNumber);
String intToChordType(int chordNumber);

void setup() {
    Serial.println("Setup");  
    Serial.begin(9600); 

    //screen setup
    tft.init(240, 320);
    tft.setRotation(1); //LANDSCAPE MODE
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextWrap(true);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(4);

    // Setting minModes for all used pins
    pinMode(upButton, INPUT);
    pinMode(downButton, INPUT);
    pinMode(selectButton, INPUT);
    pinMode(chord1, INPUT);
    pinMode(chord2, INPUT);
    pinMode(chord3, INPUT);
    pinMode(chord4, INPUT);
    pinMode(volumeKnob, INPUT);

    startupAnimation();
    //reset screen to print menu
    tft.fillScreen(ST77XX_BLACK);
}

// State machine

void loop() {
    switch(menu) {   
        /* A23 Code Starts Here */ 
        case 0: //default menu
        printMenu(); //prints out the starting menu on the display
            switch(menu_select){
                case 1: //play
                    //draw the selection cursor box
                    //note to future, this could probably be moved to its own method
                    tft.drawFastHLine(52, 92, 228, ST77XX_WHITE);
                    tft.drawFastVLine(52, 92, 28, ST77XX_WHITE);
                    tft.drawFastHLine(52, 120, 228, ST77XX_WHITE);
                    tft.drawFastVLine(280, 92, 28, ST77XX_WHITE);
                    if(digitalRead(downButton)==HIGH){ //if the down button is pressed
                        Serial.println("down button");
                        menu_select = 2; //move the cursor down (go to state 2)
                        //erase the selection cursor box (draw over it in black)
                        tft.drawFastHLine(52, 92, 228, ST77XX_BLACK);
                        tft.drawFastVLine(52, 92, 28, ST77XX_BLACK);
                        tft.drawFastHLine(52, 120, 228, ST77XX_BLACK);
                        tft.drawFastVLine(280, 92, 28, ST77XX_BLACK);
                        delay(200);
                    }
                    if(digitalRead(selectButton)==HIGH){
                        Serial.println("select button");
                        menu = 1; //move the cursor down (go to state 2)
                        //erase the selection cursor box (draw over it in black)
                        tft.fillScreen(ST77XX_BLACK);
                        delay(200);
                    }
                    break;
                case 2: //select key
                    tft.drawFastHLine(52, 127, 228, ST77XX_WHITE);
                    tft.drawFastVLine(52, 127, 28, ST77XX_WHITE);
                    tft.drawFastHLine(52, 155, 228, ST77XX_WHITE);
                    tft.drawFastVLine(280, 127, 28, ST77XX_WHITE);
                    if(digitalRead(downButton)==HIGH){ //if the down button is pressed
                        Serial.println("down button");
                        menu_select = 3; //move the cursor down (go to state 3)
                        //erase the selection cursor box (draw over it in black)
                        tft.drawFastHLine(52, 127, 228, ST77XX_BLACK);
                        tft.drawFastVLine(52, 127, 28, ST77XX_BLACK);
                        tft.drawFastHLine(52, 155, 228, ST77XX_BLACK);
                        tft.drawFastVLine(280, 127, 28, ST77XX_BLACK);
                        delay(200);
                    }
                    if(digitalRead(upButton)==HIGH){ //if the up button is pressed
                        Serial.println("up button");
                        menu_select = 1; //move the cursor up (go to state 1)
                        //erase the selection cursor box (draw over it in black)
                        tft.drawFastHLine(52, 127, 228, ST77XX_BLACK);
                        tft.drawFastVLine(52, 127, 28, ST77XX_BLACK);
                        tft.drawFastHLine(52, 155, 228, ST77XX_BLACK);
                        tft.drawFastVLine(280, 127, 28, ST77XX_BLACK);
                        delay(200);
                    }
                    break;
                  case 3: //bluetooth settings
                      tft.drawFastHLine(52, 162, 228, ST77XX_WHITE);
                      tft.drawFastVLine(52, 162, 28, ST77XX_WHITE);
                      tft.drawFastHLine(52, 190, 228, ST77XX_WHITE);
                      tft.drawFastVLine(280, 162, 28, ST77XX_WHITE);

                      if(digitalRead(upButton)==HIGH){ //if the up button is pressed
                        Serial.println("up button");
                        menu_select = 2; //move the cursor up (go to state 2)
                        //erase the selection cursor box (draw over it in black)
                        tft.drawFastHLine(52, 162, 228, ST77XX_BLACK);
                        tft.drawFastVLine(52, 162, 28, ST77XX_BLACK);
                        tft.drawFastHLine(52, 190, 228, ST77XX_BLACK);
                        tft.drawFastVLine(280, 162, 28, ST77XX_BLACK);
                        delay(200);
                      }
                      break;
                }
                break;
                /* A23 Code Ends Here */ 
        case 1:
            // For Play Mode
            printKeyboard(-1,0);
            tft.setCursor(10, 10);
            tft.setTextSize(4);
            // Printing Key
            tft.println("Key:");
            printKey(0);
            menu = 2;
            // Printing Volume
            printVolume(analogRead(volumeKnob));
            break;
        case 2:
            //actually using the buttons here 
            if(digitalRead(downButton)==HIGH && mainKey>0){ //if the down button is pressed, lower the key by 1 semitone
                Serial.println("down button");
                mainKey = mainKey - 1;
                tft.fillRect(0, 40, 80, 50, ST77XX_BLACK);
                printKey(mainKey);
                delay(200);
            }
            if(digitalRead(upButton)==HIGH && mainKey<11){ //if the up button is pressed, raise the key by 1 semitone
                Serial.println("up button");
                mainKey = mainKey + 1;
                tft.fillRect(0, 40, 80, 50, ST77XX_BLACK);
                printKey(mainKey);
                delay(200);    
            }
            diatonic(mainKey);
            // mapping analog input from 0 to 100
            mainVolume = map(analogRead(volumeKnob), 0, 1023, 0, 100);
            // Chicking if the volume has changed by 5 since last change 
            if(mainVolume-5 > newVolume || mainVolume+5 < newVolume ){
                printVolume(analogRead(volumeKnob));
                newVolume = mainVolume;
            }
            break;
          case 3:
            // code block
            break;
      }
}

// Prints out the inputted volume 
void printVolume(int volume){
    tft.fillRect(240, 50, 100, 50, ST77XX_BLACK);
    tft.setCursor(220, 10);
    tft.setTextSize(4);
    tft.println("Vol:");
    tft.setCursor(240, 50);
    tft.println((map(volume, 0, 1023, 0, 100)));
}

// Prints the current Key
void printKey(int key){
        tft.setCursor(20, 50);
        tft.println((intToChordLetter(key)));
}

// Takes a number from 0 to 11 and returns a string representing a note after C. (0 -> C, 1->C#, 2->D, etc.)
String intToChordLetter(int chordNumber){
    String chordString = "C";
    switch(chordNumber){
        case 0:
            chordString = "C";
            break;
        case 1:
            chordString = "C#";
            break;            
        case 2:
            chordString = "D";
            break;
        case 3:
            chordString = "D#";
            break;
        case 4:
            chordString = "E";
            break;
        case 5:
            chordString = "F";
            break;
        case 6:
            chordString = "F#";
            break;
        case 7:
            chordString = "G";
            break;
        case 8:
            chordString = "G#";
            break;
        case 9:
            chordString = "A";
            break;
        case 10:
            chordString = "A#";
            break;
        case 11:
            chordString = "B";
            break;
    }
    return chordString;
}

// Takes a number from 0 to 2 and returns a string representing a chord type (Major, Minor, or Diminished)
String intToChordType(int chordNumber){
    String chordTypeString = "maj";
    switch(chordNumber){
        case 0:
            chordTypeString = "maj";
            break;
        case 1:
            chordTypeString = "min";
            break;
        case 2:
            chordTypeString = "dim";
            break;
    }
    return chordTypeString;
}

// Checks for the pressing of any buttons and calls the print keybard function for the given chord pressed and key the chord is in 
// Only works for major diatonic keys 
void diatonic(int key){
    if (digitalRead(chord1)==HIGH){
        tft.fillRect(80, 40, 150, 50, ST77XX_BLACK);
        Serial.println("chord 1");
        printKeyboard(key,0);
        tft.setCursor(95, 50);
        tft.setTextSize(4);
        tft.println(intToChordLetter(key));
        tft.setCursor(145, 50);
        tft.println(intToChordType(0));   
        delay(100); 
    }
    else if (digitalRead(chord2)==HIGH){
        tft.fillRect(80, 40, 150, 50, ST77XX_BLACK);
        Serial.println("chord 2");
        printKeyboard(key+2,1);
        tft.setCursor(95, 50);
        tft.setTextSize(4);
        tft.println(intToChordLetter(key+2));
        tft.setCursor(145, 50);
        tft.println(intToChordType(1));        
        delay(100); 
    }
    else if (digitalRead(chord3)==HIGH){
        tft.fillRect(80, 40, 150, 50, ST77XX_BLACK);
        Serial.println("chord 3");
        printKeyboard(key+4,1);
        tft.setCursor(95, 50);
        tft.setTextSize(4);
        tft.println(intToChordLetter(key+4));
        tft.setCursor(145, 50);
        tft.println(intToChordType(1));
        delay(100); 
    }
    else if (digitalRead(chord4)==HIGH){
        tft.fillRect(80, 40, 150, 50, ST77XX_BLACK);
        Serial.println("chord 4");
        printKeyboard(key+5,0);
        tft.setCursor(95, 50);
        tft.setTextSize(4);
        tft.println(intToChordLetter(key+5));
        tft.setCursor(145, 50);
        tft.println(intToChordType(0));
        delay(100); 
    }
}

// Prints the menu (made by A23)
void printMenu(){
    tft.drawFastHLine(20, 20, 280, ST77XX_WHITE);
    tft.drawFastVLine(20, 20, 100, ST77XX_WHITE);
    tft.drawFastHLine(20, 220, 280, ST77XX_WHITE);
    tft.drawFastVLine(300, 20, 100, ST77XX_WHITE);
    tft.setTextSize(3);
    tft.setCursor(90, 40);
    tft.setTextColor(ST77XX_RED);
    tft.print("S");
    tft.setTextColor(ST77XX_ORANGE);
    tft.print("q");
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("u");
    tft.setTextColor(ST77XX_GREEN);
    tft.print("i");
    tft.setTextColor(ST77XX_CYAN);
    tft.print("d");
    tft.setTextColor(ST77XX_BLUE);
    tft.print("b");
    tft.setTextColor(ST77XX_MAGENTA);
    tft.print("o");
    tft.setTextColor(ST77XX_WHITE);
    tft.println("x");
    tft.setCursor(140, 100);
    tft.setTextSize(2);
    tft.println("Play");
    tft.setCursor(100, 135);
    tft.println("Select Key");
    tft.setCursor(60, 170);
    tft.println("Bluetooth Settings");
}

// Prints the keyboard depending on the root note of the chord and its type 
void printKeyboard(int rootNote, int chordType){
    int whiteKeyPos = 40; // Initial position of the first white key
    int blackKeyPos = 55; // Initial position of the first black key 
    //no chord pressed
    if(rootNote == -1){
        for (int i = 0; i < 17; i++) {
            // Draw white key
            if(i != 1 && i != 6 && i != 8 && i != 13 && i != 15 && i != 3 && i != 10){
                tft.fillRect(whiteKeyPos, 100, WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT, ST77XX_WHITE);
                whiteKeyPos = whiteKeyPos + WHITE_KEY_WIDTH + 5;
            }
        }
        // Draw black key if applicable
        for (int i = 0; i < 17; i++) {
            if (i == 1 || i == 6 || i == 8 || i == 13 || i == 15) {
                tft.fillRect(blackKeyPos, 100, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT, ST77XX_BLACK);
                blackKeyPos = blackKeyPos + WHITE_KEY_WIDTH + 5;
            }
            else if (i == 3 || i == 10) {
                tft.fillRect(blackKeyPos, 100, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT, ST77XX_BLACK);
                blackKeyPos = blackKeyPos + 2*WHITE_KEY_WIDTH + 2*5;
            }        
        }
    }
    //for rootNote != 0  
    else {
        for (int i = 0; i < 17; i++) {
            int note = i;
            bool isChordNote = false;

            // Check if the current note is part of the chord based on the chord type
            switch (chordType) {
                case 0: // Major chord
                    isChordNote = (note == rootNote) || (note == rootNote+4) || (note == rootNote+7);
                    break;
                case 1: // Minor chord
                    isChordNote = (note == rootNote) || (note == rootNote+3) || (note == rootNote+7);
                    break;
                case 2: // Diminished chord
                    isChordNote = (note == rootNote) || (note == rootNote+3) || (note == rootNote+6);
                    break;
            }

            if(i != 1 && i != 6 && i != 8 && i != 13 && i != 15 && i != 3 && i != 10){
                // Draw white key
                tft.fillRect(whiteKeyPos, 100, WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT, isChordNote ? ST77XX_ORANGE : ST77XX_WHITE);
                whiteKeyPos = whiteKeyPos + WHITE_KEY_WIDTH + 5;
            }
        }
        // Draw black key if applicable
        for (int i = 0; i < 17; i++) {
            int note = i;
            bool isChordNote = false;

            // Check if the current note is part of the chord based on the chord type
            switch (chordType) {
                case 0: // Major chord
                    isChordNote = (note == rootNote) || (note == rootNote+4) || (note == rootNote+7);
                    break;
                case 1: // Minor chord
                    isChordNote = (note == rootNote) || (note == rootNote+3) || (note == rootNote+7);
                    break;
                case 2: // Diminished chord
                    isChordNote = (note == rootNote) || (note == rootNote+3) || (note == rootNote+6);
                    break;
            }
            if (i == 1 || i == 6 || i == 8 || i == 13 || i == 15) {
                tft.fillRect(blackKeyPos, 100, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT, isChordNote ? ST77XX_ORANGE : ST77XX_BLACK);
                blackKeyPos = blackKeyPos + WHITE_KEY_WIDTH + 5;
            }
            else if (i == 3 || i == 10) {
                tft.fillRect(blackKeyPos, 100, BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT, isChordNote ? ST77XX_ORANGE : ST77XX_BLACK);
                blackKeyPos = blackKeyPos + 2*WHITE_KEY_WIDTH + 2*5;
            }        
        }
    }
}

// Code for startup animation (A23 again)
void startupAnimation(){
    tft.setTextSize(4);
    tft.drawFastHLine(40, 80, 240, ST77XX_WHITE);
    tft.drawFastVLine(40, 80, 80, ST77XX_WHITE);
    tft.drawFastHLine(40, 160, 240, ST77XX_WHITE);
    tft.drawFastVLine(280, 80, 80, ST77XX_WHITE);
    tft.setCursor(65, 105);
    tft.setTextColor(ST77XX_RED);
    tft.print("S");
    tft.setTextColor(ST77XX_ORANGE);
    tft.print("q");
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("u");
    tft.setTextColor(ST77XX_GREEN);
    tft.print("i");
    tft.setTextColor(ST77XX_CYAN);
    tft.print("d");
    tft.setTextColor(ST77XX_BLUE);
    tft.print("b");
    tft.setTextColor(ST77XX_MAGENTA);
    tft.print("o");
    tft.setTextColor(ST77XX_WHITE);
    tft.print("x");
}


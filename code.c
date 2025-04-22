#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CARKABUTTONPIN 9
#define TECKABUTTONPIN 10
#define ODDELBUTTONPIN 11
#define DISPLAY_NUMOFCOLUMNS 16

int carkaButtonState = 0;
int carkaButtonLastState = 0;
int teckaButtonState = 0;
int teckaButtonLastState = 0;
int oddelButtonState = 0;
int oddelButtonLastState = 0;
String tonesBuffer;
String text;
String expectedText;

String symbolsAlphabet[][2] = {
  { ".-","A" },
  { "-...","B" },
  { "-.-.","C" },
  { "-..","D" },
  { ".","E" },
  { "..-.","F" },
  { "--.","G" },
  { "....","H" },
  { "..","I" },
  { ".---","J" },
  { "-.-","K" },
  { ".-..","L" },
  { "--","M" },
  { "-.","N" },
  { "---","O" },
  { ".--.","P" },
  { "--.-","Q" },
  { ".-.","R" },
  { "...","S" },
  { "-","T" },
  { "..-","U" },
  { "...-","V" },
  { ".--","W" },
  { "-..-","X" },
  { "-.--","Y" },
  { "--..","Z" },
  { ".----","1" },
  { "..---","2" },
  { "...--","3" },
  { "....-","4" },
  { ".....","5" },
  { "-....","6" },
  { "--...","7" },
  { "---..","8" },
  { "----.","9" },
  { "-----","0"}
};

LiquidCrystal_I2C lcd(0x27, DISPLAY_NUMOFCOLUMNS, 2);

char getToneFromButtonStates() {
  if (!carkaButtonState && carkaButtonLastState)
    return '-';
  if (!teckaButtonState && teckaButtonLastState)
    return '.';
  if (!oddelButtonState && oddelButtonLastState)
    return ' ';
  return (char)0;
}

char getSymbolFromBuffer() {
  if (tonesBuffer == "")
    return ' ';
  for (int i = 0; i < sizeof symbolsAlphabet / sizeof symbolsAlphabet[0]; i++)
    if (tonesBuffer == symbolsAlphabet[i][0])
      return symbolsAlphabet[i][1][0];
  return (char)0;
}

void extractActionFromTonesBuffer() {
  if (tonesBuffer == "......")
    text.remove(text.length() - 1, 1);
  if (tonesBuffer == "------")
    text = "";
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Morse Code");
  lcd.setCursor(0, 1);
  lcd.print("Decoder");
  delay(1000);
  pinMode(CARKABUTTONPIN, INPUT);
  pinMode(TECKABUTTONPIN, INPUT);
  pinMode(ODDELBUTTONPIN, INPUT);
}

void loop() {
  carkaButtonState = digitalRead(CARKABUTTONPIN);
  teckaButtonState = digitalRead(TECKABUTTONPIN);
  oddelButtonState = digitalRead(ODDELBUTTONPIN);

  char tone = getToneFromButtonStates();

  if (tone = (char)0) {
    if (tone == ' ') {
      char symbol = getSymbolFromBuffer();
      if (symbol != (char)0) {
        text += symbol;
        if (text.length() > DISPLAY_NUMOFCOLUMNS) {
          text = (String)symbol;
        }
      } else {
        extractActionFromTonesBuffer();
      }
      tonesBuffer = "";
    } else {
      tonesBuffer += tone;
      if (tonesBuffer.length() > DISPLAY_NUMOFCOLUMNS) {
        tonesBuffer = (String)tone;
      }
    }

    lcd.clear();
    lcd.print(text);
    lcd.setCursor(0, 1);
    lcd.print(tonesBuffer);
  }

  carkaButtonLastState = carkaButtonState;
  teckaButtonLastState = teckaButtonState;
  oddelButtonLastState = oddelButtonState;
}

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Ustawienie adresu I2C dla wyświetlacza

const int buttonPins[] = {5, 4, 3, 2};
const int ledPins[] = {9, 8, 7, 6};
const int numButtons = sizeof(buttonPins) / sizeof(int);
int score = 0;
int currentLed = 0;
bool gameEnded = false;
const int beeperPin = 10;  // Pin, do którego podłączony jest beeper
bool gameActive = false;  // Zmienna kontrolująca, czy gra jest aktywna

void setup() {
  lcd.begin(16, 2);
  lcd.print("Nacisnij dowolny");
  lcd.setCursor(4, 1);
  lcd.print("przycisk");
  
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(beeperPin, OUTPUT);  // Ustawienie pinu beepera jako wyjście

  // Czekaj na naciśnięcie dowolnego przycisku
  while (true) {
    for (int i = 0; i < numButtons; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        delay(2000);  // Dodatkowe opóźnienie po rozpoczęciu gry
        gameActive = true;  // Rozpoczęcie gry
        return;
      }
    }
  }
}

bool isOnlyOneButtonPressed(int buttonIndex) {
  for (int i = 0; i < numButtons; i++) {
    if (i != buttonIndex && digitalRead(buttonPins[i]) == LOW) {
      return false;
    }
  }
  return true;
}

bool areAllButtonsPressed() {
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      return false;
    }
  }
  return true;
}

void loop() {
  if (!gameActive) {
    if (areAllButtonsPressed()) {
      gameEnded = false;
      gameActive = true;
      score = 0;
      lcd.clear();
      delay(2000);
    }
    return;  // Jeśli gra nie jest aktywna, nie wykonuj reszty kodu w pętli
  }

  if (gameEnded) {
    lcd.clear();
    lcd.print("Koniec gry!");
    lcd.setCursor(0, 1);
    lcd.print("Punkty: ");
    lcd.print(score);
    tone(beeperPin, 440, 500);  // Gra dźwięk o częstotliwości 440Hz na 500ms
    gameActive = false;  // Zakończenie gry
    return;
  }

  currentLed = random(numButtons);
  digitalWrite(ledPins[currentLed], HIGH);
  unsigned long startTime = millis();
  int delayTime = 1000 - score * 30;  // Skracamy czas o 30ms za każdym razem, gdy gracz zdobędzie punkt
  if (delayTime < 200) {  // Ustalamy minimalne opóźnienie na 200ms
    delayTime = 200;
  }
  bool buttonPressed = false;
  while (millis() - startTime < delayTime) {
    if (digitalRead(buttonPins[currentLed]) == LOW && isOnlyOneButtonPressed(currentLed)) {
      score++;
      buttonPressed = true;
      lcd.clear();
      lcd.print("Punkty: ");
      lcd.print(score);
      tone(beeperPin, 880, 100);  // Gra dźwięk o częstotliwości 880Hz na 100ms
      break;
    }
  }
  digitalWrite(ledPins[currentLed], LOW);
  if (!buttonPressed) {
    gameEnded = true;
  }
  delay(delayTime);
}
#include <EEPROM.h>

#include <LiquidCrystal.h>


LiquidCrystal lcd(34, 32, 42, 40, 38, 36);

int level = 1;
const char* letters[][6] = {{"a", "e", "i", "o", "u"}, { "b", "c", "d", "f", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "y", "z" }, { "ch", "ng", "sh", "th", "wh" }, { "ay", "ea", "ie", "oe", "ue" }, { "air", "ar", "ear", "er", "or", "ure" }, { "ci", "ge", "kn", "wr" }};
int sound;
int answers[4] = {-1};
int newsound;
int correctanswer;
const int button[4] = { 33, 35, 37, 39 };
bool failed = false;
int score = 0;
int previousanswers[5];
const int levelnumfiles[] = { 5, 21, 5, 5, 6, 4 };

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("hello. starting.");

  pinMode(button[1], INPUT_PULLUP);
  pinMode(button[2], INPUT_PULLUP);
  pinMode(button[3], INPUT_PULLUP);
  pinMode(button[4], INPUT_PULLUP);

  delay(1000);
}

void loop() {
  if (level == 1) {
    levelcreate(1, 4);
  }
  if (level == 2) {
    levelcreate(2, 5);
  }
  if (level == 3) {
    levelcreate(3, 5);
  }
  if (level == 4) {
    levelcreate(4, 5);
  }
  if (level == 5) {
    levelcreate(5, 5);
  }
  if (level == 6) {
    levelcreate(6, 4);
  }
}


void levelcreate(int levelset, int repetitions) {
  int levelsetsize = sizeof(letters[levelset-1]) / sizeof(int);
  Serial.println(String(levelsetsize));
  failed = false;
  for (int i = 0; i < repetitions && !failed; i++) {
    nonewanswer:
    int j = 0;
    memset(answers, -1, sizeof(answers));
    while (j != 4) {
      newsound = random(0, levelsetsize);
      Serial.println("Picking random number - " + letters[levelset][newsound]);
      if (!(newsound == answers[0] || newsound == answers[1] || newsound == answers[2])) {
        answers[j] = newsound;
        Serial.println("New number");
        j++;
      }
      delay(50);
    }
    Serial.print(String(answers[0]) + String(answers[1]) + String(answers[2]) + String(answers[3]));
    for (int k = 0; k < 50; k++) {
      correctanswer = random(0, 4);
      if ((previousanswers[0] == correctanswer || previousanswers[1] == correctanswer) || (previousanswers[2] == correctanswer || previousanswers[3] == correctanswer)) {
        goto nonewanswer;
      }
    }
    lcd.clear();
    lcd.print(answers[0]);
    lcd.setCursor(14, 0);
    lcd.print(answers[1]);
    lcd.setCursor(0, 1);
    lcd.print(answers[2]);
    lcd.setCursor(0, 14);
    lcd.print(answers[3]);
    Serial.println("Correct answer: " + correctanswer);
    while (digitalRead(button[1]) != HIGH && digitalRead(button[2]) != HIGH && digitalRead(button[3]) != HIGH && digitalRead(button[4]) != HIGH) {
      delay(1);
    }
    if (digitalRead(button[correctanswer])) {
      score++;
      previousanswers[i] = correctanswer;
      Serial.println("Correct");
    } else {
      lcd.clear();
      lcd.print("Score: " + score);
      lcd.setCursor(0, 1);
      lcd.print("High score: " + EEPROM.read(0));
      Serial.println("Incorrect");
      failed = true;
      delay(500);
    }
  }
  if (failed) {
    level = 1;
    level++;
  }
}

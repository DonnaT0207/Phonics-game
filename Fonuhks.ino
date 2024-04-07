#include <EEPROM.h>

#include <LiquidCrystal.h>


LiquidCrystal lcd(7, 6, 11, 10, 9, 8);

int level = 1;
char first[] = { 'a', 'e', 'i', 'o', 'u' };
char second[] = { 'b', 'c', 'd', 'f', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z' };
char third[] = { 'ch', 'ng', 'sh', 'th', 'wh' };
char fourth[] = { 'ay', 'ea', 'ie', 'oe', 'ue' };
char fifth[] = { 'air', 'ar', 'ear', 'er', 'or', 'ure' };
char sixth[] = { 'ci', 'ge', 'kn', 'wr' };
int sound;
int answers[4] = {-1};
int newsound;
int correctanswer;
const int button[4] = { 39, 37, 41, 33 };
bool failed = false;
int score = 0;
int previousanswers[5] = {-1};
const int levelnumfiles[] = { 5, 21, 5, 5, 6, 4 };

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("hello. starting.");

  pinMode(button[1], INPUT_PULLUP);
  pinMode(button[2], INPUT_PULLUP);
  pinMode(button[3], INPUT_PULLUP);
  pinMode(button[0], INPUT_PULLUP);

  delay(1000);
}

void loop() {
  if (level == 1) {
    levelcreate(first, 4);
  }
  if (level == 2) {
    levelcreate(second, 5);
  }
  if (level == 3) {
    levelcreate(third, 5);
  }
  if (level == 4) {
    levelcreate(fourth, 5);
  }
  if (level == 5) {
    levelcreate(fifth, 5);
  }
  if (level == 6) {
    levelcreate(sixth, 4);
  }
}


void levelcreate(char levelset[], int repetitions) {
  failed = false;
  for (int i = 0; i < repetitions && !failed; i++) {
    nonewanswer:
    int j = 0;
    memset(answers, -1, sizeof(answers));
    while (answers[3] == -1) {
      newsound = random(0, sizeof(levelset)+3);
      Serial.println("Picking random number" + String(levelset[newsound]));
      if ((newsound != answers[0] && newsound != answers[1] && newsound != answers[2])) {
        answers[j] = newsound;
        Serial.println("New number: " + String(levelset[newsound]));
        Serial.println("All numbers: " + String(levelset[answers[0]]) + String(levelset[answers[1]]) + String(levelset[answers[2]]) + String(levelset[answers[3]]));
        j++;
      }
      delay(50);
    }
    Serial.print(String(levelset[answers[0]]) + String(levelset[answers[1]]) + String(levelset[answers[2]]) + String(levelset[answers[3]]));
    for (int k = 0; k < 50; k++) {
      correctanswer = random(0, 4);
      Serial.println("New correct answer: " + String(levelset[correctanswer]));
      if (((previousanswers[0] == answers[correctanswer] || previousanswers[1] == answers[correctanswer]) || (previousanswers[2] == answers[correctanswer] || previousanswers[3] == answers[correctanswer]))) {
        if(k == 49){
          goto nonewanswer;
        }
      }else{
        break;
      }
    }
    lcd.clear();
    lcd.print(levelset[answers[0]]);
    lcd.setCursor(13, 0);
    lcd.print(levelset[answers[1]]);
    lcd.setCursor(0, 1);
    lcd.print(levelset[answers[2]]);
    lcd.setCursor(13, 1);
    lcd.print(levelset[answers[3]]);
    delay(10);
    Serial.println("Correct answer: " + String(levelset[answers[correctanswer]]));
    delay(1000);
    while (digitalRead(button[1]) == HIGH && digitalRead(button[2]) == HIGH && digitalRead(button[3]) == HIGH && digitalRead(button[0]) == HIGH) {
      delay(1);
    }
    if (!digitalRead(button[correctanswer])) {
      score++;
      previousanswers[i] = answers[correctanswer];
      Serial.println("Correct");
    } else {
      lcd.clear();
      lcd.print('Score: ' + String(score));
      lcd.setCursor(0, 1);
      lcd.print('High score: ' + String(EEPROM.read(0)));
      Serial.println("Incorrect");
      failed = true;
      delay(500);
    }
  }
  if (failed) {
    level = 1;
  }else {
    level++;
  }
}

#include <LiquidCrystal.h>
#include <Keypad.h>
#include "stdlib.h"
LiquidCrystal lcd(6, 5, 4, 3, 2, 1);
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char key, Status;
String e1, e2, e3, e4;
int HH = 0, MM = 0;
int H2[] = {8, 9, 10, 11, 11, 0, 1, 2, 3, 4, 5};
int M2[] = {30, 30, 30, 30, 45, 45, 45, 30, 30, 30, 30};
int H3[] = {10, 1, 2, 5};
int M3[] = {0, 0, 0, 0};
int H1[40], M1[40];
char state1[40], state2[] = {'A', 'A', 'A', 'A', 'A', 'P', 'P', 'P', 'P', 'P', 'P'}, state3[] = {'A', 'P', 'P', 'P'};
short int i = 0, Mode = 0, secs = 0, Day = 0, count = 0, flag = 1;
String day = "";
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'A', '0', 'e'}
};
byte like1[8] =
{
  0b00000,
  0b00000,
  0b00000,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00000,
};
byte like2[8] =
{
  0b00000,
  0b00100,
  0b01100,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11110,
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup() {
  lcd.begin(16, 2);
  lcd.createChar(4, like1);
  lcd.createChar(5, like2);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
E: lcd.clear();
  lcd.write("Mode1 or Mode2 ?");
  lcd.setCursor(0, 1);
  delay(1000);
  while (1) {
    delay(10);
    key = keypad.getKey();
    if (key) {
      lcd.write(key);
      delay(1000);
      if (key == '1') {
        Mode = 1;
        break;
      }
      else if (key == '2') {
        Mode = 2;
        input();
        break;
      }
      else {
        tryagain();
        goto E;
      }
    }
  }
  lcd.clear();
  lcd.write("If Sunday is 1");
  lcd.setCursor(0, 1);
  lcd.write("Then toDAY is");
  e1 = "Day";
  delay(4000);
  Day = key1(e1);
  lcd.clear();
  lcd.write("Please provide");
  lcd.setCursor(0, 1);
  lcd.write("Current Time");
  delay(4000);
  e2 = "Hour HH";
  HH = key1(e2);
  e3 = "Minute MM";
  MM = key1(e3);
  e4 = " AM or PM";
  Status = key1(e4);
  lcd.clear();
}
void loop() {
  delay(999);
  lcd.clear();
  secs++;
  if (flag == 1) {
    day = Days();
    flag = 0;
  }
  if (secs > 59) {
    secs = 0;
    count = 0;
    MM++;
    if (MM > 59) {
      MM = 0;
      HH++;
    }
  }
  if (HH > 11) {
    HH = 0;
    if (Status == 'A') {
      Status = 'P';
    }
    else if (Status == 'P') {
      Status = 'A';
      Day++;
      if (Day > 7)
        Day = 1;
      day = Days();
    }
  }
  lcd.print(day);
  lcd.setCursor(0, 1);
  if (HH / 10 == 0) {
    lcd.write("0");
  }
  lcd.print(HH);
  lcd.write(":");
  if (MM / 10 == 0) {
    lcd.write("0");
  }
  lcd.print(MM);
  lcd.write(":");
  if (secs / 10 == 0) {
    lcd.write("0");
  }
  lcd.print(secs);
  lcd.write(" ");
  lcd.write(Status);
  lcd.write("M");
  if (Mode == 1 && Day > 1 && Day < 7)
    routine();
  if (Mode == 1 && Day == 7)
    satroutine();
  if (Mode == 2)
    Sroutine();
}
int key1(String e) {
C: int i = 0;
  lcd.clear();
  lcd.write("Enter ");
  lcd.print(e);
  lcd.setCursor(0, 1);
  int L = 0;
  int count1 = 0;
  while (1) {
    key = keypad.getKey();
    delay(50);
    if (key) {
      delay(10);
      lcd.setCursor(i++, 1);
      if (key == 'e')
        break;
      lcd.write(key);
      delay(10);
      if (e == "Day") {
        L = L * 10 + (key - 48);
        break;
      }
      if (key == 'A') {
        delay (500);
        break;
      }
      L = L * 10 + (key - 48);
    }
  }
B: if ((L <= 12 && e == "Hour HH") || (L <= 59 && e == "Minute MM") || (L <= 7 && L > 0 && e == "Day")) {
    delay(1000);
    lcd.clear();
    lcd.print(e);
    lcd.write(" is");
    lcd.setCursor(0, 1);
    lcd.write("Received ");
    lcd.write(byte(4));
    lcd.write(byte(5));
    delay(1200);
    return L;
  }
  else if (key == 'A') {
    while (1) {
      key = keypad.getKey();
      delay(100);
      if (key) {
        if (key == 'A') {
          lcd.write(key);
          delay(1000);
          return 'P';
        }
        else
          return 'A';
      }
    }
  }
  else {
    tryagain();
    goto C;
  }
}
void tryagain() {
  lcd.clear();
  delay(100);
  lcd.write("Invalid input");
  lcd.setCursor(0, 1);
  lcd.write("Please try again");
  delay(1500);
}
void input() {
  int i, times = 0;
D:  lcd.clear();
  lcd.write("Total no. times");
  lcd.setCursor(0, 1);
  lcd.write("Bell should ring");
  delay (2000);
  lcd.clear();
  lcd.write("Enter Times");
  lcd.setCursor(0, 1);
  while (1) {
    delay(100);
    key = keypad.getKey();
    if (key) {
      if (key == 'e') {
        break;
      }
      if (key >= '1' && key <= '9') {
        lcd.write(key);
      }
      else {
        tryagain();
        goto D;
      }
      times = times * 10 + key - 48;
    }
  }
  if (times <= 0) {
    tryagain();
    goto D;
  }
  lcd.clear();
  lcd.write("Please provide");
  lcd.setCursor(0, 1);
  lcd.write("time in order");
  delay(1000);
  for (i = 0; i < times; i++) {
    lcd.clear();
    lcd.write("Enter alarm ");
    lcd.print(i + 1);
    delay(2000);
    e2 = "Hour HH";
    H1[i] = key1(e2);
    e3 = "Minute MM";
    M1[i] = key1(e3);
    e4 = "AM or PM";
    state1[i] = key1(e4);
  }
}
void routine() {
  for (i = 0; i < sizeof(H2); i++) {
    if (H2[i] == HH && M2[i] == MM && state2[i] == Status) {
      if (count++ <= 5) {
        digitalWrite(A0, HIGH);
        digitalWrite(A1, HIGH);
        digitalWrite(A2, HIGH);
        digitalWrite(A3, HIGH);
      }
      else {
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);
        digitalWrite(A2, LOW);
        digitalWrite(A3, LOW);
      }
    }
  }
}
void Sroutine() {
  for (i = 0; i < sizeof(H1); i++) {
    if (H1[i] == HH && M1[i] == MM && state1[i] == Status) {
      if (count++ <= 5) {
        digitalWrite(A0, HIGH);
        digitalWrite(A1, HIGH);
        digitalWrite(A2, HIGH);
        digitalWrite(A3, HIGH);
      }
      else {
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);
        digitalWrite(A2, LOW);
        digitalWrite(A3, LOW);
      }
    }
  }
}
void satroutine() {
  for (i = 0; i < sizeof(H3); i++) {
    if (H3[i] == HH && M3[i] == MM && state3[i] == Status) {
      if (count++ <= 5) {
        digitalWrite(A0, HIGH);
        digitalWrite(A3, HIGH);
      }
      else {
        digitalWrite(A0, LOW);
        digitalWrite(A3, LOW);
      }
    }
  }
}
String Days() {
  String day;
  switch (Day) {
    case 1: day = "Sunday";
      break;
    case 2: day = "Monday";
      break;
    case 3: day = "Tuesday";
      break;
    case 4: day = "Wednesday";
      break;
    case 5: day = "Thursday";
      break;
    case 6: day = "Friday";
      break;
    case 7: day = "Saturday";
      break;
  }
  return day;
}


#include <LiquidCrystal.h>
#include <NewPing.h>
#include <Keypad.h>
LiquidCrystal lcd(32, 30, 28, 26, 24, 22);
NewPing Ping(35, 33, 50);  //Ping(trigger,echo,distance);
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char key, e;
int cons = 0;
const int D1 = 38, D2 = 40, D3 = 42, D4 = 44;
float L1 = 0,L2 = 0, CC = 0;
float C15 = 0, P15 = 0;
bool flag1 = 1, flag2 = 1;
String Rpass = "ARDUINO";
String Number = "+919768626813";
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'.', '0', 'e'}
};
byte like1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00111,
  0b00111,
  0b00111,
  0b00111,
  0b00000,
};
byte like2[8] = {
  0b00000,
  0b00100,
  0b01100,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11110,
};
byte rupee[8] = {
  0b11111,
  0b00100,
  0b11111,
  0b01000,
  0b10000,
  0b01000,
  0b00100,
  0b00010
};
byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(9600);
  lcd.begin(16, 2);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  lcd.createChar(4, like1);
  lcd.createChar(5, like2);
  lcd.createChar(0, rupee);
  Serial1.println("AT+RST");
  Message_1();
    Serial2.println("AT+CMGD=1,4"); // delete all SMS
    delay(1000);
retry: e = '1';
  L1 = key1();
  e++;
 L2 = key1();
  Serial1.println("AT+CIPMUX=1");
  delay(100);
  lcd.clear();
  if (L1 >=L2 && L1 != 0) {
    lcd.write("Invalid Limits");
    delay(1000);
    goto retry;
  }
  else {
    RMessage_1();
    if (L1 >=L2) {
      trysim();
      goto retry;
    }
  }
}
void loop () {
  float val0 = analogRead(A0) * 5 / 1023.0;
  float val1 = analogRead(A1) * 5 / 1023.0;
  float val2 = analogRead(A2) * 5 / 1023.0;
  float val3 = analogRead(A3) * 5 / 1023.0;
  float C1 = 0, P1 = 0;
  P1 = val0 + val1 + val2 + val3;
  C1 = 0.2 * P1;
  CC = CC + C1;
  lcd.setCursor(0, 0);
  lcd.print("P=");
  lcd.print(P1);
  lcd.print("W");
  lcd.setCursor(0, 1);
  lcd.print("Cost=");
  lcd.write(byte(0));
  lcd.print(CC);
  Serial.print("P=");
  Serial.print(P1);
  Serial.print("Cost=");
  Serial.println(CC);
  C15 = C15 + C1;
  P15 = P15 + P1;
  if (cons++ % 15 == 1) {
    ESP(val0, val1, val2, val3, P15, C15);
    C15 = 0;
    P15 = 0;
  }
  if (L1 < CC && flag1 == 1) {
    Send();
    SendL1(CC);
    flag1 = 0;
    if (L2 < CC && flag2 == 1) {
      Send();
      SendL2(CC);
      flag2 = 0;
    }
  }
  int cm = Ping.ping_cm();
  if (cm > 10 || cm < 15) {
    Serial2.print("ATD ");
    Serial2.print(Number);
    Serial2.println(";");
  }
  RMessage();
  delay(900);
}
double key1() {
C:
  lcd.clear();
  lcd.write("Enter Limit ");
  lcd.write(e);
  lcd.setCursor(0, 1);
  short int i = 0, dpoint = 0;
  float tenths = 1;
  float L = 0;
  int count = 0;
  while (count++ < 40) {
    key = keypad.getKey();
    delay(100);
    if (key) {
      delay(100);
      lcd.setCursor(i++, 1);
      if (key == 'e')
        goto B;
      lcd.write(key);
      Serial.println(key);
      delay (100);
      if (key == '.') {
        count = 0;
        while (count++ < 50) {
          key = keypad.getKey();
          delay(100);
          if (key == '.') {
            tryagain();
            goto C;
          }
          if (key) {
            tenths *= 10;
            delay(100);
            lcd.setCursor(i++, 1);
            if (key == 'e')
              goto B;
            lcd.write(key);
            Serial.println(key);
            if (key == '0') {
              while (1) {
                delay(100);
                key = keypad.getKey();
                if (key) {
                  if (key == 'e') {
                    delay(100);
                    goto B;
                  }
                  if (key == '.') {
                    tryagain();
                    goto C;
                  }
                  lcd.write(key);
                  delay(1000);
                  tenths *= 10;
                  L = L + (float(key - 48)) / tenths;
                  goto B;
                }
              }
            }
            else {
              L = L + (float(key - 48)) / tenths;
              dpoint++;
              if (key == '.') {
                tryagain();
                goto C;
              }
              if (dpoint > 1) {
                delay(1000);
                goto B;
              }
            }
          }
        }
      }
      L = L * 10 + (key - 48);
    }
  }
B: Serial.println(L);
  if (L != 0) {
    lcd.clear();
    lcd.write("Limit ");
    lcd.write(e);
    lcd.write(" is");
    lcd.setCursor(0, 1);
    lcd.write("Received ");
    lcd.write(byte(4));
    lcd.write(byte(5));
    delay(1500);
  }
  return L;
}
void tryagain() {
  lcd.clear();
  delay(100);
  lcd.write("Invalid input");
  lcd.setCursor(0, 1);
  lcd.write("Please try again");
  delay(2000);
}
void Message_1() {
  Serial.println("First SMS Sent");
  Send();
  Serial2.println("Please give two limits as <SETL> <limit1>,<limit2>");
  delay(1000);
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(500);
}
void Send() {
  Serial.println("SMS Sending");
  Serial2.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);  // Delay of 1000 milli seconds or 1 second
  Serial2.print("AT+CMGS=\"");
  Serial2.print(Number);
  Serial2.println("\"\r"); // Replace x with mobile number
  delay(100);
}
void RMessage_1() {
  Receive();
  int count = 0;
  while (count++ < 50) {
    delay(100);
      if(Serial2.read() == 'S')
      if(Serial2.read() == 'E')
      if(Serial2.read() == 'T')
      if(Serial2.read() == 'L')
      Serial.println("Got Limits");
      L1 = Serial2.parseFloat();
      L2 = Serial2.parseFloat();
      Serial.println(L1);
      Serial.println(L2);
      break;
  }
}
void Receive() {
  Serial2.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(200);
}
void trysim() {
  Send();
  Serial2.println("Invalid Limits. Please try again");
  delay(100);
  Serial2.println((char)26);// ASCII code of CTRL+Z
  delay(100);
}
void RMessage() {
  Receive();
  int i = 0;
  char rchar;
  bool state;
  while (Serial2.available() > 0) {
    rchar = Serial2.read();
    if (rchar == 'D' || rchar == 'd') {
      switch (Serial2.read()) {
        case '1': state = Device();
          digitalWrite(D1, state);
          break;
        case '2': state = Device();
          digitalWrite(D2, state);
          break;
        case '3': state = Device();
          digitalWrite(D3, state);
          break;
        case '4': state = Device();
          digitalWrite(D4, state);
          break;
        default:;
      }
    }
    else {
      if (rchar == Rpass[i++]) {
        rchar = Serial.read();
        if (rchar == Rpass[i++]) {
          rchar = Serial.read();
          if (rchar == Rpass[i++]) {
            rchar = Serial.read();
            if (rchar == Rpass[i++]) {
              rchar = Serial.read();
              if (rchar == Rpass[i++]) {
                rchar = Serial.read();
                if (rchar == Rpass[i++]) {
                  rchar = Serial.read();
                  if (rchar == Rpass[i]) {
                    Serial.println("ACCESS GRANTED");
                    pinMode(4, OUTPUT);
                    digitalWrite(4, LOW);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
bool Device() {
  char ichar = Serial2.read();
  if (ichar == 'T' || ichar == 't') {
    return HIGH;
  }
  else {
    return LOW;
  }
}
void filter(char e1) {
  int i = 0;
  char c;
  String response = "";
  if (Serial1.find("field")) {
    for (i = 0; i < 4; i++) {
      delayMicroseconds(100);
      c = Serial1.read();
      response += c;
    }
    c = Serial1.read();
    Serial.println(c);
    if (c == '0')
      state(0, e1);
    else if (c == '1')
      state(1, e1);
    else if (c == 'u') {
      Serial.println(e1);
    }
  }
}
void state(int x, char e1) {
  switch (e1) {
    case '1' : digitalWrite(D1, x);
      Serial.print(e1);
      Serial.println(x);
      break;
    case '2' : digitalWrite(D2, x);
      Serial.print(e1);
      Serial.println(x);
      break;
    case '3' : digitalWrite(D3, x);
      Serial.print(e1);
      Serial.println(x);
      break;
    case '4' : digitalWrite(D4, x);
      Serial.print(e1);
      Serial.println(x);
      break;
    default:;
  }
}
void ESP(float val0,float val1,float val2,float val3,float P15,float C15) {
  String cmd0 = "AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80";
  String cmd1 = "AT+CIPSTART=1,\"TCP\",\"api.thingspeak.com\",80";
  String getStr0 = "GET https://api.thingspeak.com/update?api_key=I6F0Y9X8RUP6JATW&field1=";
  String getStr1 = "GET https://api.thingspeak.com/channels/128045/feeds.json?results=5";
  getStr1 += "\r\n";
  Serial1.println(cmd0);
  delay(1200);
  Serial.println(cmd0);
  cmd0 = "AT+CIPSEND=0,";
  cmd0 += String(getStr1.length());
  Serial.println(cmd0);
  Serial1.println(cmd0);
  bool j = Serial1.find(">");
  delay(100);
  if (j == 1) {
    Serial1.print(getStr1);
    delay(100);
    Serial.print(getStr1);
    if (Serial1.find("field4")) {
      for (int i = 0; i < 5; i++) {
        for (char c = '1'; c <= '4'; c++) {
          filter(c);
        }
      }
    }
  }
  Serial1.println(cmd1);
  Serial.println(cmd1);
  delay(200);
  getStr0 += String(val0);
  getStr0 += "&field2=";
  getStr0 += String(val1);
  getStr0 += "&field3=";
  getStr0 += String(val2);
  getStr0 += "&field4=";
  getStr0 += String(val3);
  getStr0 += "&field5=";
  getStr0 += String(P15);
  getStr0 += "&field6=";
  getStr0 += String(C15);
  getStr0 += "\r\n";
  delay(200);
  cmd0 = "AT+CIPSEND=1,";
  cmd0 += String(getStr0.length());
  Serial1.println(cmd0);
  Serial.println(cmd0);
  j = Serial1.find(">");
  delay(100);
  if (j) {
    Serial1.print(getStr0);
    Serial.print(getStr0);
  }
}
void SendL1(double CC) {
  Serial2.print("Bill=");
  delay(100);
  Serial2.print(CC);
  delay(100);
  Serial.println("Limit Reached");
  Serial2.print("/- Limit Reached");
  delay(100);
  Serial2.print("For Consumption Datalog please visit https://thingspeak.com/channels/147959");// The SMS text you want to send
  delay(500);
  Serial2.print((char)26);// ASCII code of CTRL+Z
  delay(10);
}
void SendL2(double CC) {
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  Serial.println("Second Limit Reached");
  Serial2.println("System Turned Off");
  delay(10);
  Serial2.print((char)26);// ASCII code of CTRL+Z
  delay(10);
}

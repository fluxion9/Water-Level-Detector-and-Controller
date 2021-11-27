#include <LiquidCrystal.h>
#define buzzer A0
#define dSet A5
#define hSet A4
#define relay 12
#define relay2 13
// #include <RH_ASK.h>
// #ifdef RH_HAVE_HARDWARE_SPI
// #include <SPI.h>
// #endif
String value;
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
float level;
bool bpSt;
class Blinker
{
    int ledPin;
    long onTime;
    long offTime;

    int ledState;
    unsigned long previousMillis;
    unsigned long currentMillis;
  public:
    Blinker(int pin, long on, long off)
    {
      ledPin = pin;
      pinMode(ledPin, OUTPUT);

      onTime = on;
      offTime = off;
      ledState = LOW;
      previousMillis = 0;
    }
    void Update()
    {
      currentMillis = millis();
      if ((ledState == HIGH) && (currentMillis - previousMillis >= onTime))
      {
        ledState = LOW;
        previousMillis = currentMillis;
        digitalWrite(ledPin, ledState);
      }
      else if ((ledState == LOW) && (currentMillis - previousMillis >= offTime))
      {
        ledState = HIGH;
        previousMillis = currentMillis;
        digitalWrite(ledPin, ledState);
      }
    }
};
//Blinker Click(relay2, 60000, 60000);

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  // if (!driver.init())
  // {
  //   ;
  // }
  Serial.begin(9600);
  pinMode(relay, 1);
  pinMode(8, 1);
  lcd.begin(16, 2);
  pinMode(buzzer, 1);
  digitalWrite(8, 1);
  digitalWrite(buzzer, 1);
  lcd.setCursor(0, 0);
  lcd.print("Level - Detector");
  delay(200);
  lcd.setCursor(0, 1);
  lcd.print("Initializing");
  for (int y = 0; y < 4; y++)
  {
    lcd.setCursor(12 + y, 1);
    lcd.print('.');
    delay(1000);
  }
  digitalWrite(buzzer, 0);
  lcd.clear();
}

void loop() {
  //Click.Update();
  float setDiameter = fmap(analogRead(dSet), 0, 1023.0, 0, 500.0);
  float setHeight = fmap(analogRead(hSet), 0, 1023.0, 0, 500.0);
  // uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  // uint8_t buflen = sizeof(buf);
  // if(driver.recv(buf, &buflen))
  // {
  //   for (int i = 0; i < buflen; i++)
  //   {
  //     value += char(buf[i]);
  //   }
  if (Serial.available())
  {
    while (Serial.available())
    {
      delay(3);
      char d = Serial.read();
      value += d;
    }
    Serial.println("Got:  " + String(value));
  }
  level = value.toFloat();
  Serial.println(level);
  lcd.setCursor(0, 0);
  lcd.print("H=" + String(setHeight) + ",D=" + String(setDiameter));
  float radius = setDiameter / 2.0;
  level = setHeight - level;
  level = PI * radius * radius * level;
  level /= 1000.0;
  lcd.setCursor(0, 1);
  lcd.print("Vol = " + String(level) + "ltrs");
  if (level >= (PI * radius * radius * setHeight))
  {
    if (!bpSt) {
      digitalWrite(buzzer, 1);
      delay(300);
      digitalWrite(buzzer, 0);
      lcd.setCursor(0, 1);
      lcd.print("Pump off         ");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.print("Vol = " + String(level) + "ltrs");
      delay(50);
      bpSt = true;
    }
    digitalWrite(relay, 0);
    digitalWrite(relay2, 1);
    delay(3000);
  }
  else if (level <= (PI * radius * radius * (setHeight / 2.0))) {
    if (bpSt)
    {
      digitalWrite(buzzer, 1);
      delay(300);
      digitalWrite(buzzer, 0);
      lcd.setCursor(0, 1);
      lcd.print("Pump on          ");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.print("Vol = " + String(level) + "ltrs");
      delay(50);
      bpSt = false;
    }
    digitalWrite(relay, 1);
    digitalWrite(relay2, 0);
  }
  value = "";
  lcd.setCursor(0, 0);
  lcd.print("H=" + String(setHeight) + ",D=" + String(setDiameter));
  lcd.setCursor(0, 1);
  lcd.print("Vol = " + String(level) + "ltrs");
  delay(10);
}

float fmap(float val, float fromLow, float fromHigh, float toLow, float toHigh)
{
  float norm = (val - fromLow) / (fromHigh - fromLow);
  float lerp = norm * (toHigh - toLow) + toLow;
  return lerp;
}

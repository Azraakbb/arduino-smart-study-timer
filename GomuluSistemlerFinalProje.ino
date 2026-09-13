#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
#include "MAX30100_PulseOximeter.h"

/* ---------------- Pin Tanımları ---------------- */
const int buzzerPin    = 9;
const int buttonPin    = 3;
const int ledDersPin   = 8;
const int ledMolaPin   = 7;
const int ledPulsePin  = 4;

LiquidCrystal_I2C lcd(0x27, 16, 2);
PulseOximeter pox;

/* ---------------- Durum Makinesi ---------------- */
enum State { IDLE, SELECT_DURATION, STUDY, WAIT_FOR_PULSE, SHOW_PULSE, BREAK };
volatile State currentState = IDLE;

/* ---------------- Süre Seçimi ---------------- */
const int sureSecenekleri[] = {10, 15, 20};
const byte secenekSayisi = sizeof(sureSecenekleri) / sizeof(sureSecenekleri[0]);
volatile byte seciliIndex = 0;
unsigned long secimBaslangic = 0;
const unsigned long secimTimeout = 3000; // 3 saniye

/* ---------------- Sayaçlar & Süreler ---------------- */
volatile int  dersSuresi   = 15;
volatile int  molaSure     = 10;
const unsigned long olcumSuresi = 15000;
unsigned long olcumBaslangic   = 0;
unsigned long sonNabizOkuma    = 0;

/* ---------------- Nabız İstatistikleri ---------------- */
int  nabizToplam = 0;
int  nabizAdet   = 0;
int  nabizOrt    = 0;
char stres[16]   = "Bekleniyor";

/* ---------------- Diğer Bayraklar ---------------- */
volatile bool buttonPressed = false;
volatile bool paused        = false;
volatile bool buzzerCal     = false;
unsigned long buzzerStart   = 0;
const unsigned long buzzerMs = 5000;

/* ---------------- Setup ---------------- */
void setup() {
  pinMode(buzzerPin,   OUTPUT);
  pinMode(buttonPin,   INPUT_PULLUP);
  pinMode(ledDersPin,  OUTPUT);
  pinMode(ledMolaPin,  OUTPUT);
  pinMode(ledPulsePin, OUTPUT);

  lcd.init();
  lcd.backlight();
  Serial.begin(115200);

  lcd.setCursor(0,0); lcd.print("   Merhaba!     ");
  lcd.setCursor(0,1); lcd.print(" Hazirlaniyor...");
  delay(2000);
  lcd.clear();

  if (!pox.begin()) {
    lcd.print("Sensor hatasi");
    while (1);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  lcd.setCursor(0,0); lcd.print("Hazir. Butona  ");
  lcd.setCursor(0,1); lcd.print("basin...       ");

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);

  Timer1.initialize(1000000); // 1 saniye
  Timer1.attachInterrupt(timerISR);
  Timer1.stop();

  digitalWrite(ledDersPin,  LOW);
  digitalWrite(ledMolaPin,  LOW);
  digitalWrite(ledPulsePin, LOW);
}

/* ---------------- Ana Döngü ---------------- */
void loop() {
  pox.update();

  if (buttonPressed) {
    buttonPressed = false;

    if (currentState == IDLE) {
      currentState = SELECT_DURATION;
      seciliIndex = 0;
      secimBaslangic = millis();
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("Süre Seçimi:");
      lcd.setCursor(0,1); lcd.print(sureSecenekleri[seciliIndex]); lcd.print(" saniye");
    }

    else if (currentState == SELECT_DURATION) {
      seciliIndex = (seciliIndex + 1) % secenekSayisi;
      secimBaslangic = millis();
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1); lcd.print(sureSecenekleri[seciliIndex]); lcd.print(" saniye");
    }

    else if (currentState == STUDY) {
      paused = !paused;
      lcd.setCursor(0,1);
      lcd.print(paused ? " Duraklatildi   " : " Devam ediliyor ");
    }
  }

  if (buzzerCal && millis() - buzzerStart < buzzerMs) {
    analogWrite(buzzerPin, 200);
  } else {
    analogWrite(buzzerPin, 0);
    buzzerCal = false;
  }

  if (currentState == WAIT_FOR_PULSE && millis() - sonNabizOkuma >= 250) {
    sonNabizOkuma = millis();
    int hr = pox.getHeartRate();
    if (hr > 0) {
      nabizToplam += hr;
      nabizAdet++;
    }
  }

  // Süre seçimi zaman aşımı ile dersi başlat
  if (currentState == SELECT_DURATION && millis() - secimBaslangic >= secimTimeout) {
    dersSuresi = sureSecenekleri[seciliIndex];
    currentState = STUDY;
    paused = false;

    lcd.clear();
    lcd.setCursor(0,0); lcd.print("  Ders basladi  ");
    digitalWrite(ledDersPin, HIGH);
    digitalWrite(ledMolaPin, LOW);
    digitalWrite(ledPulsePin, LOW);
    Timer1.start();
  }

  static unsigned long lcdTs = 0;
  if (millis() - lcdTs >= 500) {
    lcdTs = millis();
    lcdDisplay();
  }
}

/* ---------------- LCD Gösterimi ---------------- */
void lcdDisplay() {
  if (currentState == STUDY) {
    lcd.setCursor(0,0); lcd.print("  Ders zamani   ");
    lcd.setCursor(0,1);
    if (paused) lcd.print(" Duraklatildi   ");
    else {
      lcd.print(" Kalan: "); lcd.print(dersSuresi); lcd.print(" sn   ");
    }
  }
  else if (currentState == WAIT_FOR_PULSE) {
    lcd.setCursor(0,0); lcd.print(" Nabiz olculuyor ");
    lcd.setCursor(0,1); lcd.print("  Bekleyiniz...  ");
  }
  else if (currentState == SHOW_PULSE) {
    lcd.setCursor(0,0); lcd.print(" Nabiz: "); lcd.print(nabizOrt); lcd.print(" bpm ");
    lcd.setCursor(0,1); lcd.print(" Durum: "); lcd.print(stres);
  }
  else if (currentState == BREAK) {
    lcd.setCursor(0,0); lcd.print("  Mola zamani   ");
    lcd.setCursor(0,1); lcd.print(" Kalan: "); lcd.print(dersSuresi); lcd.print(" sn   ");
  }
  else if (currentState == IDLE) {
    lcd.setCursor(0,0); lcd.print(" Dersi Baslatmak ");
    lcd.setCursor(0,1); lcd.print("  icin Buton...  ");
  }
  else if (currentState == SELECT_DURATION) {
    lcd.setCursor(0,0); lcd.print("Süre Seçimi:");
    lcd.setCursor(0,1); lcd.print(sureSecenekleri[seciliIndex]); lcd.print(" saniye   ");
  }
}

/* ---------------- Zamanlayıcı Kesmesi ---------------- */
void timerISR() {
  if (currentState == STUDY && !paused) {
    if (--dersSuresi == 0) {
      currentState      = WAIT_FOR_PULSE;
      olcumBaslangic    = millis();
      nabizToplam       = nabizAdet = 0;

      digitalWrite(ledDersPin,  LOW);
      digitalWrite(ledPulsePin, HIGH);
    }
  }
  else if (currentState == WAIT_FOR_PULSE) {
    if (millis() - olcumBaslangic >= olcumSuresi) {
      nabizOrt = (nabizAdet ? nabizToplam / nabizAdet : 70);

      if (nabizOrt > 90)      { molaSure = 15;  strcpy(stres,"Yuksek"); }
      else if (nabizOrt > 70) { molaSure = 10;  strcpy(stres,"Orta  "); }
      else                    { molaSure = 5;   strcpy(stres,"Düşük"); }

      dersSuresi   = molaSure;
      currentState = SHOW_PULSE;

      digitalWrite(ledPulsePin, LOW);
      buzzerCal  = true;
      buzzerStart = millis();
    }
  }
  else if (currentState == SHOW_PULSE) {
    static byte cnt = 0;
    if (++cnt >= 3) {
      cnt = 0;
      currentState = BREAK;
      digitalWrite(ledMolaPin, HIGH);
      digitalWrite(ledDersPin, LOW);
    }
  }
  else if (currentState == BREAK) {
    if (--dersSuresi == 0) {
      currentState = IDLE;
      digitalWrite(ledMolaPin, LOW);
      Timer1.stop();
    }
  }
}

/* ---------------- Buton Kesmesi ---------------- */
void buttonISR() {
  static unsigned long prev = 0;
  if (millis() - prev > 200) buttonPressed = true;
  prev = millis();
}

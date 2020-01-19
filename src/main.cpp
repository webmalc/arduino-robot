#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "DHT.h"
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>

#define DHTPIN 4 
#define DHTTYPE DHT11
#define countof(a) (sizeof(a) / sizeof(a[0]))

RtcDS3231<TwoWire> Rtc(Wire);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,20,4);

/* Print date to the LCD display */
void print_date_lcd(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring, 
             countof(datestring),
             PSTR("%02u/%02u %02u:%02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  lcd.setCursor(0,1);
  lcd.print(datestring);
}

/* Setup the arduino */
void setup()
{
  lcd.init();
  dht.begin();
  Serial.begin(9600);

  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid()) 
  {
      if (Rtc.LastError() != 0)
      {
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
      }
      else
      {
          Serial.println("RTC lost confidence in the DateTime!");
          Rtc.SetDateTime(compiled);
      }
  }

  if (!Rtc.GetIsRunning())
  {
      Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
  {
      Rtc.SetDateTime(compiled);
  }
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
}


/* Print temperature to the LCD display */
void print_lcd(float t, float h) {
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("t ");
  lcd.setCursor(2,0);
  lcd.print(t);

  lcd.setCursor(8,0);
  lcd.print("h ");
  lcd.setCursor(10,0);
  lcd.print(h);
}

/* Get and print temperature to the LCD display */
void print_temperature() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    print_lcd(0.00, 0.00);
    return;
  }
  print_lcd(t, h);
}

/* Get and print date to the LCD display */
void print_date() {
  if (!Rtc.IsDateTimeValid()) 
    {
      if (Rtc.LastError() != 0)
        {
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
        }
      else
        {
          Serial.println("RTC lost confidence in the DateTime!");
        }
    }

  RtcDateTime now = Rtc.GetDateTime();
  print_date_lcd(now);
}

/* The main loop */
void loop() {
  print_temperature();
  print_date();

  delay(1000);
}



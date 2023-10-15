#include <legopowerfunctions.h>
#include "DFRobot_ST7687S_Latch.h"
#include "DFRobot_Display_Clock.h"
#include "SPI.h"

#ifdef __AVR__
uint8_t pin_cs = 3, pin_rs = 5, pin_wr = 6, pin_lck = 7;
#else
uint8_t pin_cs = D3, pin_rs = D5, pin_wr = D6, pin_lck = D7;
#endif

#define MAX_SPEED 160
#define TRAM_NAME "PESA"
#define TRAIN_NAME "IC 41104"

DFRobot_ST7687S_Latch tft(pin_cs, pin_rs, pin_wr, pin_lck);
LEGOPowerFunctions pf(10);

int nUpPin = 45;
int nDownPin = 43;

int nSpeed = 0;
int nNewSpeed = 0;
int nLegoSpeed = 0;
int nLastChanged = 0;
int nCurrentTime = 0;
char sTramMode[] = "  POSTOJ ";

void updateTrainSpeed(int nSpeed) {
  if (nSpeed == 0)
    nLegoSpeed = 0;
  else
    nLegoSpeed = (nSpeed / MAX_SPEED) * 6 + 1;
    
  pf.SingleOutput(0, nLegoSpeed, RED, CH1);
}

void setup(void)
{
  Serial.begin(115200);
  
  pinMode(nUpPin, INPUT);
  pinMode(nDownPin, INPUT);
    
  tft.begin();
  tft.fillScreen(DISPLAY_BLACK);
  tft.setTextColor(DISPLAY_WHITE);
  tft.setTextBackground(DISPLAY_BLACK);
  tft.setTextSize(3);
  tft.setCursor(tft.width / 2 - 35, tft.height / 2 - 10);
  tft.print(TRAM_NAME);
  tft.setTextSize(1);
  tft.setCursor(tft.width / 2 - 27, tft.height / 2 + 20);
  tft.print("BYDGOSZCZ");
  tft.setTextSize(4);

  delay(5000);
  tft.fillScreen(DISPLAY_BLACK);

  tft.setTextSize(4);
  tft.setCursor(tft.width /2 - 25, tft.height / 2 - 12);
  tft.print(" ");
  tft.print(nSpeed);
  tft.setTextSize(1);
  tft.print("km/h");

  tft.setCursor(tft.width / 2 - 25, tft.height / 2 + 30);
  tft.print(sTramMode);
  tft.setCursor(tft.width / 2 - 40, tft.height / 2 - 30);
  tft.print("IC ONDRASZEK");
  
  tft.setTextSize(4);

  nLastChanged = millis();
}

void loop(void)
{
  nCurrentTime = millis();
  
  if (digitalRead(nUpPin) == HIGH)
  {
    if (nSpeed < MAX_SPEED) {
      nNewSpeed = nSpeed + 2;
      nLastChanged = nCurrentTime;
      strcpy(sTramMode, " ROZRUCH ");
    }
  }
  else if (digitalRead(nDownPin) == HIGH)
  {
    if (nSpeed > 0) {
      nNewSpeed = nSpeed - 2;
      nLastChanged = nCurrentTime; 
      strcpy(sTramMode, "HAMOWANIE");
    }
    else
      updateTrainSpeed(0);
  }
  /* else {
    if ((nCurrentTime - nLastChanged > 3000) && (nSpeed > 0)) {
      nNewSpeed = nSpeed - 1;
      nLastChanged = nCurrentTime;
      strcpy(sTramMode, "  JAZDA  ");
    }
  }
 */
  if (nNewSpeed != nSpeed) {
    updateTrainSpeed(nNewSpeed);
    
    if (nNewSpeed == 0)
      strcpy(sTramMode, "  POSTOJ ");
      
    nSpeed = nNewSpeed;
    

    tft.setCursor(tft.width /2 - 40, tft.height / 2 - 12);

    // Add leading spaces
    if (nSpeed < 10)
      tft.print("  ");
      else if (nSpeed < 100)
      tft.print(" ");
      
    if (nSpeed < (MAX_SPEED - 10))
      tft.setTextColor(DISPLAY_WHITE);
    else if (nSpeed < MAX_SPEED)
      tft.setTextColor(DISPLAY_YELLOW);
    else
      tft.setTextColor(DISPLAY_RED);
    tft.print(nSpeed);
    tft.setTextSize(1);
    // tft.setTextColor(DISPLAY_WHITE);
    tft.print("km/h");
    
    tft.setCursor(tft.width / 2 - 25, tft.height / 2 + 30);
    tft.print(sTramMode);
    tft.setTextSize(4);
  } else if (strcmp(sTramMode, "  JAZDA  ") && (nSpeed > 0)) {
    strcpy(sTramMode, "  JAZDA  ");
    tft.setTextSize(1);
    tft.setCursor(tft.width / 2 - 25, tft.height / 2 + 30);
    tft.print(sTramMode);
    tft.setTextSize(4);
  }  
    
}

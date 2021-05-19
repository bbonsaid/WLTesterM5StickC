// WLTesterM5StickC bbd

#include <M5StickC.h>
#include "AXP192.h"

#define SECONDS(s)    ((s) * 1000)
#define MINUTES(m)    SECONDS(m * 60)

const int WAIT_TIME = SECONDS(1);
const uint8_t WL_PIN = 32;  // 水分計センサ Gloveコネクタ利用
float minWl = 100.0;
float maxWl = 0.0;

//GND HY2.0-4P(GROVE):GND
//5V  HY2.0-4P(GROVE):VCC
//G32 HY2.0-4P(GROVE):AOUT

float
GetWl(void)
{
  int v = analogRead(WL_PIN);
//Serial.printf("ML(%d):%8.3f\r\n", i, v);
  return ((4096.0 - v) / 4096.0) * 100.0;
}

void
PrintWl(void)
{
  float wl = GetWl();
//充電％を求める。
//https://make-muda.net/2019/09/6946/
//参考にした。
// バッテリー残量表示
// 簡易的に、線形で4.2Vで100%、3.0Vで0%とする
  float batV = M5.Axp.GetVbatData() * 1.1 / 1000;
  float batP = (batV - 3.0) / 1.2 * 100.0;
  batP = (batP > 100.0) ? 100.0 : ((batP < 0.0) ? 0.0 : batP);

  minWl = (minWl < wl) ? minWl : wl;
  maxWl = (wl < maxWl) ? maxWl : wl;

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf(" WL:%.1f\n", wl);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf(" S:%.1f\n", minWl);
  M5.Lcd.printf(" L:%.1f\n", maxWl);
  M5.Lcd.printf("    Bat:%.1f\n", batP);
  Serial.printf("WL:%.1f S:%.1f L:%.1f Bat:%.1f", wl, minWl, maxWl, batP);
}

void
setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor(0, 0);
//  Serial.begin(115200);
}

void
loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    minWl = 100.0;
    maxWl = 0.0;
  }
  PrintWl();
  delay(WAIT_TIME);
}

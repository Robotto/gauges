#include <Adafruit_NeoPixel.h>

static int rocksteadyGauge=4; 
static int rocksteadyPixelPin=15;
static int sardukarGauge=5;
static int sardukarPixelPin=13;
static int delayms=50;

Adafruit_NeoPixel rocksteadyPixel = Adafruit_NeoPixel(1, rocksteadyPixelPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel sardukarPixel = Adafruit_NeoPixel(1, sardukarPixelPin, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);
  rocksteadyPixel.begin();
  sardukarPixel.begin();
  rocksteadyPixel.setPixelColor(0, 0x00FF00);
  sardukarPixel.setPixelColor(0, 0x00FF00);
  rocksteadyPixel.show();
  sardukarPixel.show();
}

void loop() {
  Serial.println("-------------");
  
  int percent=0;

  for (percent = 0; percent <= 100; percent += 1) { // goes from 0% to 100%
    setRocksteady(percent);
    setSardukar(100-percent);
    delay(delayms);
  }
  for (percent = 100; percent >= 0; percent -= 1) { // goes from 100% to 0%
    setRocksteady(percent);
    setSardukar(100-percent);
    delay(delayms);
  }
}


void setSardukar(int percentage)
{
    int pwm=(int)((float)percentage/100.0*(float)PWMRANGE);
    uint8_t red=(uint8_t)((float)255.0*(float)percentage/100);
    uint8_t green=(uint8_t)((float)255.0*(100.0-(float)percentage)/100);
    
    Serial.print("Sardukar: "); Serial.print(percentage);
    Serial.print("%, "); Serial.print("PWM:"); Serial.print(pwm);
    Serial.print(", R: "); Serial.print(red); Serial.print(", G:"); Serial.print(green); 
    Serial.println();
    analogWrite(sardukarGauge,pwm);
    sardukarPixel.setPixelColor(0,red,green,0);
    sardukarPixel.show();

}

void setRocksteady(int percentage)
{
    int pwm=(int)((float)percentage/100.0*(float)PWMRANGE);
    uint8_t red=(uint8_t)((float)255.0*(float)percentage/100);
    uint8_t green=(uint8_t)((float)255.0*(100.0-(float)percentage)/100);
    
    Serial.print("Rocksteady: "); Serial.print(percentage);
    Serial.print("%, "); Serial.print("PWM:"); Serial.print(pwm);
    Serial.print(", R: "); Serial.print(red); Serial.print(", G:"); Serial.print(green); 
    Serial.println();
    
    analogWrite(rocksteadyGauge,pwm);
    rocksteadyPixel.setPixelColor(0,red,green,0);
    rocksteadyPixel.show();

}
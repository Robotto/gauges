#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

const char* ssid     = "nope";
const char* password = "nooope";

const char* host = "192.168.0.5"; //could be global, but this is just lan
//const char* host = "192.168.1.107"; //Test lan.
const int hostPort = 9999;

const int rocksteadyGauge=4; 
const int rocksteadyPixelPin=15;
const int sardukarGauge=5;
const int sardukarPixelPin=13;

const int loopDelay=1000*60*10; //every 10 minutes
int lastGet=0;

volatile int sardkarPercentage=50;
volatile int rocksteadyPercentage=50;

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

  //First run after 5 seconds:
  delay(5000);
  getIt();
  setSardukar(sardkarPercentage);
  setRocksteady(rocksteadyPercentage);

}

void loop() {

 if(millis()>lastGet+loopDelay) {
  	Serial.println("Getting data!");
  	lastGet=millis();
  	getIt();

    setSardukar(sardkarPercentage);
    setRocksteady(rocksteadyPercentage);
    }
  }

void getIt()
{
  int wifiTries=0;

  // Use WiFiClient class to create TCP connections
  WiFiClient client;


  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    wifiTries++;
    Serial.print(".");
    if(wifiTries==60) { //after 30 seconds of trying to connect.
    	Serial.println("WiFi connection failed.. will retry next round.");
    	WiFi.disconnect();
    	return;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, hostPort)) {
    Serial.println("connection failed");
    return;
  }

  delay(1000); //plenty of time for local server to respond

  if(client.available()>4){ //when more than one percentage has been received
    Serial.print("Before: S: "); Serial.print(sardkarPercentage); Serial.print(", R: ");Serial.println(rocksteadyPercentage);
    sardkarPercentage=client.parseInt();
    rocksteadyPercentage=client.parseInt();
    Serial.print("After: S: ");  Serial.print(sardkarPercentage); Serial.print(", R: ");Serial.println(rocksteadyPercentage);
    }

   Serial.println();
   Serial.println("disconnecting.");
   client.stop();

   WiFi.disconnect();

}

void setSardukar(int percentage)
{
    static int oldPercentage;
    if(percentage==oldPercentage) {Serial.println("Sardukar: No change."); return;} //no change -> Do nothing

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
    oldPercentage=percentage;
}

void setRocksteady(int percentage)
{
    static int oldPercentage;
    if(percentage==oldPercentage) {Serial.println("Rocksteady: No change."); return;} //no change -> Do nothing


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
    oldPercentage=percentage;
}
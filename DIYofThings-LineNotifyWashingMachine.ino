#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// library ส่ง line ฝีมือคนไทย 
// https://github.com/TridentTD/TridentTD_LineNotify
#include <TridentTD_LineNotify.h>

// กำหนด line token 
#define LINE_TOKEN  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

// กำหนด Blynk Config
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char ssid[] = "xxxxx";
char pass[] = "xxxxx";

WidgetLED led_status(V1);
BlynkTimer timer;
int washingMonitor;

BLYNK_WRITE(V0)
{
  if(param.asInt())
  {
    if(!timer.isEnabled(washingMonitor))
    {
      timer.enable(washingMonitor);
    }
  }
  else
  {
    timer.disable(washingMonitor);
    led_status.off();
    digitalWrite(D0,HIGH);
  }
}

void check_washingStatus()
{
  if(analogRead(A0) < 900)
  {
    led_status.on();
    digitalWrite(D0,LOW);
  }
  else
  {
    led_status.off();
    digitalWrite(D0,HIGH);
    Blynk.virtualWrite(V0,0);
    LINE.notifySticker("ซักผ้าเสร็จแล้วจร้าาา ไปตากด้วยนะ ^^ ", 1, 2);
    timer.disable(washingMonitor);
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  pinMode(D0,OUTPUT);
  digitalWrite(D0,HIGH);
  
  Blynk.begin(auth, ssid, pass);
  LINE.setToken(LINE_TOKEN);
  washingMonitor = timer.setInterval(1000L,check_washingStatus);
  timer.disable(washingMonitor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

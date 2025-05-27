/*
  Author: Mike Tremblay

  Arduino Nano ESP 32 sketch using HomeSpan to control an HDMI switch's pushbuttons through HomeKit.

  https://github.com/HomeSpan/HomeSpan
*/


#include <HomeSpan.h>

#define HDMI1PIN 18 // D9
#define HDMI2PIN 21 // D10
#define HDMI3PIN 38 // D11
#define HDMI4PIN 47 // D12



struct SmartSwitch : Service::Television {
  SpanCharacteristic *active = new Characteristic::Active(1);
  SpanCharacteristic *activeID = new Characteristic::ActiveIdentifier(1);

  SmartSwitch(const char *name) : Service::Television() {
    new Characteristic::ConfiguredName(name);
    Serial.printf("Configured Switch: %s\n",name);
  }

  boolean update() override {
    if(active->updated()){
      Serial.printf("Set Switch Power to: %s\n",active->getNewVal()?"ON":"OFF");
    }

    if(activeID->updated()){
      Serial.printf("Set input to HDMI-%d\n",activeID->getNewVal());
      switch(activeID->getNewVal()){
      case 1:
        digitalWrite(HDMI1PIN, LOW);
        digitalWrite(HDMI2PIN, HIGH);
        digitalWrite(HDMI3PIN, HIGH);
        digitalWrite(HDMI4PIN, HIGH);
        break;

      case 2:
        digitalWrite(HDMI1PIN, HIGH);
        digitalWrite(HDMI2PIN, LOW);
        digitalWrite(HDMI3PIN, HIGH);
        digitalWrite(HDMI4PIN, HIGH);
        break;

      case 3:
        digitalWrite(HDMI1PIN, HIGH);
        digitalWrite(HDMI2PIN, HIGH);
        digitalWrite(HDMI3PIN, LOW);
        digitalWrite(HDMI4PIN, HIGH);
        break;

      case 4:
        digitalWrite(HDMI1PIN, HIGH);
        digitalWrite(HDMI2PIN, HIGH);
        digitalWrite(HDMI3PIN, HIGH);
        digitalWrite(HDMI4PIN, LOW);
        break;
      }
    }
    return(true);
  }
};

void setup() {
  // Start serial
  Serial.begin(115200);
 
  // Set up the pins
  pinMode(HDMI1PIN, OUTPUT);
  pinMode(HDMI2PIN, OUTPUT);
  pinMode(HDMI3PIN, OUTPUT);
  pinMode(HDMI4PIN, OUTPUT);
 
 // HomeSpan

 homeSpan.begin(Category::Television, "SmartSwitch HDMI");

 SPAN_ACCESSORY();

 SpanService *hdmi1 = new Service::InputSource();
  new Characteristic::ConfiguredName("HDMI 1");
  new Characteristic::Identifier(1);
  new Characteristic::IsConfigured(1);
  new Characteristic::CurrentVisibilityState(0);

 SpanService *hdmi2 = new Service::InputSource();
  new Characteristic::ConfiguredName("HDMI 2");
  new Characteristic::Identifier(2);
  new Characteristic::IsConfigured(1);
  new Characteristic::CurrentVisibilityState(0);

SpanService *hdmi3 = new Service::InputSource();
  new Characteristic::ConfiguredName("HDMI 3");
  new Characteristic::Identifier(3);
  new Characteristic::IsConfigured(1);
  new Characteristic::CurrentVisibilityState(0);

SpanService *hdmi4 = new Service::InputSource();
  new Characteristic::ConfiguredName("HDMI 4");
  new Characteristic::Identifier(4);
  new Characteristic::IsConfigured(1);
  new Characteristic::CurrentVisibilityState(0);


(new SmartSwitch("SmartSwitch HDMI"))
  ->addLink(hdmi1)
  ->addLink(hdmi2)
  ->addLink(hdmi3)
  ->addLink(hdmi4)
  ; 
}

void loop() {
  homeSpan.poll();
  }
#include <Arduino.h>
#include <BLEMidi.h>
#include "UsbMidi.h"

UsbMidi usbMidi;

void setup() {
  Serial.begin(115200);

  BLEMidiServer.begin("GP5 MIDI Device");

  BLEMidiServer.setOnConnectCallback([]() {
    Serial.println("BLE MIDI device connected");
  });

  BLEMidiServer.setOnDisconnectCallback([]() {
    Serial.println("BLE MIDI device disconnected");
  });

  BLEMidiServer.setControlChangeCallback([](uint8_t channel, uint8_t controller, uint8_t value, uint16_t) {
    Serial.printf("Received control change: channel %d, controller %d, value %d\n", channel, controller, value);
    usbMidi.controlChange(0, controller, value);
  });

  BLEMidiServer.setProgramChangeCallback([](uint8_t channel, uint8_t program, uint16_t){
    Serial.printf("Received program change: channel %d, program %d\n", channel, program);
    usbMidi.controlChange(0, 0, program);
  });

  //BLEMidiServer.enableDebugging();

  usbMidi.onDeviceConnected([](){
    Serial.println("USB MIDI device connected: ");
    usbMidi.printDeviceInfo();
  });

  usbMidi.onDeviceDisconnected([](){
    Serial.println("USB MIDI device disconnected");
  });

  usbMidi.begin();  
}

void loop() {
  usbMidi.update();
}

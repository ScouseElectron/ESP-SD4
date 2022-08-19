/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* MAIN.CPP 3/1/19 v1.1 */

/* QH4 */

#include "main.h"

char apssid[] PROGMEM = "ESP-SD4";
char hostname[] PROGMEM = "esp-sd4.local";

uint32_t aptimer;
bool apactive = 1;

WiFiEventHandler wifi_disconnected, wifi_connected;

void setup(){
  #if DEBUG == 1
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);
  delay(400);
  Serial.println(F("\r\nStarting\r\n"));
  #endif
  if (!MountDev()){
    while(1){
      delay(5);
    }
  }
  WiFi.mode(WIFI_AP_STA);
  ConfigInit();
  if (!config->dhcp){
    IPAddress ipaddr, gateway, netmask, dns1, dns2;
    ipaddr.fromString(config->ipaddr);
    gateway.fromString(config->gateway);
    netmask.fromString(config->netmask);
    dns1.fromString(config->dns1);
    dns2.fromString(config->dns2);
    WiFi.config(ipaddr, gateway, netmask, dns1, dns2);
  }
  WiFi.hostname(FPSTR(hostname));
  WiFi.softAP(FPSTR(apssid), config->www_password);
  WiFi.begin(config->wifissid, config->wifipass);
  WiFi.setAutoReconnect(1);
  wifi_disconnected = WiFi.onStationModeDisconnected(wifi_dis_cb);
  wifi_connected = WiFi.onStationModeGotIP(wifi_con_cb);
  httpservInit();
  sdInit();
  aptimer = millis();
}

void loop(){
  sdRun();
  if (apactive){
    if (millis() - aptimer > APTIMEOUT){
      apactive = 0;
      WiFi.softAPdisconnect(1);
    }
  }
}

void wifi_dis_cb(const WiFiEventStationModeDisconnected& event){
  #if DEBUG == 1
  Serial.println(F("Station disconnected"));
  #endif
}

void wifi_con_cb(const WiFiEventStationModeGotIP& event){
  #if DEBUG == 1
  Serial.print(F("Station connected IP: "));
  Serial.print(WiFi.localIP());
  Serial.print(F(" Channel: "));
  Serial.println(WiFi.channel());
  #endif
}


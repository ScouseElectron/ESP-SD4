/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* CONFIG.H 3/1/19 v1.1 */

/* QH4 */

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

#define MAX_SSID    32
#define MAX_PASS    64
#define MAX_STR     64
#define MAX_URL     120
#define MAX_IP4     16
#define MAX_IP6     40

struct w_config{
  char ipaddr[MAX_IP4];
  char gateway[MAX_IP4];
  char netmask[MAX_IP4];
  char dns1[MAX_IP4];
  char dns2[MAX_IP4];
  //char ipaddr6[MAX_IP6];
  //char gateway6[MAX_IP6];
  //char subnet6[MAX_IP6];
  //char dns16[MAX_IP6];
  //char dns26[MAX_IP6];
  char wifissid[MAX_SSID+1];
  char wifipass[MAX_PASS+1];
  uint8_t wifienc;
  char www_password[MAX_PASS+1];
  bool dhcp;
};

extern w_config *config;

void ConfigInit(void);
void ConfigDefault(void);
bool ConfigSave(void);
bool ConfigLoad(void);
bool MountDev(void);
bool ConfigReset(void);


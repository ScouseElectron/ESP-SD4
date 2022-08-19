/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* CONFIG.CPP 3/1/19 v1.1 */

/* QH4 */

#include "config.h"

w_config *config;

void ConfigInit(){
  config = new w_config;
  #if REGEN == 1
  ConfigDefault();
  ConfigSave();
  #endif
  if (!ConfigLoad()){
    #if DEBUG == 1
    Serial.println(F("WARNING: No configuration file found, creating default file."));
    #endif
    ConfigDefault();
    ConfigSave();
  } else {
    #if DEBUG == 1
    Serial.println(F("Loaded configuration file."));
    #endif
  }
}

void ConfigDefault(){
  config->dhcp = true;
  sprintf(config->wifissid, PSTR("ETSTRT4"));
  sprintf(config->wifipass, PSTR("246e78818c60264ff19e285068a53d53"));
  config->wifienc = AUTH_WPA2_PSK;
  sprintf(config->www_password, PSTR("password"));
  sprintf(config->ipaddr, PSTR("192.168.1.10"));
  sprintf(config->netmask, PSTR("255.255.255.0"));
  sprintf(config->gateway, PSTR("192.168.1.254"));
  sprintf(config->dns1, PSTR("192.168.1.254"));
  sprintf(config->dns2, PSTR("0.0.0.0"));
}

bool ConfigSave(){
  File w = LittleFS.open(F("/config"), "w");
  size_t t = w.write((uint8_t*)config, sizeof(w_config));
  w.close();
  #if DEBUG == 1
  Serial.print(F("Saved configuration file: "));
  Serial.print(t);
  Serial.println(F(" bytes written."));
  #endif
  return true;
}

bool ConfigReset(){
  return LittleFS.remove(F("/config"));
}

bool ConfigLoad(){
  File w;
  w = LittleFS.open(F("/config"), "r");
  if (!w) return false;
  w.read((uint8_t*)config, sizeof(w_config));
  return true;
}

bool MountDev(){
  if (!LittleFS.begin()){
    #if DEBUG == 1
    Serial.println(F("FATAL: Failed to initialize flash file system."));
    #endif
    return false;
  } else {
    #if DEBUG == 1
    Serial.println(F("Mounted file system ok."));
    #endif
  }
  return true;
}

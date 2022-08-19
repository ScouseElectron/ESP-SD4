/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* VALIDATE.CPP 22/4/18 v2.3 */

/* QH2 */

#include "validate.h"

bool CheckIPAddr4(String a){
  IPAddress x;
  return x.fromString(a);
}

bool checkwifidetailes(const String ssid, const String pass){
  if (ssid.length() < 4 || ssid.length() > MAX_SSID || pass.length() < 4 || pass.length() > MAX_PASS){
    return false;
  }
  if (!CheckString(ssid) || !CheckString(pass)){
    return false;
  }
  return true;
}

bool CheckChar(char c){
 if (c < 32 || c > 127){
  return false;
 }
 
 if (c >= 'A' && c <= 'Z'){
  return true;
 }
 
 if (c >= 'a' && c <= 'z'){
  return true;
 }

 if (c >= '0' && c <= '9'){
  return true;
 }

 if (c == '/' || c == ':' || c == '.' || c == '_' || c == '-' || c == '@' || c == ' ' || c == '#' || c == '$'){
  return true;
 }
 return false;
}

bool CheckNumChar(char c){
 if (c >= '0' && c <= '9'){
  return true;
 }

 if (c == '.' || c == '-'){
  return true;
 }
 return false;
}

bool CheckString(String s){
  uint8_t t = s.length();
  if (t == 0) return false;
  for(uint8_t i=0;i<t;i++){
    if (!CheckChar(s[i])){
      return false;
    }
  }
  return true;
}

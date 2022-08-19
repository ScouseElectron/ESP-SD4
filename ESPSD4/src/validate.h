/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* VALIDATE.H 22/4/18 v2.3 */

/* QH2 */

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"

//char _cip_[] = PSTR("/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/");

bool CheckIPAddr4(String);
bool checkwifidetailes(const String, const String);
bool CheckChar(char);
bool CheckNumChar(char);
bool CheckString(String);

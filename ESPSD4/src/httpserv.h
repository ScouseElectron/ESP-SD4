/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* HTTPSERV.H 10/8/22 v1.2 */

/* QH4 */

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "validate.h"
#include "config.h"

#define HTTP_PORT 80

extern AsyncWebServer *server;

void httpservInit(void);
void notfound(AsyncWebServerRequest*);
void mainpage(AsyncWebServerRequest*);
void settingsjson(AsyncWebServerRequest*);
void updatesettings(AsyncWebServerRequest*);
void changepassword(AsyncWebServerRequest*);
void favico(AsyncWebServerRequest*);
void configreset(AsyncWebServerRequest*);

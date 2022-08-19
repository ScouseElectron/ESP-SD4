/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* STREAMDECK.H 19/8/22 v1.1 */

/* QH4 */

#pragma once

#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "httpserv.h"
#include <ArduinoJson.h>

#define LED_OUT 3     // RXD
#define PUSH_BUTTON 2 // LED

#define JSON_RECEIVE_BUFFER 480
#define DEFAULT_LED_LEVEL 10
#define SWITCH_DEBOUNCE 50
#define BUTTON_DOWN_TIME 800
#define DIMMER_INTERVAL 10

extern AsyncWebSocket *ws;
extern bool ledstatus;
extern uint8_t ledbrightness;

void sdInit(void);
void onEvent(AsyncWebSocket*, AsyncWebSocketClient*, AwsEventType, void*, uint8_t*, size_t);
void sdRun(void);
void wsMessage(void*, uint8_t*, size_t);
void ReadButton(void);
void updateButton(char*, uint8_t);
void PushButUp(void);
void PushButDown(void);
void Dimmmer(void);
void SetLedVal(int);
uint8_t GetLevel(void);

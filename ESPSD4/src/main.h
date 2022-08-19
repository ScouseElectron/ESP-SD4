/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* MAIN.H 3/1/19 v1.1 */

/* QH4 */

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "httpserv.h"
#include "streamdeck.h"

#define APTIMEOUT 900000

void wifi_con_cb(const WiFiEventStationModeGotIP&);
void wifi_dis_cb(const WiFiEventStationModeDisconnected&);

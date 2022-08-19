/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* STREAMDECK.CPP 19/8/22 v1.1 */

/* QH4 */

#include "streamdeck.h"

AsyncWebSocket *ws;

bool ledstatus = 0;
uint8_t ledbrightness = 127;
bool ledDimDirection;
bool ledIsDimming;
char but1_context[33] = {0};

bool pushButtonState;
bool pushButtonDown = 0;
bool pushButtonUp = 0;
bool pushButtonTimer = 0;

uint32_t butDownTimer, ledDimTimer, switchDebounce;

StaticJsonDocument<JSON_RECEIVE_BUFFER> doc;

void PushButUp(){
  if (!pushButtonUp){
    return;
  }
  pushButtonUp = 0;
  pushButtonDown = 0;
  pushButtonTimer = 0;
  if (!ledstatus){
    ledstatus = 1;
    analogWrite(LED_OUT, ledbrightness);
    updateButton(but1_context, ledstatus);
    return;
  }
  if (!ledIsDimming){
    ledstatus = 0;
    analogWrite(LED_OUT, 0);
    updateButton(but1_context, ledstatus);
    return;
  }
  ledDimDirection = !ledDimDirection;
  ledIsDimming = 0;
}

void PushButDown(){
  if (!pushButtonDown){
    return;
  }
  if (!ledstatus){
    pushButtonDown = 0;
    return;
  }
  if (!pushButtonTimer){
    butDownTimer = millis();
    pushButtonTimer = 1;
  } else {
    if (millis() - butDownTimer > BUTTON_DOWN_TIME){
      ledIsDimming = 1;
      pushButtonDown = 0;
    }
  }
}

void Dimmmer(){
  if (!ledIsDimming){
    return;
  }
  if (millis() - ledDimTimer > DIMMER_INTERVAL){
    ledDimTimer = millis();
    if (ledDimDirection){
      if (ledbrightness < 255){
        ledbrightness++;
        analogWrite(LED_OUT, ledbrightness);
      }
    } else {
      if (ledbrightness > 1){
        ledbrightness--;
        analogWrite(LED_OUT, ledbrightness);
      }
      
    }
  }
}

void sdInit(){
    ws = new AsyncWebSocket("/ws");
    ws->onEvent(onEvent);
    server->addHandler(ws);
    pinMode(PUSH_BUTTON, INPUT_PULLUP);
    analogWrite(LED_OUT, 0);
    pushButtonState = digitalRead(PUSH_BUTTON);
}

void sdRun(){
  ReadButton();
  PushButDown();
  PushButUp();
  Dimmmer();
  ws->cleanupClients();
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
    switch (type){
      case WS_EVT_CONNECT:
        #if DEBUG == 1
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        #endif
        break;
      case WS_EVT_DISCONNECT:
        #if DEBUG == 1
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        #endif
        break;
      case WS_EVT_DATA:
        wsMessage(arg, data, len);
        break;
      case WS_EVT_PONG:

        break;
      case WS_EVT_ERROR:
        break;
  }
}

void wsMessage(void *arg, uint8_t *data, size_t len){
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT){
  
    DeserializationError er = deserializeJson(doc, data, len);
    if (er){
      return;
    }

    String event = doc["event"];
    String id = doc["payload"]["settings"]["id"];
    String context = doc["context"];

    #if DEBUG == 1
    Serial.print("Event: ");
    Serial.println(event);
    Serial.print("ID: ");
    Serial.println(id);
    Serial.print("Context: ");
    Serial.println(context);
    Serial.println("");
    #endif
    
    if (id == "BUTTON1"){
      strcpy(but1_context, context.c_str());
      if (event == "keyUp"){
        pushButtonUp = 1;
      } else if (event == "keyDown"){
        pushButtonDown = 1;
      } else if (event == "willAppear"){
        updateButton(but1_context, ledstatus);
      } else if (event == "willDisappear"){
        *but1_context = 0;
      }
    }
  }
}

void updateButton(char *context, uint8_t state){
  if (*context == 0){
    return;
  }
  char f[150];
  sprintf(f, "{\"event\":\"setState\",\"context\":\"%s\",\"payload\":{\"state\":%d}}", context, state);
  ws->textAll(f);
}

void ReadButton(){
  if (millis() - switchDebounce > SWITCH_DEBOUNCE){
    switchDebounce = millis();
    uint8_t x = digitalRead(PUSH_BUTTON);
    if (x == pushButtonState){
      return;
    }
    pushButtonState = x;
    if (x == 0){
      pushButtonDown = 1;
    } else {
      pushButtonUp = 1;
    }
  }
}

void SetLedVal(int x){
  if (x >= 0 && x <=255){
    if (x == 0){
      analogWrite(LED_OUT, 0);
      ledstatus = 0;
    } else {
      ledstatus = 1;
      analogWrite(LED_OUT, x);
    }
    updateButton(but1_context, ledstatus);
  }
}

uint8_t GetLevel(){
  return ledbrightness;
}

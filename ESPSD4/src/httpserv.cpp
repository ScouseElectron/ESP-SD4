/* Copyright ScouseElectron <info@scouseelectron.co.uk> */
/* Telephone: +44151 909 4329 9AM-5PM MON-FRI */
/* HTTPSERV.CPP 10/8/22 v1.2 */

/* QH4 */

#include "httpserv.h"

AsyncWebServer *server;

char loginusername[] = "admin";

void httpservInit(){
  server = new AsyncWebServer(HTTP_PORT);
  server->on("/favicon.ico", HTTP_GET, favico);
  server->on("/", HTTP_GET, mainpage);
  server->on("/main.html", HTTP_GET, mainpage);
  server->on("/settings", HTTP_GET, settingsjson);
  server->on("/update", HTTP_POST, updatesettings);
  server->on("/password", HTTP_POST, changepassword);
  server->on("/resetme", HTTP_GET, configreset);
  server->serveStatic("/img", LittleFS, "/img/");
  server->serveStatic("/js", LittleFS, "/js/");
  server->serveStatic("/css", LittleFS, "/css/");
  server->onNotFound(notfound);
  server->begin();
}

void configreset(AsyncWebServerRequest *r){
  ConfigReset();
  r->send(LittleFS, "/update.html", "text/html");
}

void favico(AsyncWebServerRequest *r){
  r->send(LittleFS, "/favicon.ico", "image/x-icon");
}

void notfound(AsyncWebServerRequest *r){
  r->send(404, "text/html", "<h2>404 Not Found</h2>");
}

void mainpage(AsyncWebServerRequest *r){
  if (!r->authenticate(loginusername, config->www_password)){
    return r->requestAuthentication();
  }
  r->send(LittleFS, "/main.html", "text/html");
}

void settingsjson(AsyncWebServerRequest *r){
  if (!r->authenticate(loginusername, config->www_password)){
    return r->requestAuthentication();
  }
  char b[512];
  sprintf_P(b, PSTR("{\"ipaddr\":\"%s\",\"gateway\":\"%s\",\"netmask\":\"%s\",\"dns1\":\"%s\",\"dns2\":\"%s\",\"wifissid\":\"%s\",\"wifipass\":\"%s\",\"ipmode\":\"%u\",\"authmode\":\"%u\"}"), config->ipaddr, config->gateway, config->netmask, config->dns1, config->dns2, config->wifissid, config->wifipass, config->dhcp, config->wifienc);
  AsyncWebServerResponse *res = r->beginResponse(200, "application/json", b);
  res->addHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  res->addHeader("Cache-Control", "post-check=0, pre-check=0");
  res->addHeader("Pragma", "no-cache");
  r->send(res);
}

void updatesettings(AsyncWebServerRequest *r){
  if (!r->authenticate(loginusername, config->www_password)){
    return r->requestAuthentication();
  }
  w_config *tmpconfig = new w_config;
  memcpy(tmpconfig, config, sizeof(w_config));
  if (!r->hasArg("wifissid") || !r->hasArg("wifipass") || !r->hasArg("ipmode")){
    return r->send(400, "text/html", "<h2>400 Bad Request</h2>");
  }
  if (!checkwifidetailes(r->arg("wifissid"), r->arg("wifipass"))){
    return r->send(400, "text/html", "<h2>400 Bad Request</h2>");
  }
  r->arg("wifissid").toCharArray(tmpconfig->wifissid, MAX_SSID);
  r->arg("wifipass").toCharArray(tmpconfig->wifipass, MAX_PASS);
  if (r->arg("ipmode") == "0"){
    tmpconfig->dhcp = 0;
  } else {
    tmpconfig->dhcp = 1;
  }
  bool e = 0;
  if (!tmpconfig->dhcp){
    if (!r->hasArg("ipaddr") || !r->hasArg("netmask") || !r->hasArg("gateway") || !r->hasArg("dns1") || !r->hasArg("dns2")){
      return r->send(400, "text/html", "<h2>400 Bad Request</h2>");
    }
    if (!CheckIPAddr4(r->arg("ipaddr"))) e = 1;
    if (!CheckIPAddr4(r->arg("netmask"))) e = 1;
    if (!CheckIPAddr4(r->arg("gateway"))) e = 1;
    if (!CheckIPAddr4(r->arg("dns1"))) e = 1;
    if (r->arg("dns2") != ""){
      if (!CheckIPAddr4(r->arg("dns2"))) e = 1;
    }
    if (e){
      return r->send(400, "text/html", "<h2>400 Bad Request</h2>");
    }
    r->arg("ipaddr").toCharArray(tmpconfig->ipaddr, MAX_IP4);
    r->arg("netmask").toCharArray(tmpconfig->netmask, MAX_IP4);
    r->arg("gateway").toCharArray(tmpconfig->gateway, MAX_IP4);
    r->arg("dns1").toCharArray(tmpconfig->dns1, MAX_IP4);
    r->arg("dns2").toCharArray(tmpconfig->dns2, MAX_IP4);
  }
  memcpy(config, tmpconfig, sizeof(w_config));
  ConfigSave();
  r->send(LittleFS, "/update.html", "text/html");
}

void changepassword(AsyncWebServerRequest *r){
  if (!r->authenticate(loginusername, config->www_password)){
    return r->requestAuthentication();
  }
  if (!r->hasArg("pas1") || !r->hasArg("pas2") || !r->hasArg("pas3")){
    return r->send(400, "text/html", "<h2>400 Bad Request</h2>");
  }
  String pas1 = r->arg("pas1");
  String pas2 = r->arg("pas2");
  String pas3 = r->arg("pas3");
  if (!CheckString(pas1) || !CheckString(pas2) || !CheckString(pas3)){
    return r->send(400, "text/html", "<h2>400 Bad Request</h2>");
  }
  if (strcmp(pas2.c_str(), pas3.c_str()) != 0){
    return r->send(422, "text/html", "<h2>422 Unprocessable Entity</h2>");
  }
  if (strcmp(config->www_password, pas1.c_str()) != 0){
    return r->send(401, "text/html", "<h2>401 Unauthorized</h2>");
  }
  r->send(LittleFS, "/password_update.html", "text/html");
  pas2.toCharArray(config->www_password, MAX_PASS);
  ConfigSave();
  #if DEBUG == 1
  Serial.println("Password has been changed!");
  #endif
}


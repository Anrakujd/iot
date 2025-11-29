#include <WiFi>
#include <WebServer.h>
#include "SuperCap.h"

#define USE_INTRANET
#define LOCAL_SSID "STKIP Surya"
#define LOCAL_PASS "Stkip#A51."
#define AP_SSID "SuperCapacitor"
#define AP_PASS "0123456789"
#define PIN_OUTPUT 23  // Emergency Stop Charging
#define PIN_A0 36      // SC1
#define PIN_A1 39      // SC2
#define PIN_A2 34      // SC3
#define PIN_A3 35      // SC4
#define PIN_A4 32      // SC5
#define PIN_A5 33      // SC6
#define PIN_A6 25      // SC7
#define PIN_A7 26      // SC8
#define PIN_A8 27      // SC9

int BitsA0 = 0;
int BitsA1 = 0;
int BitsA2 = 0;
int BitsA3 = 0;
int BitsA4 = 0;
int BitsA5 = 0;
int BitsA6 = 0;
int BitsA7 = 0;
int BitsA8 = 0;

float VoltsA0 = 0;
float VoltsA1 = 0;
float VoltsA2 = 0;
float VoltsA3 = 0;
float VoltsA4 = 0;
float VoltsA5 = 0;
float VoltsA6 = 0;
float VoltsA7 = 0;
float VoltsA8 = 0

bool S1 = false;
uint32_t SensorUpdate = 0;
char XML[2048];
char buf[32];

IPAddress Actual_IP;
IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;
WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_OUTPUT, OUTPUT);
  S1 = false;
  digitalWrite(PIN_OUTPUT, S1);
  analogSetVRefPin(25);
  analogSetVRefPin(26);
  analogSetVRefPin(27);
  disableCore0WDT();
  Serial.println("starting server");

#ifdef USE_INTRANET
  WiFi.begin(LOCAL_SSID, LOCAL_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Actual_IP = WiFi.localIP();
#endif

#ifndef USE_INTRANET
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(Actual_IP);
#endif

  server.on("/", SendWebsite);
  server.on("/xml", SendXML);
  server.on("/BUTTON_0", ProcessButton_0);
  server.begin();
}

void loop() {
  if ((millis() - SensorUpdate) >= 50) {
    SensorUpdate = millis();
    BitsA0 = analogRead(PIN_A0);
    BitsA1 = analogRead(PIN_A1);
    BitsA2 = analogRead(PIN_A2);
    BitsA3 = analogRead(PIN_A3);
    BitsA4 = analogRead(PIN_A4);
    BitsA5 = analogRead(PIN_A5);
    BitsA6 = analogRead(PIN_A6);
    BitsA7 = analogRead(PIN_A7);
    BitsA8 = analogRead(PIN_A8);
    VoltsA0 = BitsA0 * 3.3 / 4096;
    VoltsA1 = BitsA1 * 3.3 / 4096;
    VoltsA2 = BitsA2 * 3.3 / 4096;
    VoltsA3 = BitsA3 * 3.3 / 4096;
    VoltsA4 = BitsA4 * 3.3 / 4096;
    VoltsA5 = BitsA5 * 3.3 / 4096;
    VoltsA6 = BitsA6 * 3.3 / 4096;
    VoltsA7 = BitsA7 * 3.3 / 4096;
    VoltsA8 = BitsA8 * 3.3 / 4096;
  }
  server.handleClient();
}

void ProcessButton_0() {
  S1 = !S1;
  digitalWrite(PIN_OUTPUT, S1);
  Serial.print("Button 0 ");
  Serial.println(S1);
  server.send(200, "text/plain", "");
}

void SendWebsite() {
  Serial.println("sending web page");
  server.send(200, "text/html", PAGE_MAIN);
}

void SendXML() {
  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");
  // send bitsA0
  sprintf(buf, "<B0>%d</B0>\n", BitsA0);
  strcat(XML, buf);
  // send Volts0
  sprintf(buf, "<V0>%d.%d</V0>\n", (int)(VoltsA0), abs((int)(VoltsA0 * 10) - ((int)(VoltsA0)*10)));
  strcat(XML, buf);
  // send bits1
  sprintf(buf, "<B1>%d</B1>\n", BitsA1);
  strcat(XML, buf);
  // send Volts1
  sprintf(buf, "<V1>%d.%d</V1>\n", (int)(VoltsA1), abs((int)(VoltsA1 * 10) - ((int)(VoltsA1)*10)));
  strcat(XML, buf);

  // send bits2
  sprintf(buf, "<B2>%d</B2>\n", BitsA2);
  strcat(XML, buf);
  // send Volts2
  sprintf(buf, "<V2>%d.%d</V2>\n", (int)(VoltsA2), abs((int)(VoltsA2 * 10) - ((int)(VoltsA2)*10)));
  strcat(XML, buf);
  // send bits3
  sprintf(buf, "<B3>%d</B3>\n", BitsA3);
  strcat(XML, buf);
  // send Volts3
  sprintf(buf, "<V3>%d.%d</V3>\n", (int)(VoltsA3), abs((int)(VoltsA3 * 10) - ((int)(VoltsA3)*10)));
  strcat(XML, buf);
  // send bits4
  sprintf(buf, "<B4>%d</B4>\n", BitsA4);
  strcat(XML, buf);
  // send Volts4
  sprintf(buf, "<V4>%d.%d</V4>\n", (int)(VoltsA4), abs((int)(VoltsA4 * 10) - ((int)(VoltsA4)*10)));
  strcat(XML, buf);
  // send bits5
  sprintf(buf, "<B5>%d</B5>\n", BitsA5);
  strcat(XML, buf);
  // send Volts5
  sprintf(buf, "<V5>%d.%d</V5>\n", (int)(VoltsA5), abs((int)(VoltsA5 * 10) - ((int)(VoltsA5)*10)));
  strcat(XML, buf);
  // send bits6
  sprintf(buf, "<B6>%d</B6>\n", BitsA6);
  strcat(XML, buf);
  // send Volts6
sprintf(buf, "<V6>%d.%d</V6>\n", (int) (VoltsA6), abs((int) (VoltsA6 * 10) - ((int)
(VoltsA6) * 10));
strcat(XML, buf);

// send bits7
sprintf(buf, "<B7>%d</B7>\n", BitsA7);
strcat(XML, buf);
// send Volts7
sprintf(buf, "<V7>%d.%d</V7>\n", (int) (VoltsA7), abs((int) (VoltsA7 * 10) - ((int)
(VoltsA7) * 10))
strcat(XML, buf);

// send bits8
sprintf(buf, "<B8>%d</B8>\n", BitsA8);
strcat(XML, buf);
// send Volts8
sprintf(buf, "<V8>%d.%d</V8>\n", (int) (VoltsA8), abs((int) (VoltsA8 * 10) - ((int)
(VoltsA8) * 10)));
strcat(XML, buf);
if (S1) {
    strcat(XML, "<BTN0>1</BTN0>\n");
}
else {
    strcat(XML, "<BTN0>0</BTN0>\n");
}
strcat(XML, "</Data>\n");
Serial.println(XML);
server.send(200, "text/xml",
XML);
//====================================================================

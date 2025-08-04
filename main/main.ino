

#define BLYNK_TEMPLATE_ID "TMPL3LJujOM-p"
#define BLYNK_TEMPLATE_NAME "SSAAS"
#define BLYNK_FIRMWARE_VERSION "0.1.0"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "credentials.h"

BlynkTimer timer;
WidgetTerminal terminal(V2);


#include "blynk_callbacks.h"
#include "fingerprint.h"


void setup() {
  Serial.begin(115200);
  delay(1000);

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

  terminal.println("🔌 Connected to Blynk");
  terminal.println("📡 Waiting for commands...");
  terminal.flush();

  setupFingerprint();  // AS608 Setup

  // Poll menu-triggered actions
  timer.setInterval(300L, handleRequestedActions);
}

void loop() {
  Blynk.run();
}


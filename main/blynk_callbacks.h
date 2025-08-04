#ifndef BLYNK_CALLBACKS_H
#define BLYNK_CALLBACKS_H

#include <BlynkSimpleEsp32.h>
#include "fingerprint.h"

// Variable Definitions (not extern here!)
String userName = "";
int userID = 0;
int selectedAction = 0;

bool enrollRequested = false;
bool deleteRequested = false;
bool listRequested = false;
bool verifyRequested = false;


// Terminal widget declared once here
extern WidgetTerminal terminal;

// BLYNK_WRITE handlers
BLYNK_WRITE(V0) {
  userID = param.asInt();
  terminal.println("🆔 User ID set to: " + String(userID));
  terminal.flush();
}

BLYNK_WRITE(V3) {
  userName = param.asStr();
  terminal.println("📛 User Name set to: " + userName);
  terminal.flush();
}

BLYNK_WRITE(V1) {
  selectedAction = param.asInt();
  // Serial.println(selectedAction);
  terminal.println("🚀 Action selected: " + String(selectedAction));
  terminal.flush();

  handleRequestedActions();

  switch (selectedAction) {
    case 0:
      enrollRequested = true;
      break;
    case 1:
      deleteRequested = true;
      break;
  }

  selectedAction = -1;
}
BLYNK_WRITE(V5) {
  if (param.asInt() == 1) {
    verifyFingerprint();
    Blynk.virtualWrite(V5, 0);  // Reset switch
  }
}

BLYNK_WRITE(V6) {  // List Fingerprints Switch
  int state = param.asInt();
  if (state == 1) {
    terminal.println("📋 Fingerprint list requested.");
    terminal.flush();
    listFingerprints();
    Blynk.virtualWrite(V6, 0); // reset switch
  }
}




#endif

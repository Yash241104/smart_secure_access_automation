#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>


// UART2 for AS608: RX = GPIO16, TX = GPIO17
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// External from blynk_callbacks.h
extern WidgetTerminal terminal;
extern String userName;
extern int userID;
extern int selectedAction;

// Forward declarations
void setupFingerprint();
void enrollFingerprint(int id, String name);
void deleteFingerprint(int id);
void listFingerprints();
void handleRequestedActions();

// -------------------- Setup --------------------
void setupFingerprint() {
  mySerial.begin(57600, SERIAL_8N1, 16, 17);  // RX, TX
  finger.begin(57600);
  delay(1000);

  if (finger.verifyPassword()) {
    terminal.println("✅ Fingerprint sensor ready");
  } else {
    terminal.println("❌ Sensor connection failed");
    while (true); // Block forever
  }

  terminal.flush();
}

// -------------------- Action Handler --------------------
void handleRequestedActions() {

  switch (selectedAction) {
    case 0:
      terminal.println("🚀 Enrolling " + userName + " [ID: " + String(userID) + "]");
      enrollFingerprint(userID, userName);
      break;
    case 1:
      terminal.println("🗑️ Deleting fingerprint for ID: " + String(userID));
      deleteFingerprint(userID);
      break;
    default:
      terminal.println("⚠️ Unknown action.");
      break;
  }

  terminal.flush();
}

// -------------------- Enroll --------------------
void enrollFingerprint(int id, String name) {
  terminal.println("➡️ Place finger to enroll...");
  terminal.flush();

  int p = -1;
  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    delay(500);
  }

  finger.image2Tz(1);
  terminal.println("✅ First scan done. Remove finger.");
  terminal.flush();
  delay(2000);

  while ((p = finger.getImage()) != FINGERPRINT_NOFINGER) {
    delay(100);
  }

  terminal.println("➡️ Place the same finger again...");
  terminal.flush();

  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    delay(500);
  }

  finger.image2Tz(2);
  if (finger.createModel() != FINGERPRINT_OK) {
    terminal.println("❌ Finger mismatch. Try again.");
    terminal.flush();
    return;
  }

  if (finger.storeModel(id) == FINGERPRINT_OK) {
    terminal.println("🎉 Fingerprint stored successfully!");
    terminal.println("🔐 User " + name + " added with ID: " + String(id));
  } else {
    terminal.println("❌ Failed to store fingerprint.");
  }

  terminal.flush();
}

// -------------------- Delete --------------------
void deleteFingerprint(int id) {
  if (finger.deleteModel(id) == FINGERPRINT_OK) {
    terminal.println("✅ Deleted fingerprint ID: " + String(id));
  } else {
    terminal.println("❌ Delete failed. ID might not exist.");
  }

  terminal.flush();
}

// -------------------- List --------------------
void listFingerprints() {
  bool anyFound = false;

  for (int i = 0; i < 128; i++) {
    if (finger.loadModel(i) == FINGERPRINT_OK) {
      terminal.println("🟢 ID " + String(i) + ": Available");
      anyFound = true;
    }
  }

  if (!anyFound) {
    terminal.println("📭 No fingerprint entries found.");
  }

  terminal.flush();
}

void verifyFingerprint() {
  terminal.println("🔍 Place your finger to verify...");
  terminal.flush();

  int p = -1;
  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    delay(500);
  }

  if (finger.image2Tz() != FINGERPRINT_OK) {
    terminal.println("❌ Could not convert image");
    terminal.flush();
    return;
  }

  if (finger.fingerSearch() != FINGERPRINT_OK) {
    terminal.println("🔒 No match found.");
  } else {
    int id = finger.fingerID;
    terminal.println("✅ Match found!");
    terminal.println("🆔 ID: " + String(id));
  }

  terminal.flush();
}


#endif

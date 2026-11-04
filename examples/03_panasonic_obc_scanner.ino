/*
 * Panasonic SC-EN27 IR tester (ESP8266 / Difi WF8266RDF)
 * JP1 setup: Panasonic AMP / 0308
 *   Device = 160, Subdevice = 0, OBC 154 = AUX (MUSIC PORT)
 *
 * Pins:  IR_E (send) = GPIO0 (D3)
 *        IR_R (recv) = GPIO4 (D2)
 *
 * Serial monitor: 115200 baud
 *
 * Single-letter commands (no Enter needed for the ones without a number):
 *   m        send MUSIC PORT (AUX, OBC 154) - independent of scan settings
 *   a        send AUX (same as m)
 *   s        start / continue scan from current position
 *   p        pause / resume scan
 *   x        stop scan and reset position to 0
 *   n        send next OBC manually (one step forward)
 *   b        send previous OBC manually (one step back)
 *   h        show help
 *
 * Commands with a number (type the number, then press Enter):
 *   <0-255>  send that OBC            e.g. 154
 *   g<n>     jump scan position to n  e.g. g100
 *   t<ms>    scan interval (min 500) e.g. t1000 (default 1000)
 *   d<n>     set device               e.g. d160
 *   u<n>     set subdevice            e.g. u0
 *   r<n>     set repeat count (0-10)  e.g. r1
 *
 * Important:
 *   [RECEIVED] confirms local IR receiver loopback/capture. It does not
 *   by itself prove that the stereo accepted or acted on the command.
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

const uint16_t IR_SEND_PIN = 0;
const uint16_t IR_RECV_PIN = 4;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;

IRsend irsend(IR_SEND_PIN);
IRrecv irrecv(IR_RECV_PIN, kCaptureBufferSize, kTimeout, true);
decode_results results;

// MUSIC PORT uses fixed values, independent of the scan settings.
const uint8_t AUX_DEVICE = 160;
const uint8_t AUX_SUBDEVICE = 0;
const uint8_t AUX_OBC = 154;
const uint8_t AUX_REPEATS = 1;

uint8_t device = 160;
uint8_t subdevice = 0;
uint8_t repeats = 1;

// Scan state.
bool scanning = false;
bool paused = false;
int scanNext = 0;          // Next OBC the scan will send.
int lastSent = -1;         // Last OBC actually sent.
uint32_t scanDelayMs = 1000;
uint32_t lastScanMs = 0;

String inputLine;

// JP1 fields are LSB-first; IRremoteESP8266 sendPanasonic64 is MSB-first.
uint8_t reverseBits(uint8_t value) {
  uint8_t result = 0;
  for (uint8_t i = 0; i < 8; i++) {
    result = (result << 1) | (value & 1);
    value >>= 1;
  }
  return result;
}

uint64_t makePanasonicCode(uint8_t dev, uint8_t sub, uint8_t obc) {
  uint8_t checksum = dev ^ sub ^ obc;
  uint64_t code = 0x4004ULL << 32;
  code |= (uint64_t)reverseBits(dev) << 24;
  code |= (uint64_t)reverseBits(sub) << 16;
  code |= (uint64_t)reverseBits(obc) << 8;
  code |= reverseBits(checksum);
  return code;
}

void sendObc(uint8_t obc) {
  uint64_t code = makePanasonicCode(device, subdevice, obc);
  lastSent = obc;
  Serial.printf("[SEND] OBC=%u  (dev=%u sub=%u rep=%u)  data=0x%s\n",
                obc, device, subdevice, repeats,
                uint64ToString(code, HEX).c_str());
  irsend.sendPanasonic64(code, 48, repeats);
}

// MUSIC PORT shortcut: fixed dev=160, sub=0, OBC=154.
// It does not read/change device, subdevice, repeats, lastSent, or scan state.
void sendMusicPort() {
  uint64_t code = makePanasonicCode(AUX_DEVICE, AUX_SUBDEVICE, AUX_OBC);
  Serial.printf("[MUSIC PORT] dev=%u sub=%u OBC=%u  data=0x%s\n",
                AUX_DEVICE, AUX_SUBDEVICE, AUX_OBC,
                uint64ToString(code, HEX).c_str());
  irsend.sendPanasonic64(code, 48, AUX_REPEATS);
}

void printHelp() {
  Serial.println();
  Serial.println("=== SC-EN27 IR Test (Panasonic AMP/0308) ===");
  Serial.println("m  MUSIC PORT (a = same)  s  start/continue scan");
  Serial.println("p  pause/resume          x  stop + reset to 0");
  Serial.println("n  next OBC (manual)     b  previous OBC (manual)");
  Serial.println("h  help");
  Serial.println("<0-255>  send that OBC   g<n>  jump scan position");
  Serial.println("t<ms>  interval          d<n>  device");
  Serial.println("u<n>   subdevice         r<n>  repeat count");
  Serial.printf("Current: dev=%u sub=%u rep=%u interval=%lums next=%d\n",
                device, subdevice, repeats, (unsigned long)scanDelayMs,
                scanNext);
  Serial.println();
}

void startScan() {
  if (scanNext > 255) scanNext = 0;
  scanning = true;
  paused = false;
  lastScanMs = 0;
  Serial.printf("[SCAN] start from OBC %d, every %lu ms. p=pause x=stop\n",
                scanNext, (unsigned long)scanDelayMs);
}

void togglePause() {
  if (!scanning) {
    Serial.println("[SCAN] not running. Press s to start.");
    return;
  }
  paused = !paused;
  if (paused) {
    Serial.printf("[PAUSE] last sent OBC = %d, next = %d. Press p to resume.\n",
                  lastSent, scanNext);
  } else {
    lastScanMs = millis();
    Serial.println("[RESUME]");
  }
}

void stopScan() {
  scanning = false;
  paused = false;
  scanNext = 0;
  Serial.println("[STOP] scan stopped, position reset to 0");
}

// Letters that run immediately (no Enter needed).
bool runInstant(char c) {
  switch (c) {
    case 'm':
    case 'a':
      sendMusicPort();
      return true;
    case 's': startScan(); return true;
    case 'p': togglePause(); return true;
    case 'x': stopScan(); return true;
    case 'h': printHelp(); return true;
    case 'n':
      scanNext = constrain(scanNext, 0, 255);
      sendObc((uint8_t)scanNext);
      if (scanNext < 255) scanNext++;
      return true;
    case 'b':
      scanNext = constrain(lastSent - 1, 0, 255);
      sendObc((uint8_t)scanNext);
      return true;
  }
  return false;
}

// Commands that need a number followed by Enter.
void runLine(String cmd) {
  cmd.trim();
  cmd.toLowerCase();
  if (cmd.length() == 0) return;

  char first = cmd.charAt(0);
  long val = cmd.substring(isDigit(first) ? 0 : 1).toInt();

  if (isDigit(first)) {
    if (val >= 0 && val <= 255) {
      sendObc((uint8_t)val);
    } else {
      Serial.println("OBC must be 0-255");
    }
  } else if (first == 'g') {
    scanNext = constrain(val, 0, 255);
    Serial.printf("[SCAN] position = %d\n", scanNext);
  } else if (first == 't') {
    scanDelayMs = max(500L, val);
    Serial.printf("interval = %lu ms\n", (unsigned long)scanDelayMs);
  } else if (first == 'd') {
    device = (uint8_t)constrain(val, 0, 255);
    Serial.printf("device = %u\n", device);
  } else if (first == 'u') {
    subdevice = (uint8_t)constrain(val, 0, 255);
    Serial.printf("subdevice = %u\n", subdevice);
  } else if (first == 'r') {
    repeats = (uint8_t)constrain(val, 0, 10);
    Serial.printf("repeat = %u\n", repeats);
  } else {
    Serial.println("Unknown. Press h for help.");
  }
}

void pollSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputLine.length()) {
        runLine(inputLine);
        inputLine = "";
      }
      continue;
    }
    if (inputLine.length() == 0) {
      char lc = tolower(c);
      if (runInstant(lc)) continue;
    }
    inputLine += c;
  }
}

void pollScan() {
  if (!scanning || paused) return;
  if (lastScanMs != 0 && millis() - lastScanMs < scanDelayMs) return;
  lastScanMs = millis();
  if (lastScanMs == 0) lastScanMs = 1;

  if (scanNext > 255) {
    scanning = false;
    Serial.println("[SCAN] finished");
    return;
  }
  sendObc((uint8_t)scanNext);
  scanNext++;
}

void pollReceive() {
  if (!irrecv.decode(&results)) return;

  Serial.println("[RECEIVED]");
  Serial.print(resultToHumanReadableBasic(&results));

  if (results.decode_type == PANASONIC && results.bits == 48) {
    uint64_t v = results.value;
    uint8_t dev = reverseBits((v >> 24) & 0xFF);
    uint8_t sub = reverseBits((v >> 16) & 0xFF);
    uint8_t obc = reverseBits((v >> 8) & 0xFF);
    Serial.printf("  -> JP1 dev=%u sub=%u OBC=%u\n", dev, sub, obc);
  }
  Serial.println();
  irrecv.resume();
}

void setup() {
  Serial.begin(115200);
  delay(500);
  irsend.begin();
  irrecv.enableIRIn();
  printHelp();
}

void loop() {
  pollSerial();
  pollScan();
  pollReceive();
  yield();
}

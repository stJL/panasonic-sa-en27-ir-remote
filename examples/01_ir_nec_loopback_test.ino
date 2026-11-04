/*
 * IR hardware loopback test
 * Board: Difi WF8266RDF (ESP8266)
 *
 * Purpose:
 *   Verify that the IR sender and receiver hardware work before
 *   investigating a device-specific protocol.
 *
 * Pins:
 *   IR_E sender   = GPIO0 (D3)
 *   IR_R receiver = GPIO4 (D2)
 *
 * Serial Monitor: 115200 baud
 *
 * Expected result:
 *   The sketch periodically sends NEC 0x20DF10EF and the receiver
 *   should decode the same value when the emitter is aimed at the
 *   receiver module at a short distance.
 *
 * Note:
 *   This is a local hardware loopback test. It does not test the
 *   Panasonic stereo or prove that the stereo accepts any command.
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

const uint16_t IR_SEND_PIN = 0;  // GPIO0 / D3 / IR_E
const uint16_t IR_RECV_PIN = 4;  // GPIO4 / D2 / IR_R

IRsend irsend(IR_SEND_PIN);
IRrecv irrecv(IR_RECV_PIN);
decode_results results;

uint32_t lastSendMs = 0;
const uint32_t SEND_INTERVAL_MS = 3000;

void setup() {
  Serial.begin(115200);
  delay(500);

  irsend.begin();
  irrecv.enableIRIn();

  Serial.println();
  Serial.println("=== IR Send/Receive Hardware Test ===");
  Serial.println("Sender: GPIO0 (IR_E)");
  Serial.println("Receiver: GPIO4 (IR_R)");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println("[RECEIVED]");
    Serial.print("Protocol: ");
    Serial.println(typeToString(results.decode_type));
    Serial.print("Value: 0x");
    Serial.println(uint64ToString(results.value, HEX));
    Serial.print("Bits: ");
    Serial.println(results.bits);
    Serial.println();
    irrecv.resume();
  }

  const uint32_t now = millis();
  if (now - lastSendMs >= SEND_INTERVAL_MS) {
    lastSendMs = now;
    Serial.println("[SENDING] NEC test signal...");
    irsend.sendNEC(0x20DF10EF, 32);
  }
}

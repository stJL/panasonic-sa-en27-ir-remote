/*
 * Panasonic 48-bit encode/send/receive test
 * Board: Difi WF8266RDF (ESP8266)
 *
 * Purpose:
 *   Verify that IRremoteESP8266 can encode, transmit, and decode a
 *   Panasonic 48-bit frame through the project's sender/receiver setup.
 *
 * Pins:
 *   IR_E sender   = GPIO0 (D3)
 *   IR_R receiver = GPIO4 (D2)
 *
 * Serial Monitor: 115200 baud
 *
 * Test parameters used during the project:
 *   Panasonic ID = 0x4004
 *   Device       = 0x01
 *   Subdevice    = 0x02
 *   Function     = 0x00
 *
 * Expected loopback value: 0x400401020003 (48 bits)
 *
 * Note:
 *   This is a protocol/hardware loopback test, not a SC-EN27 command.
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

const uint16_t IR_SEND_PIN = 0;  // GPIO0 / D3 / IR_E
const uint16_t IR_RECV_PIN = 4;  // GPIO4 / D2 / IR_R

const uint16_t PANASONIC_ID = 0x4004;
const uint8_t DEVICE = 0x01;
const uint8_t SUBDEVICE = 0x02;
const uint8_t FUNCTION_CODE = 0x00;

IRsend irsend(IR_SEND_PIN);
IRrecv irrecv(IR_RECV_PIN);
decode_results results;


void setup() {
  Serial.begin(115200);
  delay(500);

  irsend.begin();
  irrecv.enableIRIn();

  Serial.println();
  Serial.println("=== Panasonic 48-bit Loopback Test ===");
  Serial.println("Sender: GPIO0 (IR_E)");
  Serial.println("Receiver: GPIO4 (IR_R)");

  const uint64_t code = irsend.encodePanasonic(
    PANASONIC_ID, DEVICE, SUBDEVICE, FUNCTION_CODE
  );

  Serial.print("Encoded value: 0x");
  Serial.println(uint64ToString(code, HEX));
  Serial.println("Sending Panasonic 48-bit frame...");
  irsend.sendPanasonic64(code, 48, 0);
  Serial.println("Send complete.");
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
}

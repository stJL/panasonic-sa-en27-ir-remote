# Example sketches

These sketches document the steps used during the SA-EN27 infrared investigation. They are research and verification tools, not a finished physical-button remote-control firmware.

## Recommended order

1. **`01_ir_nec_loopback_test.ino`** — verify the IR sender and receiver hardware with a simple NEC signal.
2. **`02_panasonic_48bit_loopback_test.ino`** — verify Panasonic 48-bit encoding, transmission, and local reception.
3. **`03_panasonic_obc_scanner.ino`** — scan OBC values for a selected Device/Subdevice combination, manually send individual values, and decode the locally received signal.

## Build environment

- Difi WF8266RDF ESP8266 IoT Learning Board
- Arduino IDE 2
- ESP8266 board support package
- `IRremoteESP8266` library
- Serial Monitor at 115200 baud

Pin mapping used in this project:

- IR sender (`IR_E`): GPIO0 / D3
- IR receiver (`IR_R`): GPIO4 / D2

## Important interpretation note

The scanner's `[RECEIVED]` output means the ESP8266 receiver captured the signal emitted by the local sender (loopback). It does not, by itself, demonstrate that the stereo received or acted on that command. Record the stereo's visible or audible response separately, along with its current input mode and operating state.

The scanner is deliberately configurable because the OBC mapping depends on the selected Device and Subdevice. The MUSIC PORT shortcut always sends the project's known AUX command (`Device 160`, `Subdevice 0`, `OBC 154`).

For the findings collected so far, see the main [README](../README.md) and [`ir-codes.csv`](../ir-codes.csv).

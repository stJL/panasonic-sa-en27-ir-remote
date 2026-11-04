# Arduino examples

This folder contains the sketches used during the SC-EN27 IR investigation. They are research and test tools, not a finished consumer remote-control firmware.

## Requirements

- ESP8266-compatible Arduino board package.
- Difi WF8266RDF board (or a suitably wired ESP8266 plus IR transmitter and receiver).
- [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) Arduino library.
- Serial Monitor at **115200 baud**.

For the Difi WF8266RDF used in this project:

- IR sender (`IR_E`): GPIO0 / D3.
- IR receiver (`IR_R`): GPIO4 / D2.

See the root [README](../README.md) for hardware notes and research references.

## Sketches

### `01_ir_nec_loopback_test.ino`

A simple NEC send/receive loopback test. It verifies that the transmitter can emit an IR frame and that the receiver can capture and decode it. It does not test Panasonic commands or prove that the stereo accepts a command.

### `02_panasonic_48bit_loopback_test.ino`

Builds and sends a Panasonic 48-bit test frame, then checks whether the local receiver decodes the same frame. It validates the basic Panasonic send/receive path before attempting to control the stereo.

### `03_panasonic_obc_scanner.ino`

The main reverse-engineering tool. It supports:

- Scanning OBC values from 0 to 255.
- Changing Device and Subdevice values.
- Sending a single OBC manually or stepping forward/backward.
- Pausing, resuming, stopping, and repositioning a scan.
- Adjusting the scan interval and repeat count.
- Sending the known MUSIC PORT command independently of the current scan settings.
- Receiving Panasonic frames and decoding the Device, Subdevice, and OBC fields for display.

Read the sketch's header comments for the serial commands. Commands that take a number require a line ending (Newline or Both NL & CR); single-letter commands run immediately.

## Important test discipline

1. Verify the ESP8266 transmitter/receiver path before scanning.
2. Keep the stereo pointed at the IR transmitter and observe the stereo display or behavior during scanning.
3. A `[RECEIVED]` message only confirms that the ESP8266 receiver captured a frame. It is not proof that the stereo reacted.
4. Record the input mode (FM, MUSIC PORT, CD, etc.) and the actual stereo response for every candidate command.
5. Be careful with state-changing commands such as POWER, SLEEP, and timer controls. Stop the scan when the stereo reacts unexpectedly.

The scanner can transmit many experimental commands. Use it only on equipment you own or are authorized to test.

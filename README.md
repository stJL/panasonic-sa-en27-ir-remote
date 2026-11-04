# Panasonic SC-EN27 / SA-EN27 IR Remote Codes

An experimental, community-oriented collection of infrared (IR) commands identified while recreating the remote control for the Panasonic SC-EN27 / SA-EN27 mini stereo.

Original remote model referenced during the project: `N2QAGB000038`.

> **Disclaimer:** This is an independent reverse-engineering project, not an official Panasonic document. The notes and button labels are based on hands-on testing and may contain mistakes or incomplete interpretations. If you spot an error, have a correction, or discover another code, contributions are very welcome. Please open an issue or pull request and include the stereo model, input mode, command details, and the behavior you observed.

繁體中文版本：[README.zh-TW.md](README.zh-TW.md)

## Project status

- 29 command mappings are currently documented across five Device 160 subdevices.
- The protocol observed for these commands is Panasonic 48-bit.
- Identified functions include MUSIC PORT, tuner, CD controls, volume, sound effects, numeric keys, display, sleep, and timer controls.
- CD STOP and PLAY MODE have not yet been reproduced. A usable CD was not available for the relevant playback tests.

The command names describe the behavior observed during this project. Some functions may behave differently depending on the stereo's current input or operating state.

## Hardware and software

| Item | Configuration |
|---|---|
| Board | Difi WF8266RDF ESP8266 IoT Learning Board |
| Development environment | Arduino IDE 2 |
| IR library | `IRremoteESP8266` |
| IR transmitter | `IR_E` = GPIO0 (D3) |
| IR receiver | `IR_R` = GPIO4 (D2) |
| Serial Monitor | 115200 baud |
| IR protocol | Panasonic, 48 bits |
| Device value | 160 |

### Board pin note

On the Difi WF8266RDF board used in this project:

- `IR_E` is the IR sender on GPIO0 / D3.
- `IR_R` is the IR receiver on GPIO4 / D2.
- `IRSW` is associated with the IR obstacle-sensing function; it is not the IR transmitter pin.

## Source code and implementation

The [`examples/`](examples/) directory contains the sketches used to validate the hardware and investigate the command set:

| Sketch | Purpose |
|---|---|
| [`01_ir_nec_loopback_test.ino`](examples/01_ir_nec_loopback_test.ino) | Checks the IR sender/receiver path with a simple NEC test frame. |
| [`02_panasonic_48bit_loopback_test.ino`](examples/02_panasonic_48bit_loopback_test.ino) | Checks Panasonic 48-bit encoding and local reception. |
| [`03_panasonic_obc_scanner.ino`](examples/03_panasonic_obc_scanner.ino) | Main research tool: scans OBC values, supports manual sends and scan control, and decodes captured Panasonic frames. |
| [`examples/README.md`](examples/README.md) | Build notes, recommended test order, and interpretation notes. |

### How the investigation works

1. Verify the physical IR sender and receiver with a known NEC test signal.
2. Verify Panasonic 48-bit transmission and decoding using a generated test frame.
3. Use the scanner to select a Device and Subdevice, then examine OBC values while observing the stereo itself.
4. Record the stereo's actual response and the corresponding Device, Subdevice, OBC, and 48-bit data in the command table.

The scanner converts the JP1-style byte fields to the bit order expected by `IRremoteESP8266`'s `sendPanasonic64()` method, including the checksum byte. The code and command table are provided so other researchers can reproduce the method, challenge the interpretations, and contribute additional findings.

**Scope:** The repository currently includes the test and reverse-engineering sketches. It does not yet include a finished standalone remote with a physical keypad, display, or custom user interface.

## IR command data

All entries below use Device `160`. The `IR Data` column contains the 48-bit value used with `IRremoteESP8266`'s `sendPanasonic64()` method.

### Subdevice 0 — AMP/0308

| Button / function | OBC | IR data |
|---|---:|---|
| MUSIC PORT | 154 | `0x40040500595C` |
| Volume Up | 32 | `0x400405000401` |
| Volume Down | 33 | `0x400405008481` |
| Live Virtualizer | 48 | `0x400405000C09` |

### Subdevice 4 — AMP/0309

| Button / function | OBC | IR data |
|---|---:|---|
| TUNER/BAND | 164 | `0x400405202500` |

### Subdevice 10 — CD/0303

| Button / function | OBC | IR data |
|---|---:|---|
| OPEN/CLOSE | 1 | `0x4004055080D5` |
| CD | 10 | `0x400405505005` |

### Subdevice 16 — Panasonic SC-series research lead

| Button / function | OBC | IR data |
|---|---:|---|
| SOUND EQ | 131 | `0x40040508C1CC` |

### Subdevice 28 — AMP/0518

| Button / function | OBC | IR data |
|---|---:|---|
| 1 | 16 | `0x400405380835` |
| 2 | 17 | `0x4004053888B5` |
| 3 | 18 | `0x400405384875` |
| 4 | 19 | `0x40040538C8F5` |
| 5 | 20 | `0x400405382815` |
| 6 | 21 | `0x40040538A895` |
| 7 | 22 | `0x400405386855` |
| 8 | 23 | `0x40040538E8D5` |
| 9 | 24 | `0x400405381825` |
| 0 | 25 | `0x4004053898A5` |
| POWER | 61 | `0x40040538BC81` |
| Skip/Search Down | 73 | `0x4004053892AF` |
| Skip/Search Up | 74 | `0x40040538526F` |
| DISPLAY | 85 | `0x40040538AA97` |
| >=10 | 132 | `0x40040538211C` |
| PGM/CLEAR | 138 | `0x40040538516C` |
| CLOCK/TIMER ADJ | 149 | `0x40040538A994` |
| SLEEP | 150 | `0x400405386954` |
| TIMER SET/CHECK | 155 | `0x40040538D9E4` |
| ALBUM/CHANNEL SELECT DOWN | 252 | `0x400405383F02` |
| ALBUM/CHANNEL SELECT UP | 253 | `0x40040538BF82` |

A machine-readable copy of the command table is available in [`ir-codes.csv`](ir-codes.csv).

## Subdevice research log — community contributions welcome

The following table records which Device 160 subdevices have been explored in this particular setup. It is a research log, **not** a compatibility list and **not** a claim that a subdevice is unused or unsupported.

For some entries, no matching stereo action was identified during this project's test runs. That result may depend on the test conditions, input mode, equipment state, or interpretation. Other researchers are encouraged to investigate these and share their observations—even a different result is useful.

| Device | Subdevice | Research note |
|---:|---:|---|
| 160 | 0 | Commands identified; associated with AMP/0308. |
| 160 | 4 | TUNER/BAND identified; associated with AMP/0309. |
| 160 | 8 | Explored in this project; no mapping confirmed here. Further investigation welcome. |
| 160 | 10 | OPEN/CLOSE and CD commands identified; associated with CD/0303. |
| 160 | 16 | SOUND EQ identified; found while following a Panasonic SC-series research lead. |
| 160 | 18 | Explored in this project; no mapping confirmed here. Further investigation welcome. |
| 160 | 20 | Explored in this project; no mapping confirmed here. Further investigation welcome. |
| 160 | 28 | Numeric, power, display, skip/search, album/channel, and timer-related commands identified; associated with AMP/0518. |
| 160 | 194 | Explored while following an AMP/0367 lead; no mapping confirmed here. Further investigation welcome. |

If you test one of these combinations, please share:

- Stereo model and, if known, regional variant.
- Current input mode (FM, CD, MUSIC PORT, etc.).
- Device, subdevice, OBC, and full IR data.
- What the stereo actually did (including no visible change, display text, or state-dependent behavior).
- Any relevant test conditions or sequence of commands.

## Not yet reproduced

- **CD STOP** — not yet identified or verified. A usable CD was not available for the playback tests.
- **PLAY MODE** — not yet identified or verified. A usable CD was not available for the playback tests.

These are open items, not claims that the functions cannot be reproduced.

## Testing notes

1. Confirm the board pin mapping: sender GPIO0 (`IR_E`), receiver GPIO4 (`IR_R`).
2. Use the Panasonic 48-bit format with `IRremoteESP8266`.
3. Aim the IR transmitter at the stereo's IR receiver window.
4. Record the stereo's actual response separately from the ESP8266 receiver output.
5. A `[RECEIVED]` message from the ESP8266 receiver can confirm that the locally transmitted signal was captured (loopback); by itself, it does **not** prove that the stereo accepted or acted on the command.
6. The same command may have different effects depending on whether the stereo is in FM, CD, or MUSIC PORT mode. Include the input mode when reporting results.

## Research references and acknowledgements

- Button mappings in this repository were collected through hands-on tests with a Difi WF8266RDF ESP8266 board and a Panasonic SC-EN27 stereo.
- JP1 / One For All setup-code information was used as a research lead to narrow the search. The mappings listed here are based on the project's own tests, not on an assumption that a universal-remote setup code guarantees compatibility.
- Reverse engineering, testing, and documentation were carried out collaboratively by the project maintainer and ChatGPT.

## Corrections and contributions

Errors are possible, and this repository is intended to be improved through shared testing. If you find an incorrect label, a missing command, a different subdevice, or a behavior that only appears in a particular mode, please open an issue or submit a pull request. Please include enough details for someone else to reproduce the observation.

---

Research notes compiled from tests conducted around 2026-11-05. The table may evolve as more devices and operating modes are tested.

# TechWave Audio Midi Controller Module Hardware Design

Simply put, the module reads MIDI messages in, and outputs signals to digital analog controllers to provide the voltage outputs.
This is all based around a RP2040 microcontroller as the brains for handling inputs and outputs.

* [Microcontroller](#microcontroller)
* [MIDI Input](#midi-input)
* [Note and Velocity Output](#note-and-velocity-output)
* [Control and Aux Output](#control-and-aux-output)
* [Gate, Trigger, and Clock Output](#gate-trigger-and-clock-output)
* [User Input](#user-input)
* [OLED Screen](#oled-screen)
* [Power](#power)

## Front View

<img src="../design/board_render_front_annotated.jpg" alt="Rendered PCB front" />

## Rear View
<img src="../design/board_render_rear_annotated.jpg" alt="Rendered PCB rear" />

## PCB Layout
<img src="../design/TechWaveAudio_MidiController_Board.jpg" alt="PCB layout" />

## Microcontroller
The microcontroller at the heart of the system is a RP2040 based [Raspberry Pi Pico 1](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#pico1).
The RP2040 is a very capable and low cost with ernough GPIO, I2C, and SPI lines to handle the needs of this system.

To keep the design a bit more accessible, I made the conscience decision to use the Raspberry Pi Pico module itself instead of opting for using a discrete RP2040 chip only.
Using a discrete RP2040 itself would have allowed for a much smaller footprint on the PCB, however the using the full module allowed for easier testing overall without having to worry about getting the microcontroller section of the circuitry right (why reinvent the wheel).
It also makes flashing the firmware much easier (although I could have added a discrete USB port for doing so).

Both cores of the RP2040 are used. Core 0 handles the display, buttons, menus, and state. 
Core 1 handles the MIDI input and hardware outputs.

## MIDI Input

The MIDI input is handled by a 6N137 optocoupler that signals at +5v.
The output signal is reduced to +3.3v via a simple voltage divider to bring the signal to a manageable level for the microcontroller to use.
All processing of the signal happens within the software.

## Note and Velocity Output

The note 1v/oct and velocity signals are handled by two MCP4725 DAC ICs.
These are single channel, 12 bit DAC chips that use I2C for communication from the microcontroller.
Since these chips are able to output between 0 and +5v, the signal is then increased to the 0 to +10v range via a TL074 opamp.
Additional 1k trimming potentiometers are used to fine tune the output.

## Control and Aux Output

The control and aux signals are handled by a single MCP4902 DAC IC.
This is a single channel 8 bit DAC chip that uses SPI for communication with the microcontroller.
This chip produces a stable output voltage from 0 to +5v, which in turns is sent through a TL074 op amp to increase the range to 0 to +10v.
Like the note and velocity outputs, there are 1k trim potentiometers to fine tune the output voltages.

## Gate, Trigger, and Clock Output

The gate, trigger and clock signals all run directly from the associated GPIO pins from the microcontroller.
Because these signals coming out of the Pi Pico are at 3.3v, a TL074 op amp is used to turn the pulses into 0 to +5v signals.

## User Input

The five button input consists of four directions (up/down, forward/back) and push to enter. 
All buttons also have dedicated hardware debouncing circuits to ensure simple and accurate signaling.

Initially, four individual buttons were used for the user input.
This proved to be a challenge to fit everything into a compact 12 hp space.
The decision to use a 5 button (four-way, plus push down) interface worked out in terms of both user friendliness and space savings as well.

## OLED Screen

The OLED screen used is a 0.96" display using the SSD1306 chipset.
These modules can be found at a reasonable price, and offer enough performance for the needs of this module.
Communication with the microcontroller is via an I2C bus.

## Power

The power coupling for the module uses a standard 16 pin (2x08) IDC connector as detailed in the original [A-100 Doefer power system bus](https://doepfer.de/a100_man/a100t_e.htm).

As measured, the power draw on the +5v, +12v, and -12v lines are:
* +5v: 70mA
* +12v: 20mA
* -12v: 15mA

# Parts List

[Bill of Materials, CSV version](../design/TechWaveAudio_MidiController_BOM.csv)

## Surface Mount Components

| ID                                   | Qty | Description                           | Package   |
|--------------------------------------|-----|---------------------------------------|-----------|
| U5,U6                                | 2   | MCP4725 single channel 12 bit I2C DAC | SOIC-14   |
| U1,U2                                | 2   | TL074 op amp                          | SOIC-14   |
| U4                                   | 1   | MCP4902 dual channel 8 bit SPI DAC    | TSSOP-14  |
| U3                                   | 1   | 40106 Hex Schmitt-trigger inverter    | TSSOP-14  |
| C1,C10,C11,C12,C15,C18 C20,C22,C23   | 9   | 100nf capacitor                       | SMD 0402  |
| C2,C8,C9,C13,C14,C16,C17,C19,C21,C24 | 10  | 10uf capacitor (polarized)            | SMD 0805  |
| C3,C4,C5,C6,C7                       | 5   | 1uf capacitor                         | SMD 0402  |
| D1,D2                                | 2   | Schottky diode (30v)                  | SOT-23    |
| D3,D4,D5,D6,D7,D8                    | 6   | 1N4148 diode                          | D_SOD-323 |
| R2 R4,R5,R6,R7,R27,R28,R35           | 8   | resistor 5.1k                         | SMD 0402  |
| R8,R9,R10,R36                        | 4   | resistor 10k                          | SMD 0402  |
| R11,R14,R24,R25,R31                  | 5   | resistor 1k                           | SMD 0402  |
| R12,R30,R38,R40,R42                  | 5   | resistor 4.7k                         | SMD 0402  |
| R13,R16,R18,R20,R22                  | 5   | resistor 100k                         | SMD 0402  |
| R15,R17,R19,R21,R23                  | 5   | resistor 220k                         | SMD 0402  |
| R26                                  | 1   | resistor 220 ohm                      | SMD 0402  |
| R32,R33,R34                          | 3   | resistor 330 ohm                      | SMD 0402  |

## Through Hole Components

| ID                     | Qty | Description                  | Mfgr            | Mfg Part #     | Link                   |
|------------------------|-----|------------------------------|-----------------|----------------|------------------------|
| J2                     | 1   | 16 pos 2x08 IDC pins (male)  | Samtec          | TST-108-02-T-D | https://mou.sr/4oMjFmU |
| R37,R39,R41,R43        | 4   | 1k trim pot                  | Bourns          | 3296X-1-102LF  | https://mou.sr/44u4OnX |
| OLED1                  | 1   | 1x04 socket connector        | Samtec          | SSW-104-01-T-S | https://mou.sr/43Qrwqd |
| J9                     | 1   | 1x06 socket connector        | Samtec          | SSW-106-01-T-S | https://mou.sr/4ahPiPr |
| J7,J12                 | 2   | 1x20 socket connector        |                 |                |                        |
| J1,J3,J4,J5,J6,J13,J14 | 7   | 3.5mm Jack                   |                 |                |                        |
| SW1                    | 1   | 4 position DIP switch        | TE Connectivity | 2454982-3      | https://mou.sr/3SZFLqg |
| --                     | 1   | 5 way button                 |                 |                |                        |
| U7                     | 1   | 6N137 Optocoupler            | LITEON          | 6N137          | https://mou.sr/4eIy9Qa |
| D10                    | 1   | green LED                    | Kingbright      | WP113GDT       | https://mou.sr/4oNpEIp |
| J10                    | 1   | MIDI jack                    |                 |                |                        |
| --                     | 1   | 0.96" SSD1306-based I2C OLED |                 |                |                        |
| A1                     | 1   | Raspberry Pi Pico            | Raspberry Pi    | SC0917         | https://mou.sr/41kRroA |
| D9                     | 1   | red LED                      | Kingbright      | WP113IDT       | https://mou.sr/4xOIRNK |
| D11                    | 1   | yellow LED                   | Kingbright      | WP113YDT       | https://mou.sr/4xOQVhs |

## Assembly Components
| ID | Qty | Description               |                        |
|----|-----|---------------------------|------------------------|
| -- | 5   | 11mm PCB Standoff 3M hole |                        |
| -- | 2   | 2M nut                    |                        |
| -- | 2   | 2M x 8mm screw            |                        |
| -- | 6   | 3M nut                    |                        |
| -- | 8   | 3M x 2mm spacer           | (3d printed or bought) |
| -- | 10  | 3M x 5mm screw            |                        |
| -- | 6   | 3M x 8mm screw            |                        |


# TechWave Audio Midi Controller Module Hardware Design

The hardware is based around an RP2040 based Raspberry Pi Pico.

To keep the design a bit more accessible, I made the conscience decision to base the hardware around the Raspberry Pi Pico module itself instead of opting for using a discrete RP2040.
Using a discrete RP2040 itself would have allowed for a much smaller footprint on the PCB, however the using the full module allowed for easier testing overall without having to worry about getting the microcontroller section of the circuitry right.


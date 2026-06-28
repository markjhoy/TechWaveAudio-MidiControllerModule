# Midi Controller Module Software Design


<img src="./images/midi_controller_arch.png" alt="Midi Controller Block Diagram" />

The functionality of the software can be broken up into several components across the two cores with two shared objects.

Core 0 handles the user input, display, and settings persistance. Core 1 handles the real-time MIDI input and hardware outputs.


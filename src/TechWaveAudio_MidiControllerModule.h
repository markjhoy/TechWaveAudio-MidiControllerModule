/*****************************
 * TechWave Audio (techwaveaudio.com)
 * Midi Controller Module definitions and defines
 *****************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_H

#include <functional>
#include <stdexcept>
#include <string>

#include "hardware/flash.h"

// ###########################################
// ### --- General Configuration Items --- ###
// ###########################################

#define DEBUG_BUILD true

#define TECHWAVEAUDIO_MCM_VERSION 1.0.0
#define TECHWAVEAUDIO_MCM_VERSION_STR "    v1.0.0"
#define TECHWAVEAUDIO_MCM_RELEASE_STR "release: 260625"

// use the last sector for our storage
#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)
// wear leveling for 1 sector
#define MAX_WEAR_LEVEL_PAGES (FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE)

// our invalid page marker
#define STATE_INVALID_PAGE 0xFFFFFFFF
// define the start page marker, also used for versioning
#define STATE_START_MARKER 0x0F91

#define USE_HARDWARE_DEBOUNCE true
// time in ms for button software debouncing
#define BTN_PIN_BOUNCE_TIME_MS 50

// our max number of octave ranges at +10v
#define MAX_CV_OCTAVE_RANGES 10.0f
// the number of semitones available at +10v
#define MAX_CV_NOTE_VALUES (12.0f * MAX_CV_OCTAVE_RANGES)

// ##################################
// ### --- Hardware pin setup --- ###
// ##################################

// the onboard GPIO pin number
#define ONBOARD_LED_PIN 25

// input pin for the back button
#define BUTTON_BACK_PIN 21
// input pin for the next button
#define BUTTON_NEXT_PIN 20
// input pin for the enter button
#define BUTTON_ENTER_PIN 19
// input pin for the up button
#define BUTTON_UP_PIN 26
// input pin for the down button
#define BUTTON_DOWN_PIN 22

// -- I2C pins --

// The SSD1306 (OLED Display) data and clock pins
#define OLED_I2C_DATA_PIN 2
#define OLED_I2C_CLOCK_PIN 3

// The MCP4725 (12 bit / single channel) DAC data and clock pins
#define DAC_4725_I2C_DATA_PIN 0
#define DAC_4725_I2C_CLOCK_PIN 1

// -- MCP4902 (8 bit / dual channel) DAC SPI pins --
#define DAC_4902_SPI_CLOCK_PIN 10
#define DAC_4902_SPI_TX_PIN 11
#define DAC_4902_SPI_RX_PIN 12
#define DAC_4902_SPI_CS_PIN 13

// The UART / MIDI pins
#define MIDI_IN_RX_PIN 5
#define MIDI_OUT_TX_PIN 4

// Out gate line pin
#define PIN_GATE_LINE 7
// Out clock line pin
#define PIN_CLOCK_LINE 8
// Out trigger line pin
#define PIN_TRIGGER_LINE 9

// The pin for the LED showing the clock pulse
#define PIN_CLOCK_LED 27
// The pin for the LED for showing when a note is pressed
#define PIN_NOTE_LED 28

// #####################################
// ### --- Display Configuration --- ###
// #####################################

// THe i2c, bus and address for the OLED
#define HW_OLED_I2C i2c1_inst
#define OLED_BUS_NUMBER 1
#define OLED_ADDRESS 0x3C

// I2C frequency for the OLED
#define OLED_BUS_HARDWARE_FREQ 800000
// number of pixes per character for the screen
#define OLED_PIXELS_PER_CHAR 8
// the width in pixels of the display
#define OLED_DISPLAY_WIDTH 128
// the height in pixels of the display
#define OLED_DISPLAY_HEIGHT 64
// total number of lines in the display
#define OLED_NUM_TEXT_LINES 4
// number of characters per line
#define OLED_NUM_CHARS_PER_LINE 16
// number of pixels per display line
#define OLED_PIXELS_PER_LINE (OLED_DISPLAY_HEIGHT / OLED_NUM_TEXT_LINES)
// number of pixels to shift the text down when displaying a line
#define OLED_LINE_TEXT_OFFSET 0

// the number of steps to display on the range control
#define RANGE_DISPLAY_STEPS 13

// default display dashboard when running
#define DEFAULT_SHOW_DASHBOARD 1

// default update time in ms
#define DEFAULT_DASHBOARD_UPDATE_FREQUENCY 100

// our available selections for setting the dashboard update frequency
#define NUM_DASHBOARD_UPDATE_VALUES 10
static uint32_t display_update_values[NUM_DASHBOARD_UPDATE_VALUES] = {
    50, 100, 150, 200, 250, 300, 350, 400, 450, 500
};

// how many ticks to toggle the clock LED
#define DEFAULT_CLOCK_TICK_LED_CYCLE 12
// our available selections for the clock LED tick frequency
#define NUM_CLOCK_TICK_LED_VALUES 7
static uint8_t clock_led_toggle_values[NUM_CLOCK_TICK_LED_VALUES] = {
    0, 1, 2, 3, 6, 12, 24
};

// ##################################
// ### --- MIDI configuration --- ###
// ##################################

// standard MIDI baud rate
#define MIDI_BAUD_RATE 31250
// the max a midi data value can be
#define MAX_MIDI_DATA_VALUE 128

// midi UART values
#define MIDI_UART_ID uart1
#define MIDI_UART_IRQ UART1_IRQ

// the default MIDI channel to use
#define DEFAULT_MIDI_CHANNEL 1

// the default pitch adjustment value
#define DEFAULT_PITCH_ADJUST 0.0f
#define MIN_PITCH_ADJUST (-250.0f)
#define MAX_PITCH_ADJUST 250.0f

// the default velocity adjustment value
#define DEFAULT_VELOCITY_ADJUST 0
#define MIN_VELOCITY_ADJUST (-250.0f)
#define MAX_VELOCITY_ADJUST 250.0f

// the default duration (in ms) of the trigger
#define DEFAULT_TRIGGER_DURATION 100
// The clock pulse time (in ms)
#define CLOCK_PULSE_MS 10

// default pitch bend maximum value (note)
#define DEFAULT_PITCH_BEND_RANGE_OCTAVES 1
// max number octaves the user can set the pitch bend range to
#define MAX_PITCH_BEND_RANGE_OCTAVES 5.0f

// pitch bend midpoint value is 8192
// this defines the +/- from this value to turn it off
#define PITCH_BEND_ZERO_TOLERANCE 256

// midi commands
#define MIDI_CMD_NOTE_OFF 0x80
#define MIDI_CMD_NOTE_ON 0x90
#define MIDI_CMD_POLY_AFTERTOUCH 0xA0
#define MIDI_CMD_CONTROL_CHANGE 0xB0
#define MIDI_CMD_CONTROL_CHANGE_MOD_WHEEL 0x01
#define MIDI_CMD_CONTROL_CHANGE_MOD_SUSTAIN 0x40
#define MIDI_CMD_CONTROL_CHANGE_MOD_VOLUME 0x07
#define MIDI_CMD_CONTROL_CHANGE_MOD_MUTE 0x7B
#define MIDI_CMD_CONTROL_CHANGE_EXPRESSION 0x0B
#define MIDI_CMD_CONTROL_CHANGE_EFFECT_1 0x0C
#define MIDI_CMD_CONTROL_CHANGE_EFFECT_2 0x0D
#define MIDI_CMD_PROGRAM_CHANGE 0xC0
#define MIDI_CMD_CHANNEL_AFTERTOUCH 0xD0
#define MIDI_CMD_PITCH_BEND 0xE0
#define MIDI_CMD_SYSEX 0xF0
#define MIDI_CMD_TIME_CODE_QTR 0xF1
#define MIDI_CMD_SONG_POSITION 0xF2
#define MIDI_CMD_SONG_SELECT 0xF3
#define MIDI_CMD_CLOCK_TICK 0xF8
#define MIDI_CMD_RESET 0xFF
#define MIDI_CMD_SYSEX_END 0xF7

// last note value for the output controller when a note is not active
#define DEFAULT_LAST_NOTE_VALUE 127

// our ranges for available midi notes at +5 and +10 volts
#define MIDI_MIN_NOTE_10V 0     /* C-1 */
#define MIDI_MAX_NOTE_10V 119   /* B9 */
#define MAX_NUM_NOTES_10V 120   /* 10 full octaves, 12 semitones each */
#define MIDI_MIN_NOTE_5V 36     /* C2 */
#define MIDI_MAX_NOTE_5V 95     /* B6 */
#define MAX_NUM_NOTES_5V 60     /* 5 full octaves, 12 semitones each */

// #################################
// ### --- DAC Configuration --- ###
// #################################

// THe i2c, bus and address for the MCP4725 (DAC / 12 bit) ICs
#define HW_DAC_4725_I2C i2c0_inst
#define HW_DAC_4725_I2C_BAUD_RATE 100000
#define DAC_4725_I2C_BUS_NUMBER 0

// address fpr the note 4725 DAC
#define DAC_NOTE_I2C_ADDRESS 0b01100000
// address fpr the velocity 4725 DAC
#define DAC_VELOCITY_I2C_ADDRESS 0b01100001

// Total number of steps for the 4725 DACs (12 bit)
#define DAC_4725_MAX_RANGE 4096

// which SPI bus for the 4902 DAC (aux/ctl, 8 bit)
#define DAC_4902_SPI_BUS spi1
// baud rate for the Mcp4902 DAC (aux/ctl, 8 bit)
#define DAC_4902_BAUD_RATE 100000
// Total number of steps for the 4092 DAC (aux/ctl, 8 bit)
#define DAC_4902_MAX_RANGE 256

// AUX register 0x30 (VAout) / CTL register 0xB0 (VBout)
#define DAC_4902_AUX_HIGH_BYTE 0b00110000
#define DAC_4902_CTL_HIGH_BYTE 0b10110000

// Maximum output voltage of the ADC
#define DAC_MAX_OUTPUT_VOLTS 5.0f
// volts per octave for the ADC
#define DAC_OUTPUT_VOLTS_PER_OCTAVE (DAC_MAX_OUTPUT_VOLTS / 5.0f)
// volts per note for the ADC
#define DAC_OUTPUT_VOLTS_PER_NOTE (DAC_OUTPUT_VOLTS_PER_OCTAVE / 12.0f)

// our default voltage ranges for the CV outputs
#define DEFAULT_VOLTS_OUTPUT_NOTE_DAC TenVoltOutput
#define DEFAULT_VOLTS_OUTPUT_VELOCITY_DAC TenVoltOutput
#define DEFAULT_VOLTS_OUTPUT_AUX_DAC TenVoltOutput
#define DEFAULT_VOLTS_OUTPUT_CTL_DAC TenVoltOutput

// ################################################
// ### -- macros, enums and typedefs, oh my --- ###
// ################################################

// callback function definitions
#define GeneralFunctionCallback std::function<void()>
#define TimerCallback std::function<void()>
#define OnPinValueChangeCallback std::function<void()>
#define RangeEditorCallback std::function<void(float)>
#define NoValueMidiMessageCallback std::function<void()>
#define SingleValueMidiMessageCallback std::function<void(uint8_t)>
#define DoubleValueMidiMessageCallback std::function<void(uint8_t, uint8_t)>

// this is good for up to at least 47 days (uint32) without a restart
#define GetTicksMs to_ms_since_boot(get_absolute_time())

// GPIO pin directions
enum PinDirection : uint8_t {
    PIN_DIRECTION_OUTPUT = 0,
    PIN_DIRECTION_INPUT = 1
};

// frame buffer modes for the OLED. Only page is used.
enum MonoLcdFramebufferMode : uint8_t {
    LCD_FRAMEBUFFER_MODE_HORIZONTAL = 0,
    LCD_FRAMEBUFFER_MODE_VERTICAL = 1,
    LCD_FRAMEBUFFER_MODE_PAGE = 2,
};

inline void ThrowError(const char *message) {
    throw std::runtime_error(message);
}

// our boot screen template
static std::string boot_screen_lines[4] = {
    std::string(" TechWave Audio "),
    std::string("Midi Controller "),
    std::string("     Module     "),
    std::string(TECHWAVEAUDIO_MCM_VERSION_STR)
};

// our dashboard string template
static std::string default_dashboard_template[4] = {
    "midi channel: --",
    "N:  ___ Vel: ___",
    "CC: ___ Aux: ___",
    " T: _ G: _ C: _ ",
};

typedef struct ScreenXYPos_t {
    int xPos = 0;
    int yPos = 0;
} ScreenXYPos;

typedef struct ScreenRectangle_t {
    int xPos = 0;
    int yPos = 0;
    int width = 0;
    int height = 0;
} ScreenRectangle;

// indexes to the positioning for our dashboard items
#define DASHBOARD_VALUE_CHANNEL 0
#define DASHBOARD_VALUE_NOTE 1
#define DASHBOARD_VALUE_VELOCITY 2
#define DASHBOARD_VALUE_CC 3
#define DASHBOARD_VALUE_AUX 4
#define DASHBOARD_VALUE_TRIGGER 5
#define DASHBOARD_VALUE_GATE 6
#define DASHBOARD_VALUE_CLOCK 7

// positions of our dashboard items in the text buffer
static ScreenXYPos dashboard_value_position[8] = {
    { 14, 0 },
    { 4, 1 },
    {13, 1},
    {4, 2},
    {13, 2},
    {4, 3},
    {9, 3},
    {14, 3}
};

// note names for display
static std::string note_names_display[12] = {
    "C ", "C#", "D ", "D#",
    "E ", "F ", "F#", "G ",
    "G#", "A ", "A#", "B "
};

// our dashboard state structure
typedef struct DashboardState_t {
    volatile uint8_t midiChannel = 0;
    volatile uint8_t currentNote = DEFAULT_LAST_NOTE_VALUE;
    volatile uint8_t currentVelocity = 0;
    volatile uint8_t currentAux = 0;
    volatile uint8_t currentCtl = 0;
    volatile bool triggerState = false;
    volatile bool gateState = false;
    volatile bool clockState = false;
} DashboardState;

// incoming octave range mappings (min note / max note)
typedef struct IncomingMidiNoteRange_t {
    int minNoteValue;
    int maxNoteValue;
} IncomingMidiNoteRange;

// our available output voltage values
enum CVOutputVoltsValue : uint8_t {
    TenVoltOutput = 0,
    FiveVoltOutput = 1,
};

// incoming note priority
enum NotePriorityType : uint8_t {
    NOTE_PRIORITY_LAST = 0,
    NOTE_PRIORITY_HIGHEST = 1,
    NOTE_PRIORITY_LOWEST = 2
};

#define DEFAULT_NOTE_PRIORITY NOTE_PRIORITY_LAST

// the setting of the aux function
enum AuxSettingType : uint8_t {
    AUX_SETTING_AFTERTOUCH = 0,
    AUX_SETTING_EXPRESSION = 1,
};

#define DEFAULT_AUX_SETTING AUX_SETTING_AFTERTOUCH

// the setting of the control function
enum ControlSettingType : uint8_t {
    CTL_SETTING_MOD_WHEEL = 0,
    CTL_SETTING_EFFECT_1 = 1,
    CTL_SETTING_EFFECT_2 = 2
};

#define DEFAULT_CONTROL_SETTING CTL_SETTING_MOD_WHEEL

// parsed midi message
typedef struct MidiMessage_t {
    uint8_t channel;
    uint8_t command;
    uint8_t data1;
    uint8_t data2;
} MidiMessage;

#define DIAGNOSTIC_SWEEP_SLEEP_MS 0
#define DIAGNOSTIC_PULSE_SLEEP_MS 250

// lookup tables for our note to CV data value mappings
inline uint16_t ten_volt_note_12_bit_output[MAX_NUM_NOTES_10V];
inline uint16_t five_volt_note_12_bit_output[MAX_NUM_NOTES_5V];
inline uint16_t ten_volt_linear_12_bit_output[MAX_MIDI_DATA_VALUE];
inline uint16_t ten_volt_8_bit_output[MAX_MIDI_DATA_VALUE];

// designates an invalid timer queue event id
#define INVALID_EVENT_ID 0xFFFFFFFF

// a timer queue message item node
typedef struct TimedEventItem_t {
    uint32_t eventId = INVALID_EVENT_ID;
    uint32_t expirationTime = 0;
    std::function<void()> callback = nullptr;
    TimedEventItem_t *previous = nullptr;
    TimedEventItem_t *next = nullptr;
} TimedEventItem;

// our available CV outputs
enum CVOutput: uint8_t {
    CVOutput_NONE = 0,
    CVOutput_Note = 1,
    CVOutput_Velocity = 2,
    CVOutput_Aux = 3,
    CVOutput_Control = 4,
};

// our cross-core communication commands
enum SignalCommand: uint8_t {
    SignalCommand_None = 0,
    SignalCommand_OutputOn = 1,
    SignalCommand_OutputOff = 2,
    SignalCommand_Shutdown = 3,
    SignalCommand_Shutdown_Ack = 4,
    SignalCommand_NoteChange = 5,
    SignalCommand_VelocityChange = 6,
    SignalCommand_AuxChange = 7,
    SignalCommand_ControlChange = 8,
    SignalCommand_ClockTick = 9,
    SignalCommand_TriggerPulse_On = 10,
    SignalCommand_TriggerPulse_Off = 11,
    SignalCommand_Gate_On = 12,
    SignalCommand_Gate_Off = 13,
    SignalCommand_Reset = 14,
};

// a cross-core signal message structure
typedef struct SignalMessage_t {
    SignalCommand command = SignalCommand_None;
    uint8_t data = 0;
} SignalMessage;

// maximum cross-core queue size
#define MAX_SIGNALS_IN_QUEUE 1024

#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_H
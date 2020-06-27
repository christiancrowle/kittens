//
// Created by devbat on 6/26/20.
//

#ifndef KITTENS_MIDICONTROL_H
#define KITTENS_MIDICONTROL_H

#include <rtmidi17/rtmidi17.hpp>

namespace Kittens::Core {
class MidiDevice {
   protected:
    rtmidi::midi_out device_output;
    rtmidi::midi_in device_input;  // FIXME: unimplemented
   public:
    std::vector<std::string> get_port_names();
    int port_name_to_num(std::string port_name);
    int get_num_out_ports();
    void select_out_port(std::string name);
    void select_out_port(int num);
};

// midi messages: see http://archive.is/2BHjr
/*
 * MIDI messages basically work like this:
 *  there is always one "status byte" that determines two things:
 *      - first nibble is the message type (as in table below)
 *      - second nibble is the channel number
 *  next come one or two data bytes, depending on the message. comments below show number for each message
 * message types:
 *  - voice message, the meat (note on, note off, etc)
 *  - channel mode message, controls for stuff like monophonic/polyphonic modes, etc. and panic
 *  - system message, stuff like clocking and sequencing.
 */
enum MidiMessageType {
    // Voice messages, these are low byte (last 4 bits are channel number)
    VM_NOTE_OFF = 0b10000000,           // 2 data bytes - key number and velocity
    VM_NOTE_ON = 0b10010000,            // 2 data bytes - key number and velocity
    VM_POLY_KEY_PRESSURE = 0b10100000,  // 2 data bytes - key number and amount of pressure
    VM_CONTROL_CHANGE = 0b10110000,     // 2 data bytes - controller number and controller value (note: see below)
    VM_PROGRAM_CHANGE = 0b11000000,     // 1 data byte  - the program number
    VM_CHANNEL_PRESSURE = 0b11010000,   // 1 data byte  - the pressure value
    VM_PITCH_BEND = 0b11100000,         // 2 data bytes - MSB and LSB

    // channel mode messages (first data byte of control change)
    CM_RESET_ALL = 0x79,      // 2nd data byte = 0
    CM_LOCAL_CONTROL = 0x7A,  // 2nd data byte = either 0 (off) or 127 (on)
    CM_PANIC = 0x7B,          // 2nd data byte = 0
    CM_OMNI_OFF = 0x7C,       // 2nd data byte = 0
    CM_OMNI_ON = 0x7D,        // 2nd data byte = 0
    CM_MONO_ON = 0x7E,  // 2nd data byte = if value is 0 then the number of channels used is determined by the receiver,
                        // otherwise it sets the specified number of channels.
    CM_POLY_ON = 0x7F,  // 2nd data byte = 0

    // system realtime messages (sync)
    SYSRT_CLOCK = 0xF8,
    SYSRT_START = 0xFA,
    SYSRT_CONTINUE = 0xFB,
    SYSRT_STOP = 0xFC,
    SYSRT_ACTIVE_SENSE = 0xFE,
    SYSRT_RESET = 0xFF

    // system common messages (random shit)
    SYSCOM_TIMING_CODE = 0xF1,    // 1 data byte
    SYSCOM_SONG_POSITION = 0xF2,  // 2 data bytes
    SYSCOM_SONG_SELECT = 0xF3,    // 1 data byte
    SYSCOM_TUNE_REQUEST = 0xF6,   // no data byte

    // SPECIAL CASE: system exclusive message (sysex)
    // these aren't standardized messages, they are basically a custom byte stream
    // surrounded by SYSEX_START and SYSEX_STOP
    SYSEX_START = 0xF0,
    SYSEX_STOP = 0xF7
};

class MidiMessage {};

std::vector<unsigned char> construct_message(MidiMessage m);
}  // namespace Kittens::Core

#endif  // KITTENS_MIDICONTROL_H

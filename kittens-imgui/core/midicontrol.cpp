//
// Created by devbat on 6/26/20.
//

#include "midicontrol.h"

namespace Kittens::Core {
std::vector<std::string> MidiDevice::get_port_names() {
    int count = this->get_num_out_ports();
    std::vector<std::string> out;

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            out.push_back(this->device_output.get_port_name(i));
        }
    }

    return out;
}

int MidiDevice::port_name_to_num(std::string port_name) {
    int count = this->get_num_out_ports();

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            if (this->device_output.get_port_name(i) == port_name)
                return i;
        }
    }
}

int MidiDevice::get_num_out_ports() {
    return this->device_output.get_port_count();
}

void MidiDevice::select_out_port(std::string name) {
    this->device_output.open_port(this->port_name_to_num(name));
}

void MidiDevice::select_out_port(int num) {
    this->device_output.open_port(num);
}

}  // namespace Kittens::Core
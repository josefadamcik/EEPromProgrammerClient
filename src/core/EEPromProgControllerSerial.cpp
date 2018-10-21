//
// Created by josef on 21.10.18.
//

#include "EEPromProgControllerSerial.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>



EEPromProgControllerSerial::~EEPromProgControllerSerial() {
    serial.reset();
}

EEPromProgControllerSerial::EEPromProgControllerSerial(std::unique_ptr<serial::Serial> serial)
        : serial(std::move(serial)) {}


void EEPromProgControllerSerial::send_cmd_help() {
    serial->write("h");
}

void EEPromProgControllerSerial::send_cmd_dump_segment(unsigned int segment) {

}

const std::array<unsigned char, 16> EEPromProgControllerSerial::send_cmd_read(unsigned int address) {
    serial->write(str( boost::format("r%1$04x") % address));

    return std::array<unsigned char, 16>();
}



void EEPromProgControllerSerial::send_cmd_write(unsigned int address,
                                                const std::array<unsigned char, write_buffer_size> &buffer) {
    serial->write(str( boost::format("w%1$04x") % address));
    for (const auto &val : buffer) {
        serial->write(str(boost::format("%1$02x") % (int)val));
    }
}


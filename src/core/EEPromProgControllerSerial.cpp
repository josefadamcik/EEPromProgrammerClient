//
// Created by josef on 21.10.18.
//

#include "EEPromProgControllerSerial.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <regex>
#include <stdexcept>


using std::string;


unsigned char hex_to_num(unsigned char hex) {
    if (hex >= 'a' && hex <= 'f') {
        return static_cast<unsigned char>(hex - 'a' + 10);
    } else if (hex >= 'A' && hex <= 'F') {
        return static_cast<unsigned char>(hex - 'A' + 10);
    } else if (hex >= '0' && hex <= '9') {
        return static_cast<unsigned char>(hex - '0');
    } else {
        return 0;
    }
}



EEPromProgControllerSerial::~EEPromProgControllerSerial() {
    serial.reset();
}

EEPromProgControllerSerial::EEPromProgControllerSerial(std::unique_ptr<serial::Serial> serial)
        : serial(std::move(serial)) {}


void EEPromProgControllerSerial::send_cmd_help() {
    serial->write("h");
}

std::unique_ptr<std::vector<std::vector<unsigned char>>>
EEPromProgControllerSerial::send_cmd_dump_segment(unsigned int segment) {
    serial->write(str( boost::format("d%1$02x") % segment));
    auto result = std::make_unique<std::vector<std::vector<unsigned char>>>();
    int expected_line_count = 16;
    int expected_byte_count = 16;

    for (int i = 0; i < expected_line_count; i++) {//we expect 16 lines
        (*result)[i].resize(expected_byte_count);
        string line;
        serial->readline(line);
        parse_read_line_to_vector(line, (*result)[i]);
    }

    return std::move(result);
}

std::unique_ptr<std::vector<unsigned char>>
EEPromProgControllerSerial::send_cmd_read(unsigned int address) {
    serial->write(str( boost::format("r%1$04x") % address));
    string line;
    serial->readline(line);
    auto result = std::make_unique<std::vector<unsigned char>>();

    parse_read_line_to_vector(line, *result);

    return std::move(result);
}



void EEPromProgControllerSerial::send_cmd_write(unsigned int address,
                                                const std::array<unsigned char, write_buffer_size> &buffer) {
    serial->write(str( boost::format("w%1$04x") % address));
    for (const auto &val : buffer) {
        serial->write(str(boost::format("%1$02x") % (int)val));
    }
}

void EEPromProgControllerSerial::parse_read_line_to_vector(const std::string &line,
                                                           std::vector<unsigned char> &result) {
    auto str_buffer = std::vector<string>();
    //abcd:  AB AB AB AB AB AB AB AB  AB AB AB AB AB AB AB AB
    std::regex dump_regex("\\s+([a-fA-F0-9]{2})"); //matches just pairs of hex bytes, shouldn't match the address at the beginning
    std::smatch dump_match;

    std::copy( std::sregex_token_iterator(line.begin(), line.end(), dump_regex, 1),
               std::sregex_token_iterator(),
               std::back_inserter(str_buffer));

    std::transform(str_buffer.begin(), str_buffer.end(), std::back_inserter(result), [] (string str_byte) {
        return hex_to_num(static_cast<unsigned char>(str_byte[0])) << 4
               | hex_to_num(static_cast<unsigned char>(str_byte[1]));
    });

}


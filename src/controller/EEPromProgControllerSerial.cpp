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
#include <core/hex_to_num.hpp>

using std::string;

static const int read_bytes_per_row = 16;
static const int read_lines_per_segment = 16;


EEPromProgControllerSerial::~EEPromProgControllerSerial() {
    serial.reset();
}

EEPromProgControllerSerial::EEPromProgControllerSerial(std::unique_ptr<serial::Serial> serial)
        : serial(std::move(serial)) {}


void EEPromProgControllerSerial::sendCmdHelp() {
    serial->write("h");
    waitForAck();
}

std::unique_ptr<std::vector<std::vector<unsigned char>>>
EEPromProgControllerSerial::sendCmdDumpSegment(unsigned int segment) {
    serial->write(str(boost::format("d%1$02x") % segment));
    auto result = std::make_unique<std::vector<std::vector<unsigned char>>>(read_lines_per_segment);
    for (int i = 0; i < read_lines_per_segment; i++) {//we expect 16 lines
        string line;
        serial->readline(line);
        parseReadLineToVector(line, result->at(i));
    }

    waitForAck();

    return std::move(result);
}

std::unique_ptr<std::vector<unsigned char>>
EEPromProgControllerSerial::sendCmdRead(unsigned int address) {
    serial->write(str(boost::format("r%1$04x") % address));
    string line;
    serial->readline(line);
    auto result = std::make_unique<std::vector<unsigned char>>();

    parseReadLineToVector(line, *result);
    waitForAck();

    return std::move(result);
}


void EEPromProgControllerSerial::sendCmdWrite(unsigned int address,
                                              const std::array<unsigned char, writeBufferSize> &buffer) {
    serial->write(str(boost::format("w%1$04x") % address));
    for (const auto &val : buffer) {
        serial->write(str(boost::format("%1$02x") % (int) val));
    }

    waitForAck();
}

void EEPromProgControllerSerial::parseReadLineToVector(const std::string &line,
                                                       std::vector<unsigned char> &result) {
    auto str_buffer = std::vector<string>();
    //abcd:  AB AB AB AB AB AB AB AB  AB AB AB AB AB AB AB AB
    //matches just pairs of hex bytes, shouldn't match the address at the beginning
    std::regex dump_regex("\\s+([a-fA-F0-9]{2})");
    std::smatch dump_match;

    std::copy(std::sregex_token_iterator(line.begin(), line.end(), dump_regex, 1),
              std::sregex_token_iterator(),
              std::back_inserter(str_buffer));

    std::transform(str_buffer.begin(), str_buffer.end(), std::back_inserter(result), [](string str_byte) {
        return hexToNum(static_cast<unsigned char>(str_byte[0])) << 4
               | hexToNum(static_cast<unsigned char>(str_byte[1]));
    });

    if (result.size() != read_bytes_per_row) {
        throw EEPromProgCtrlError("Invalid response from programmer");
    }

}

bool EEPromProgControllerSerial::isConnected() {
    return serial && serial->isOpen();
}

//
//const vector<string> Application::wait_for_done() {
//    vector<string> lines;
//    bool done = false;
//    bool error = false;
//    do {
//        string line_buffer;
//        size_t read = serial->readline(line_buffer);
//        out << read << " " << line_buffer << endl;
//        if (read > 0) {
//            done = boost::starts_with(line_buffer, "=DONE");
//            if (!done) {
//                lines.push_back(line_buffer);
//                error = boost::starts_with(line_buffer, "=E:");
//            }
//        }
//    } while (!done && !error);
//    return lines;
//}

void EEPromProgControllerSerial::waitForAck() {
    bool done = false;
    do {
        string lineBuffer;
        size_t read = serial->readline(lineBuffer);
        std::cout << "read: " << read << " " << lineBuffer << std::endl;
        if (read > 0) {
            done = boost::starts_with(lineBuffer, "=DONE");
            if (boost::starts_with(lineBuffer, "=E:")) {
                throw EEPromProgCtrlError("Programmer returned error: " + lineBuffer);
            }
        }
    } while (!done);

}




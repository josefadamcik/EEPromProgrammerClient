#include "eepromprog.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace std;

const unsigned int waiting_sleep = 1;

void Eepromprog::enumerate_ports() {
    vector<serial::PortInfo> devices_found = serial::list_ports();

    auto iter = devices_found.begin();

    while (iter != devices_found.end()) {
        serial::PortInfo device = *iter++;

        printf("(%s, %s, %s)\n", device.port.c_str(),
               device.description.c_str(), device.hardware_id.c_str());
    }
}




void Eepromprog::connect(string interface, int baudrate) {
    if (is_connected()) {
        disconnect();
    }
    serial = move(make_unique<serial::Serial>(interface, baudrate, serial::Timeout::simpleTimeout(1000)));

    out << "Is the serial port open?";
    if(serial->isOpen())
        out << " Yes." << endl;
    else
        out << " No." << endl;

}

void Eepromprog::dump_lines(const vector<string>& lines) {
    for (const auto &line: lines) {
        out << line;
    }
}

void Eepromprog::consume_input() {
    vector<string> lines = serial->readlines();
    for (const auto &line : lines) {
        out << "Got: " << line;
    }
}

const vector<string> Eepromprog::wait_for_done() {
    vector<string> lines;
    bool done = false;
    do {
        string line_buffer;
        size_t read = serial->readline(line_buffer);
        if (read > 0) {
            done = boost::starts_with(line_buffer, "=DONE");
            if (!done) {
                lines.push_back(line_buffer);
            }
        }
    } while (!done);
    return lines;
}

bool Eepromprog::is_connected() {
    return serial && serial->isOpen();
}

void Eepromprog::disconnect() {
    serial->close();
}

void Eepromprog::do_stuff() {

    vector<string> lines;
    consume_input();
    serial->write("h");
    lines = wait_for_done();
    dump_lines(lines);
    serial->write("d00");
    lines = wait_for_done();
    dump_lines(lines);
    //write something

    for (int i = 0; i < 16; i++) {
        serial->write(str( boost::format("w%1$04x") % 15 ));
        for (int c = 0; c < 16; c++) {
            serial->write(str(boost::format("%1$x") % 0xAA));
        }
    }
    lines = wait_for_done();
    dump_lines(lines);
    serial->write(str( boost::format("d%1$02x") % 0 ));
    lines = wait_for_done();
    dump_lines(lines);
    serial->close();


}

Eepromprog::~Eepromprog() {
    if (is_connected()) {
        disconnect();
    }
    if (serial) {
        serial.release();
    }
}


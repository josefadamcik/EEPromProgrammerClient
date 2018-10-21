#include "Application.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace std;

const unsigned int waiting_sleep = 1;

void Application::enumerate_ports() {
    vector<serial::PortInfo> devices_found = serial::list_ports();

    auto iter = devices_found.begin();

    while (iter != devices_found.end()) {
        serial::PortInfo device = *iter++;

        printf("(%s, %s, %s)\n", device.port.c_str(),
               device.description.c_str(), device.hardware_id.c_str());
    }
}




void Application::connect(string interface, int baudrate) {
    if (is_connected()) {
        disconnect();
    }

    auto new_serial = make_unique<serial::Serial>(interface, baudrate, serial::Timeout::simpleTimeout(1000));
    serial = move(new_serial);

    out << "Is the serial port open?";
    if(serial->isOpen())
        out << " Yes." << endl;
    else
        out << " No." << endl;

}

void Application::dump_lines(const vector<string>& lines) {
    for (const auto &line: lines) {
        out << line;
    }
}

void Application::consume_input() {
    vector<string> lines = serial->readlines();
    dump_lines(lines);
}

const vector<string> Application::wait_for_done() {
    vector<string> lines;
    bool done = false;
    bool error = false;
    do {
        string line_buffer;
        size_t read = serial->readline(line_buffer);
        out << read << " " << line_buffer << endl;
        if (read > 0) {
            done = boost::starts_with(line_buffer, "=DONE");
            if (!done) {
                lines.push_back(line_buffer);
                error = boost::starts_with(line_buffer, "=E:");
            }
        }
    } while (!done && !error);
    return lines;
}

bool Application::is_connected() {
    return serial && serial->isOpen();
}

void Application::disconnect() {
    serial->close();
}


void Application::do_stuff() {

    vector<string> lines;
    consume_input();
    send_cmd_help();
//    lines = wait_for_done();
//    dump_lines(lines);
//    send_cmd_dump_segment(0x00);
//    lines = wait_for_done();
//    dump_lines(lines);
//    send_cmd_read(0x7000);
//    lines = wait_for_done();
//    dump_lines(lines);
//    //write something
////    for (unsigned int i = 0; i < 16; i++) {
////        send_cmd_write(0x6000 + i * 0x10, {0xBC, 0xBC,0xBC,0xBC,0xBC,0xBC, 0xBC, 0xBC, 0xBC, 0xBC, 0xBC , 0xBC, 0xBC, 0xBC ,0xBC ,0xBC});
////        lines = wait_for_done();
////        dump_lines(lines);
////    }
//    send_cmd_dump_segment(0x70);
//    lines = wait_for_done();
//    dump_lines(lines);
}

Application::~Application() {
    if (is_connected()) {
        disconnect();
    }
    if (serial) {
        serial.reset();
    }
}

void Application::send_cmd_help() {
    serial->write("h");
}

void Application::send_cmd_write(unsigned int address, const std::array<unsigned char , write_buffer_size> &buffer) {
    serial->write(str( boost::format("w%1$04x") % address));
    out << "write to " << address << hex << endl;
    out << "OK"<< endl;
    for (const auto &val : buffer) {
        out << "write " << (int)val << hex;
        serial->write(str(boost::format("%1$x") % (int)val));
    }
    out << endl;
}

void Application::send_cmd_dump_segment(unsigned int segment) {
    //segment = segment << 8;
    serial->write(str( boost::format("d%1$02x") % segment));

}

const array<unsigned char, 16> Application::send_cmd_read(unsigned int address) {
    auto buffer = array<unsigned char, read_buffer_size>();
    serial->write(str( boost::format("r%1$04x") % address));
    return buffer;
}


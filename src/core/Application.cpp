#include "Application.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <controller/EEPromProgControllerSerial.hpp>
#include <filereader/HexDataFileReader.hpp>

using namespace std;

const unsigned int waiting_sleep = 1;

void Application::enumeratePorts() {
    vector<serial::PortInfo> devices_found = serial::list_ports();

    auto iter = devices_found.begin();

    while (iter != devices_found.end()) {
        serial::PortInfo device = *iter++;

        printf("(%s, %s, %s)\n", device.port.c_str(),
               device.description.c_str(), device.hardware_id.c_str());
    }
}


void Application::connect(string interface, int baudrate) {
    if (isConnected()) {
        disconnect();
    }

    auto new_serial = make_unique<serial::Serial>(interface, baudrate, serial::Timeout::simpleTimeout(1000));
    if (!new_serial->isOpen()) {
        throw ConnectionException("Failed to open serial");
    }
    std::unique_ptr<EEPromProgController> new_controller = make_unique<EEPromProgControllerSerial>(std::move(new_serial));
    eePromProgController = std::move(new_controller);

}

void Application::dumpLines(const vector<string> &lines) {
    for (const auto &line: lines) {
        out << line;
    }
}

bool Application::isConnected() {
    return eePromProgController && eePromProgController->isConnected();
}

void Application::disconnect() {
    eePromProgController.reset();
}


Application::~Application() {
    if (isConnected()) {
        disconnect();
    }
}

void
Application::programFromHexfile(std::basic_string<char, std::char_traits<char>, std::allocator<char>> filename) {
    if (!isConnected()) {
        throw ConnectionException("Not connected to serial.");
    }
    HexDataFileReader reader(filename);
    auto const& data = reader.read();
    for (auto const& dataItem : data) {
        eePromProgController->sendCmdWrite(dataItem.address, dataItem.data);
    }

}



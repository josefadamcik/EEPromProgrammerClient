#include <serial/serial.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>

using namespace std;
namespace po = boost::program_options;

void enumerate_ports() {
    vector<serial::PortInfo> devices_found = serial::list_ports();

    vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while (iter != devices_found.end()) {
        serial::PortInfo device = *iter++;

        printf("(%s, %s, %s)\n", device.port.c_str(),
               device.description.c_str(), device.hardware_id.c_str());
    }
}

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("list", "list serial interfaces");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    if (vm.count("list")) {
        cout << "Listing serial interfaces:" << endl;
        enumerate_ports();
        return 0;
    }

    cout << desc << endl;
}

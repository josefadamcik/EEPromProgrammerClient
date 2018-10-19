#include <boost/program_options.hpp>
#include <iostream>
#include <core/eepromprog.hpp>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    Eepromprog eeprom_programmer(cout);
    po::options_description desc("Usage:");
    desc.add_options()
        ("help,h", "produce help message")
        ("list,l", "list serial interfaces")
        ("file,f", po::value<string>(), "binary file that should be written into EEPROM")
        ("interface,i", po::value<string>(), "serial interface to use")
        ("baudrate,b", po::value<int>()->default_value(57600), "baudrate")
        ;
    po::positional_options_description positional;
    positional.add("interface", 1);
    positional.add("file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
        .options(desc)
        .positional(positional)
        .run(), 
        vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    if (vm.count("list")) {
        cout << "Listing serial interfaces:" << endl;
        eeprom_programmer.enumerate_ports();
        return 0;
    }

    if (vm.count("file") && vm.count("interface")) {
        cout << "Source file: " << vm["file"].as<string>() << endl;
        cout << "Interface: " << vm["interface"].as<string>() << endl;
        //110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000 and 256000
        int baudrate = vm["baudrate"].as<int>();
        cout << "Baudrate: " << baudrate << endl;
        eeprom_programmer.connect(vm["interface"].as<string>(), baudrate);
        eeprom_programmer.do_stuff();
        return 0;
    }

    cout << desc << endl;
}

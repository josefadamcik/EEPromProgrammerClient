#include <iostream>
#include <serial/serial.h>
#include <memory>

#ifndef INCLEEPROMPROP
#define INCLEEPROMPROP


class Eepromprog {
public:
    Eepromprog() : out(std::cout) {}

    virtual ~Eepromprog();

    explicit Eepromprog(std::ostream &out) : out(out) {}

    void enumerate_ports();

    void connect(std::string interface, int baudrate);

    void do_stuff();

    bool is_connected();

    void disconnect();
private:
    std::ostream &out;

    std::unique_ptr<serial::Serial> serial;

    void consume_input();

    const std::vector<std::string> wait_for_done();

    void dump_lines(const std::vector<std::string> &lines);
};


#endif
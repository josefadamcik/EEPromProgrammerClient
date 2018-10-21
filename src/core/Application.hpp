#include <iostream>
#include <serial/serial.h>
#include <memory>
#include <vector>
#include <array>

#ifndef INCLEEPROMPROP
#define INCLEEPROMPROP


class Application {
public:

    Application() : out(std::cout) {}

    virtual ~Application();

    explicit Application(std::ostream &out) : out(out) {}

    void enumerate_ports();

    void connect(std::string interface, int baudrate);

    void do_stuff();

    bool is_connected();

    void disconnect();
private:
    const static int write_buffer_size = 16;
    const static int read_buffer_size = 16;
    std::ostream &out;

    std::unique_ptr<serial::Serial> serial;

    void consume_input();

    const std::vector<std::string> wait_for_done();

    void dump_lines(const std::vector<std::string> &lines);

    void send_cmd_help();
    /**
     * @param segment - two most significand bytes of address
     */
    void send_cmd_dump_segment(unsigned int segment);
    const std::array<unsigned char, read_buffer_size> send_cmd_read(unsigned int address);
    void send_cmd_write(unsigned int address, const std::array<unsigned char, write_buffer_size> &buffer);


};

#endif
//
// Created by josef on 21.10.18.
//

#ifndef EEPROMPROGCLIENT_EEPROMPROGCONTROLLERSERIAL_HPP
#define EEPROMPROGCLIENT_EEPROMPROGCONTROLLERSERIAL_HPP


static const int expected_size = 16;

#include <memory>
#include <serial/serial.h>
#include "EEPromProgController.hpp"

class EEPromProgControllerSerial : public EEPromProgController {
public:
    explicit EEPromProgControllerSerial(std::unique_ptr<serial::Serial> serial);
    ~EEPromProgControllerSerial() override;
    void
    send_cmd_help() override;

    std::unique_ptr<std::vector<std::vector<unsigned char>>>
    send_cmd_dump_segment(unsigned int segment) override;

    std::unique_ptr<std::vector<unsigned char>>
    send_cmd_read(unsigned int address) override;

    void
    send_cmd_write(unsigned int address, const std::array<unsigned char, write_buffer_size> &buffer) override;


private:
    std::unique_ptr<serial::Serial> serial;

    void parse_read_line_to_vector(const std::string &line, std::vector<unsigned char> &result);
};


#endif //EEPROMPROGCLIENT_EEPROMPROGCONTROLLERSERIAL_HPP

//
// Created by josef on 21.10.18.
//

#ifndef EEPROMPROGCLIENT_EEPROMPROGCONTROLLERSERIAL_HPP
#define EEPROMPROGCLIENT_EEPROMPROGCONTROLLERSERIAL_HPP


#include <memory>
#include <serial/serial.h>
#include "EEPromProgController.hpp"

class EEPromProgControllerSerial : public EEPromProgController {
public:
    explicit EEPromProgControllerSerial(std::unique_ptr<serial::Serial> serial);
    ~EEPromProgControllerSerial() override;
    void
    send_cmd_help() override;

    void
    send_cmd_dump_segment(unsigned int segment) override;

    const std::array<unsigned char, 16>
    send_cmd_read(unsigned int address) override;

    void
    send_cmd_write(unsigned int address, const std::array<unsigned char, write_buffer_size> &buffer) override;


private:
    std::unique_ptr<serial::Serial> serial;
};


#endif //EEPROMPROGCLIENT_EEPROMPROGCONTROLLERSERIAL_HPP

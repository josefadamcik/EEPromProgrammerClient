#include "array"
#include "memory"

//
// Created by josef on 21.10.18.
//

#ifndef EEPROMPROGCLIENT_EEPROMPROGCONTROLLER_HPP
#define EEPROMPROGCLIENT_EEPROMPROGCONTROLLER_HPP

const static int write_buffer_size = 16;
const static int read_buffer_size = 16;

class EEPromProgController {
public:
    virtual void send_cmd_help() = 0;

    /**
    * @param segment - two most significand bytes of address
    */
    virtual void
    send_cmd_dump_segment(unsigned int segment) = 0;
    /**
     * Read 16 bytes from EEPROM
     * @param address
     * @return
     */
    virtual std::unique_ptr<std::vector<unsigned char>>
    send_cmd_read(unsigned int address) = 0;

    /**
     * Writes 16bytes to EEPROM
     * @param address
     * @param buffer
     */
    virtual void
    send_cmd_write(unsigned int address, const std::array<unsigned char, write_buffer_size> &buffer) = 0;
    virtual
    ~EEPromProgController() = default;
};


#endif //EEPROMPROGCLIENT_EEPROMPROGCONTROLLER_HPP
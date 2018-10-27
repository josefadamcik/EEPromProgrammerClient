#include "array"
#include "memory"
#include "vector"

//
// Created by josef on 21.10.18.
//

#ifndef EEPROMPROGCLIENT_EEPROMPROGCONTROLLER_HPP
#define EEPROMPROGCLIENT_EEPROMPROGCONTROLLER_HPP

const static int writeBufferSize = 16;
const static int readBufferSize = 16;

class EEPromProgCtrlError: public std::logic_error {
    public:
    explicit
    EEPromProgCtrlError(const std::string& msg) : std::logic_error(msg) {};
};

/**
 * An interface for a facade for remote device. Allows
 * client to call commands. Implementation will use serial
 */
class EEPromProgController {
public:
    virtual void sendCmdHelp() = 0;

    /**
    * @param segment - two most significand bytes of address
    */
    virtual std::unique_ptr<std::vector<std::vector<unsigned char>>>
    sendCmdDumpSegment(unsigned int segment) = 0;
    /**
     * Read 16 bytes from EEPROM
     * @param address
     * @return
     */
    virtual std::unique_ptr<std::vector<unsigned char>>
    sendCmdRead(unsigned int address) = 0;

    /**
     * Writes 16bytes to EEPROM
     * @param address
     * @param buffer
     */
    virtual void
    sendCmdWrite(unsigned int address, const std::array<unsigned char, writeBufferSize> &buffer) = 0;
    virtual
    ~EEPromProgController() = default;

    virtual
    bool isConnected() = 0;

    virtual
    void waitForAck() = 0;

};


#endif //EEPROMPROGCLIENT_EEPROMPROGCONTROLLER_HPP

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
    sendCmdHelp() override;

    std::unique_ptr<std::vector<std::vector<unsigned char>>>
    sendCmdDumpSegment(unsigned int segment) override;

    std::unique_ptr<std::vector<unsigned char>>
    sendCmdRead(unsigned int address) override;

    void
    sendCmdWrite(unsigned int address, const std::array<unsigned char, writeBufferSize> &buffer) override;

    bool
    isConnected() override;
private:
    std::unique_ptr<serial::Serial> serial;

    void parseReadLineToVector(const std::string &line, std::vector<unsigned char> &result);

    void waitForAck();
};


#endif //EEPROMPROGCLIENT_EEPROMPROGCONTROLLERSERIAL_HPP

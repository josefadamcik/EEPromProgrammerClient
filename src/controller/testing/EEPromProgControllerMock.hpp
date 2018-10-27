//
// Created by josef on 24.10.18.
//

#include <gmock/gmock.h>
#include <controller/EEPromProgController.hpp>

#ifndef EEPROMPROGCLIENT_EEPROMPROGCONTROLLERMOCK_HPP
#define EEPROMPROGCLIENT_EEPROMPROGCONTROLLERMOCK_HPP


class EEPromProgControllerMock: public EEPromProgController {
    public:
    MOCK_METHOD0(sendCmdHelp, void());

    MOCK_METHOD0(waitForAck, void());

    MOCK_METHOD0(isConnected, bool());

    MOCK_METHOD1(sendCmdDumpSegment, std::unique_ptr<std::vector<std::vector<unsigned char>>>(unsigned int
            segment));

    MOCK_METHOD1(sendCmdRead, std::unique_ptr<std::vector<unsigned char>>(unsigned int
            address));

    MOCK_METHOD2(sendCmdWrite, void(unsigned int
            address,
            const std::array<unsigned char, writeBufferSize> &buffer));


};


#endif //EEPROMPROGCLIENT_EEPROMPROGCONTROLLERMOCK_HPP

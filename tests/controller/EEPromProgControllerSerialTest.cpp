#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "controller/EEPromProgControllerSerial.hpp"
#include <memory>
#include <serial/serial.h>
#include <array>
#include <boost/bind.hpp>

#if defined(__linux__)
#include <pty.h>
#else
#include <util.h>
#endif


using std::string;

namespace {

    class EEPromProgControllerSerialTest : public ::testing::Test {
    protected:
        void SetUp() override {
            if (openpty(&masterFd, &slaveFd, name, NULL, NULL) == -1) {
                perror("openpty");
                exit(127);
            }

            ASSERT_TRUE(masterFd > 0);
            ASSERT_TRUE(slaveFd > 0);
            ASSERT_TRUE(string(name).length() > 0);

            controller = std::make_unique<EEPromProgControllerSerial>(std::move(std::make_unique<serial::Serial>(
                    name,
                    57600,
                    serial::Timeout::simpleTimeout(1000))
                    ));
        }

        void TearDown() override {
            controller.reset();
        }

        ssize_t readFromMockSerial(char *buffer, ssize_t size) {
            ssize_t readCount = 0;
            char oneCharBuff[1] = "";
            while (readCount < size) {
                readCount += read(masterFd, oneCharBuff, 1);
                buffer[readCount - 1] = oneCharBuff[0];
            }
            return readCount;
        }

        void arrangeMockReadResultLine() {
            string mockResultLine = "abcd:  00 01 02 03 04 05 06 07  FF FE FD FC FB FA F9 F8\n";
            write(masterFd, mockResultLine.c_str(), mockResultLine.length());
        }

        void arrangeAckReadResultLine() {
            string ackLine = "=DONE\n";
            write(masterFd, ackLine.c_str(), ackLine.length());
        }

        void arrangeErrorReadResultLine() {
            string ackLine = "=E:error\n";
            write(masterFd, ackLine.c_str(), ackLine.length());
        }

        void arrangeMultipleMockReadResultLines(int count) {
            for (int i = 0; i < count; i++) {
                arrangeMockReadResultLine();
            }
        }


        std::unique_ptr<EEPromProgControllerSerial> controller;
        int masterFd;
        int slaveFd;
        char name[100];
    };


    TEST_F(EEPromProgControllerSerialTest, TestSendHelpCommand) {
        char buf[1] = "";

        arrangeAckReadResultLine();
        controller->sendCmdHelp();

        read(masterFd, buf, 1);

        EXPECT_EQ(string(buf), string("h"));
    }


    TEST_F(EEPromProgControllerSerialTest, TestSendWriteCommand) {
        const int expectedCharCount = 1 + 4 /* command, address in hex */ + 16 * 2 /*2 char hex per byte*/;
        char buf[expectedCharCount] = "";

        arrangeAckReadResultLine();
        controller->sendCmdWrite(0xABCD,
                                 {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF});

        auto  readCount = readFromMockSerial(buf, expectedCharCount);

        EXPECT_EQ(readCount, expectedCharCount);
        EXPECT_EQ(string(buf, readCount), string("wabcd000102030405060708090a0b0c0d0e0f"));
    }

    TEST_F(EEPromProgControllerSerialTest, TestWriteCommandFailsWhenErrorInsteadOfAck) {
        const int expectedCharCount = 1 + 4 /* command, address in hex */ + 16 * 2 /*2 char hex per byte*/;
        char buf[expectedCharCount] = "";

        arrangeErrorReadResultLine();
        try {
            controller->sendCmdWrite(0xABCD,
                                     {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF});
            FAIL();
        } catch (EEPromProgCtrlError& exc) {
            SUCCEED();
        }
    }


    TEST_F(EEPromProgControllerSerialTest, TestSendReadCommand) {
        const int expectedCharCount = 1 + 4 /* command, address in hex*/;
        char buf[expectedCharCount] = "";

        arrangeMockReadResultLine();
        arrangeAckReadResultLine();

        controller->sendCmdRead(0xABCD);

        ssize_t readCount = read(masterFd, buf, expectedCharCount);

        EXPECT_EQ(readCount, expectedCharCount);
        EXPECT_EQ(string(buf, expectedCharCount), string("rabcd"));
    }

    TEST_F(EEPromProgControllerSerialTest, TestReadCommandProcessesResult) {
        std::vector<unsigned char> expectedParsedResult{
            0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
            0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8
        };


        arrangeMockReadResultLine();
        arrangeAckReadResultLine();

        auto parsedResult = controller->sendCmdRead(0xABCD);

        EXPECT_EQ(parsedResult->size(), expectedParsedResult.size());
        EXPECT_EQ(*parsedResult, expectedParsedResult);
    }

    TEST_F(EEPromProgControllerSerialTest, TestReadCommandFailsWhenIncompleteData) {
        string mockResultLine = "abcd:  00 01 02 03 04 05 06 07  FF FE F\n";
        write(masterFd, mockResultLine.c_str(), mockResultLine.length());
        arrangeAckReadResultLine();
        try {
            controller->sendCmdRead(0xABCD);
            FAIL();
        } catch (EEPromProgCtrlError& exc) {
            EXPECT_TRUE(1);
        }
    }



    TEST_F(EEPromProgControllerSerialTest, TestSendDumpCommand) {
        const int expectedCharCount = 1 + 2 /* command, address in hex*/;
        char buf[expectedCharCount] = "";

        arrangeMultipleMockReadResultLines(16);
        arrangeAckReadResultLine();

        controller->sendCmdDumpSegment(0x11);

        ssize_t readCount = readFromMockSerial(buf, expectedCharCount);

        EXPECT_EQ(readCount, expectedCharCount);
        EXPECT_EQ(string(buf, expectedCharCount), string("d11"));
    }

    TEST_F(EEPromProgControllerSerialTest, TestDumpCommandProcessesResult) {
        std::vector<unsigned char> expectedParsedResult{
                0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
                0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8
        };
        int mockResultCount = 16;
        arrangeMultipleMockReadResultLines(mockResultCount);
        arrangeAckReadResultLine();

        auto result = controller->sendCmdDumpSegment(0x11);

        //one item for each row of output. Normally this would be 16 rows
        EXPECT_EQ(result->size(), mockResultCount);
        for (int i = 0; i < mockResultCount; i++) {
            EXPECT_EQ(result->at(i).size(), expectedParsedResult.size());
            EXPECT_EQ(result->at(i), expectedParsedResult);
        }
    }

    TEST_F(EEPromProgControllerSerialTest, TestDumpCommandFailsWhenIncompleteData) {
        int mockResultCount = 16;
        string mockResultLine = "abcd:  00 01 02 03 04 05 06 07  FF FE F\n";
        for (int i = 0; i < mockResultCount; i++) {
            if (i % 2 == 0) {
                arrangeMockReadResultLine();
            } else {
                write(masterFd, mockResultLine.c_str(), mockResultLine.length());
            }
        }

        try {
            controller->sendCmdDumpSegment(0x11);
            FAIL();
        } catch (EEPromProgCtrlError& exc) {
            EXPECT_TRUE(1);
        }
    }
}  // namespace


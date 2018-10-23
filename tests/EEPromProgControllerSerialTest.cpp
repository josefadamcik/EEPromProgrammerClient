#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "core/EEPromProgControllerSerial.hpp"
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
            if (openpty(&master_fd, &slave_fd, name, NULL, NULL) == -1) {
                perror("openpty");
                exit(127);
            }

            ASSERT_TRUE(master_fd > 0);
            ASSERT_TRUE(slave_fd > 0);
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

        ssize_t ReadPty(char * buffer, size_t size) {
            ssize_t read_count = 0;
            char onechar_buff[1] = "";
            while (read_count < size) {
                read_count += read(master_fd, onechar_buff, 1);
                buffer[read_count - 1] = onechar_buff[0];
            }
            return read_count;
        }

        std::unique_ptr<EEPromProgControllerSerial> controller;
        int master_fd;
        int slave_fd;
        char name[100];
    };


    TEST_F(EEPromProgControllerSerialTest, TestSendHelpCommand) {
        char buf[1] = "";

        controller->send_cmd_help();

        read(master_fd, buf, 1);

        EXPECT_EQ(string(buf), string("h"));
    }


    TEST_F(EEPromProgControllerSerialTest, TestSendWriteCommand) {
        const int expected_char_count = 1 + 4 /* command, address in hex */ + 16 * 2 /*2 char hex per byte*/;
        char buf[expected_char_count] = "";

        controller->send_cmd_write(0xABCD, {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF});

        auto  read_count = ReadPty(buf, expected_char_count);

        EXPECT_EQ(read_count, expected_char_count);
        EXPECT_EQ(string(buf, read_count), string("wabcd000102030405060708090a0b0c0d0e0f"));
    }



    TEST_F(EEPromProgControllerSerialTest, TestSendReadCommand) {
        const int expectedCharCount = 1 + 4 /* command, address in hex*/;
        char buf[expectedCharCount] = "";

        controller->send_cmd_read(0xABCD);

        ssize_t read_count = read(master_fd, buf, expectedCharCount);

        EXPECT_EQ(read_count, expectedCharCount);
        EXPECT_EQ(string(buf, expectedCharCount), string("rabcd"));
    }

    TEST_F(EEPromProgControllerSerialTest, TestReadCommandProcessesResult) {
        const int expectedCharCount = 1 + 4 /* command, address in hex*/;
        char buf[expectedCharCount] = "";
        string result = "abcd:  00 01 02 03 04 05 06 07  FF FE FD FC FB FA F9 F8\nl";
        std::vector<unsigned char> expected_parsed_result{
            0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7,
            0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8
        };

        write(master_fd, result.c_str(), result.length());

        auto parsed_result = controller->send_cmd_read(0xABCD);


        EXPECT_EQ(parsed_result->size(), expected_parsed_result.size());
        EXPECT_EQ(*parsed_result, expected_parsed_result);
    }
}  // namespace

//int main(int argc, char **argv) {
//    try {
//        ::testing::InitGoogleTest(&argc, argv);
//        return RUN_ALL_TESTS();
//    } catch (std::exception &e) {
//        std::cerr << "Unhandled Exception: " << e.what() << std::endl;
//    }
//    return 1;
//}

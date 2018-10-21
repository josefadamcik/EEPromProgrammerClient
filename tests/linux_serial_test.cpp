#include <string>
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "core/Application.hpp"

#include <boost/bind.hpp>

#if defined(__linux__)
#include <pty.h>
#else
#include <util.h>
#endif


using std::string;

namespace {

    class SerialTests : public ::testing::Test {
    protected:
        void SetUp() override {
            if (openpty(&master_fd, &slave_fd, name, NULL, NULL) == -1) {
                perror("openpty");
                exit(127);
            }



            ASSERT_TRUE(master_fd > 0);
            ASSERT_TRUE(slave_fd > 0);
            ASSERT_TRUE(string(name).length() > 0);

            std::cout << name << std::endl;

        }

        void TearDown() override {
            if (eepromprog.is_connected()) {
                eepromprog.disconnect();
            }
        }

        Application eepromprog;
        int master_fd;
        int slave_fd;
        char name[100];
    };

    TEST_F(SerialTests, TestEnumeratePorts) {
        testing::internal::CaptureStdout();
        eepromprog.enumerate_ports();
        std::string output = testing::internal::GetCapturedStdout();
        ASSERT_THAT(output, ::testing::HasSubstr("/dev/tty"));
        std::cout << output << std::endl;
    }

    TEST_F(SerialTests, TestConnect) {
        testing::internal::CaptureStdout();
        eepromprog.connect(name, 57600);
        std::string output = testing::internal::GetCapturedStdout();
        ASSERT_THAT(output, ::testing::HasSubstr("Is the serial port open? Yes."));
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

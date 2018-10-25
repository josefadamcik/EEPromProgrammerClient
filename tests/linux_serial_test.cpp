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
            if (openpty(&master_fd, &slave_fd, name, nullptr, nullptr) == -1) {
                perror("openpty");
                exit(127);
            }


            ASSERT_TRUE(master_fd > 0);
            ASSERT_TRUE(slave_fd > 0);
            ASSERT_TRUE(string(name).length() > 0);

            std::cout << name << std::endl;

        }

        void TearDown() override {
            if (application.isConnected()) {
                application.disconnect();
            }
        }

        Application application;
        int master_fd;
        int slave_fd;
        char name[100];
    };

    TEST_F(SerialTests, TestEnumeratePorts) {
        testing::internal::CaptureStdout();
        application.enumeratePorts();
        std::string output = testing::internal::GetCapturedStdout();
        ASSERT_THAT(output, ::testing::HasSubstr("/dev/tty"));
        std::cout << output << std::endl;
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

#include <iostream>
#include <serial/serial.h>
#include <memory>
#include <vector>
#include <array>
#include <controller/EEPromProgController.hpp>
#include <gtest/gtest_prod.h>

#ifndef INCLEEPROMPROP
#define INCLEEPROMPROP


class Application {
public:
    explicit Application(std::ostream &out = std::cout)
            : out(out) {}
    virtual ~Application();

    void enumeratePorts();

    void connect(std::string interface, int baudrate);

    void doStuff();

    bool isConnected();

    void disconnect();

    class ConnectionException : public std::runtime_error {
    public:
        explicit ConnectionException(const std::string& msg) : std::runtime_error(msg) {}
    };
private:
    std::ostream &out;
    std::unique_ptr<EEPromProgController> eePromProgController;
    void dumpLines(const std::vector<std::string> &lines);
    FRIEND_TEST(ApplicationTest, TestDumpBlock);
};

#endif
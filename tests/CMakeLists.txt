# simple tests
add_executable(simpletest simpletest.cpp)
add_test(simple simpletest)

set_property(
    TEST simple
    PROPERTY PASS_REGULAR_EXPRESSION "^Test!"
)

# gtest
# find_package(GTest REQUIRED)

add_executable(AllTests
        simplegtest.cpp
        linux_serial_test.cpp
        controller/EEPromProgControllerSerialTest.cpp
        core/ApplicationTest.cpp
        )
target_link_libraries(AllTests gmock gmock_main EepromProgCore)
target_link_libraries(AllTests util)

add_test(AllTestsInFoo simplegtest)


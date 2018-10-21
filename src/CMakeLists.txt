add_library(EepromProgCore
        core/Application.hpp
        core/Application.cpp
        core/EEPromProgController.hpp core/EEPromProgControllerSerial.cpp core/EEPromProgControllerSerial.hpp)

target_include_directories(
    EepromProgCore
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(EEPROMProgClient main.cpp)

target_link_libraries(EepromProgCore
  PRIVATE 
    serial Boost::boost 
)
target_link_libraries(EEPROMProgClient 
  PRIVATE 
    Boost::boost Boost::program_options EepromProgCore
)


# SImple tests for executable

add_test(noparam_helpdisplayed EEPROMProgClient)
set_property(
    TEST noparam_helpdisplayed
    PROPERTY PASS_REGULAR_EXPRESSION "^Usage:"
)


add_test(paramhelp_helpdisplayed EEPROMProgClient "--help")
set_property(
    TEST paramhelp_helpdisplayed
    PROPERTY PASS_REGULAR_EXPRESSION "^Usage:"
)

add_test(paramfile_filenamedisplayed EEPROMProgClient "--file=hi.hex --interface=tty0")
set_property(
    TEST paramfile_filenamedisplayed
    PROPERTY PASS_REGULAR_EXPRESSION "hi.hex"
)



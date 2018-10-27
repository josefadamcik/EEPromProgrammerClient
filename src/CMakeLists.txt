add_library(EepromProgCore
        core/Application.hpp
        core/Application.cpp
        core/hex_to_num.hpp
        core/hex_to_num.cpp
        controller/EEPromProgController.hpp
        controller/EEPromProgControllerSerial.cpp
        controller/EEPromProgControllerSerial.hpp
        controller/testing/EEPromProgControllerMock.hpp
        filereader/DataFileItem.hpp
        filereader/DataFileReader.hpp
        filereader/HexDataFileReader.cpp
        filereader/HexDataFileReader.hpp
        )

target_include_directories(
    EepromProgCore
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_executable(eepromprog
        main.cpp
        )

target_link_libraries(EepromProgCore
  PRIVATE 
    serial Boost::boost 
)
target_link_libraries(eepromprog
  PRIVATE 
    Boost::boost Boost::program_options EepromProgCore
)




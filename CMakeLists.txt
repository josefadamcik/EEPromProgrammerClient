cmake_minimum_required(VERSION 3.0.0)
project(EEPROMProgClient VERSION 0.1.0)

include(CTest)
enable_testing()


add_subdirectory(tests)

add_executable(EEPROMProgClient main.cpp)


find_package(Boost COMPONENTS REQUIRED program_options)
# if(Boost_FOUND)
#   include_directories(${Boost_INCLUDE_DIRS})
#   target_link_libraries(EEPROMProgClient boost_program_options)
# endif()

# prefer targets over library paths
target_link_libraries(EEPROMProgClient 
  PRIVATE 
    serial Boost::boost Boost::program_options 
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.0.0)
project(EEPROMProgClient VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Boost)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(EEPROMProgClient main.cpp)
endif()



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

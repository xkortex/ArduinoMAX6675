cmake_minimum_required(VERSION 3.6)
project (thermocouple)

#add_library(avr /usr/lib/avr/)
#find_package(avr REQUIRED)

include_directories(/usr/lib/avr
        /usr/lib/avr/include
        /usr/share/arduino/hardware/
        /usr/share/arduino/hardware/arduino/cores/arduino
        /usr/share/arduino/libraries
#        ${CMAKE_SOURCE_DIR}/LUFA
        )

FIND_LIBRARY(avr_LIBRARIES avr /usr/lib/avr)
FIND_LIBRARY(arduino_LIBRARIES arduino /usr/share/arduino/hardware/arduino/cores/arduino)


#add_subdirectory(LUFA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES serialthermocouple.ino serialthermocouple.h max6675a.cpp max6675a.h )

add_executable(thermocouple ${SOURCE_FILES})
#target_link_libraries(LyrebirdFirmware ${avr_LIBRARIES})
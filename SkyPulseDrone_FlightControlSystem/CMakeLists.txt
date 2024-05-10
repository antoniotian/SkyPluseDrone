cmake_minimum_required(VERSION 3.14)

project(SkyPulseDrone_FlightControlSystem LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network)
find_package(OpenCV REQUIRED)
find_library(PIGPIO_LIBRARY NAMES pigpio REQUIRED)

# Add the DRIVER and SENSOR directories to the include path.
include_directories(
    ${CMAKE_SOURCE_DIR}/SYSTEM
    ${CMAKE_SOURCE_DIR}/SENSORS
    ${CMAKE_SOURCE_DIR}/DRIVER
    ${CMAKE_SOURCE_DIR}/COMM)

add_executable(SkyPulseDrone_FlightControlSystem
  SYSTEM/main.cpp SYSTEM/mainsystem.h SYSTEM/mainsystem.cpp
  SYSTEM/sensormanager.h SYSTEM/sensormanager.cpp
  COMM/tcp.h COMM/tcp.cpp
  SENSORS/mpu6050.h SENSORS/mpu6050.cpp
  SENSORS/bmp180.h SENSORS/bmp180.cpp
  SENSORS/qmc5883.h SENSORS/qmc5883.cpp
  DRIVER/i2cdriver.h DRIVER/i2cdriver.cpp
)
target_link_libraries(SkyPulseDrone_FlightControlSystem
    ${PIGPIO_LIBRARY}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network)

install(TARGETS SkyPulseDrone_FlightControlSystem
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

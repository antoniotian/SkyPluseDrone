cmake_minimum_required(VERSION 3.5)

project(SkyPulseDrone_FlightControlSystem VERSION 0.1 LANGUAGES CXX)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent Network Sql Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent Network Sql Multimedia)
find_package(OpenCV REQUIRED)
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_library(PIGPIO_LIBRARY NAMES pigpio REQUIRED)

# Add the DRIVER and SENSOR directories to the include path.
include_directories(
    ${PROJECT_SOURCE_DIR}/usr/local/include/eigen-3.4.0
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/ALGORITHM
    ${CMAKE_SOURCE_DIR}/COMM
    ${CMAKE_SOURCE_DIR}/DRIVER
    ${CMAKE_SOURCE_DIR}/SENSOR
    ${CMAKE_SOURCE_DIR}/SYSTEM
    ${CMAKE_SOURCE_DIR}/MIDDLEWARES)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        ALGORITHM/kalmanfilter.h ALGORITHM/kalmanfilter.cpp

        COMM/tcp.h COMM/tcp.cpp

        DRIVER/i2cdriver.h DRIVER/i2cdriver.cpp

        SENSOR/mpu6050.h SENSOR/mpu6050.cpp
        SENSOR/bmp180.h SENSOR/bmp180.cpp
        SENSOR/qmc5883.h SENSOR/qmc5883.cpp

        SYSTEM/sensormanager.h SYSTEM/sensormanager.cpp

        Resource.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SkyPulseDrone_FlightControlSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SkyPulseDrone_FlightControlSystem APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SkyPulseDrone_FlightControlSystem SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SkyPulseDrone_FlightControlSystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SkyPulseDrone_FlightControlSystem PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Multimedia
    ${OpenCV_LIBS}
    ${PIGPIO_LIBRARY}
    Eigen3::Eigen)

set_target_properties(SkyPulseDrone_FlightControlSystem PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS SkyPulseDrone_FlightControlSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SkyPulseDrone_FlightControlSystem)
endif()

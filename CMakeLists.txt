cmake_minimum_required(VERSION 3.6)

project(as)

find_package(Boost COMPONENTS system REQUIRED)

add_library(bulk commandblock.cpp observers.cpp interpreter.cpp bulkhandler.cpp)
add_library(async SHARED async.cpp async.h)

target_link_libraries(async bulk)

add_executable(async_ser main.cpp)

target_link_libraries(async_ser
        async
        ${Boost_LIBRARIES}
        )

set_target_properties(async_ser async bulk  PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        # COMPILE_DEFINITIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;"
        )

#target_link_libraries(async bulk)

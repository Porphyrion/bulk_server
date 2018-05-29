cmake_minimum_required(VERSION 3.6)

project(Bulk_server) # VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_library(async async.cpp commandblock.cpp observers.cpp interpreter.cpp bulkhandler.cpp)

add_executable(bulk_server main.cpp)

target_link_libraries(bulk_server
        async
        ${Boost_LIBRARIES}
        )

set_target_properties(bulk_server async   PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;"
        )

include(GNUInstallDirs)

install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT ivanbasurman88@yandex.ru)

include(CPack)

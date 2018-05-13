cmake_minimum_required(VERSION 3.6)

project(Bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS system REQUIRED)

add_library(bulk commandblock.cpp observers.cpp interpreter.cpp bulkhandler.cpp)
add_library(async SHARED async.cpp async.h)

target_link_libraries(async bulk)

add_executable(bulk_server main.cpp)

target_link_libraries(bulk_server
        async
        ${Boost_LIBRARIES}
        )

set_target_properties(bulk_server async bulk  PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        # COMPILE_DEFINITIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;"
        )

include(GNUInstallDirs)
set_target_properties(async PROPERTIES PUBLIC_HEADER async.h)
set_target_properties(async PROPERTIES PRIVATE_HEADER bulk.h)
install(TARGETS async LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}  PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT ivanbasurman88@yandex.ru)

include(CPack)

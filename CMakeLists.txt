cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(async VERSION ${PROJECT_VERSION})

set(SOURCES
    src/main.cpp
    src/Parser.cpp
    src/async.cpp
)

set(HEADERS
    include/logger.h
    include/Parser.h
    include/async.h
)

add_library(async SHARED ${SOURCES})

target_include_directories(async PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(async PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

add_executable(myproject ${SOURCES})

set_target_properties(myproject PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(myproject PRIVATE "${CMAKE_BINARY_DIR}")

target_link_libraries(myproject PRIVATE async)

if (MSVC)
    target_compile_options(async PRIVATE /W4)
else()
    target_compile_options(async PRIVATE -Wall -Wextra -pedantic)
endif()


set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Install path prefix" FORCE)


include(GNUInstallDirs)


install(TARGETS async
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}          
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libasync
)


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/async)



set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)


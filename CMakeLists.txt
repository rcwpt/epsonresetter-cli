cmake_minimum_required(VERSION 3.20)
project(rcwpt-cli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings and optimizations
if (MSVC)
    add_compile_options(/W4 /permissive- /utf-8 /EHsc)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN NOMINMAX UNICODE _UNICODE)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
    add_compile_definitions(UNICODE _UNICODE)
endif()

include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/cli/cli_parser.cpp
    src/terminal/dashboard.cpp
    src/terminal/interactive_menu.cpp
    src/printer/detection.cpp
    src/printer/spooler.cpp
    src/usb/usb_discovery.cpp
    src/counter/counter_reader.cpp
    src/maintenance/maintenance_engine.cpp
    src/backup/backup_engine.cpp
    src/database/model_db.cpp
    src/security/audit.cpp
    src/credentials/credential_mgr.cpp
    src/logging/logger.cpp
)

add_executable(rcwpt ${SOURCES})

# Windows Native Libraries
if (WIN32)
    target_link_libraries(rcwpt PRIVATE
        setupapi
        winspool
        advapi32
        crypt32
        user32
        shell32
    )
endif()

# Installation target
install(TARGETS rcwpt DESTINATION bin)
install(DIRECTORY database/ DESTINATION database)

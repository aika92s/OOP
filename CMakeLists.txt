cmake_minimum_required(VERSION 3.10)
project(MinesweeperLab VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CLANG_FORMAT "clang-format")
find_program(CPPLINT "cpplint")

find_package(SFML REQUIRED system window graphics)
if(SFML_FOUND)
    message(STATUS "SFML found. Linking modules: ${SFML_LIBRARIES}")
else()
    message(FATAL_ERROR "SFML not found! Please install libsfml-dev or check your configuration.")
endif()

if(POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(minesweeper ${SOURCES} ${HEADERS}
        include/GameDifficulty.h
        include/GameDifficulty.h
        src/SettingsFactory.cpp
        include/SettingsFactory.h
        src/BombCell.cpp
        include/BombCell.h
        src/EmptyCell.cpp
        include/EmptyCell.h
        src/NumberedCell.cpp
        include/Colors.h
)

target_link_libraries(minesweeper PRIVATE sfml-graphics sfml-window sfml-system)

target_include_directories(minesweeper PRIVATE include)

set_target_properties(minesweeper PROPERTIES
        OUTPUT_NAME "minesweeper"
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

if(CLANG_FORMAT)
    add_custom_target(style-check
            COMMAND ${CLANG_FORMAT} --style=Google --dry-run --Werror
            src/*.cpp include/*.h
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_command(TARGET minesweeper PRE_BUILD
            COMMAND ${CLANG_FORMAT} --style=Google -i src/*.cpp include/*.h
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Auto-formatting code"
    )
endif()

if(CPPLINT)
    add_custom_target(lint-check
            COMMAND ${CPPLINT} --filter=-build/include_subdir,-build/header_guard
            src/*.cpp include/*.h
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

if(CLANG_FORMAT AND CPPLINT)
    add_custom_target(check-all
            DEPENDS style-check lint-check
            COMMENT "Running all code style checks"
    )
endif()


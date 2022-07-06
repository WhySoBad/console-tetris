CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(PROJECT_NAME console-tetris)
PROJECT(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

# FIND_LIBRARY(Curses REQUIRED)
# INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

ADD_EXECUTABLE(${PROJECT_NAME} src/game.cpp src/main.cpp src/tetris.cpp src/helper/TetrisHelper.cpp src/helper/TetrisHelper.h src/helper/DrawHelper.cpp src/helper/DrawHelper.h src/helper/FontHelper.cpp src/helper/FontHelper.h src/helper/ConfigHelper.cpp src/helper/ConfigHelper.h)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CURSES_LIBRARIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ncurses)
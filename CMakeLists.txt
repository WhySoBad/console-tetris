CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(PROJECT_NAME console-tetris)
PROJECT(${PROJECT_NAME})

SET(CMAKE_CXX_STANDARD 17)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

ADD_EXECUTABLE(${PROJECT_NAME} src/Game.cpp src/main.cpp src/Tetris.cpp src/helper/TetrisHelper.cpp src/helper/TetrisHelper.h src/helper/DrawHelper.cpp src/helper/DrawHelper.h src/helper/FontHelper.cpp src/helper/FontHelper.h src/helper/ConfigHelper.cpp src/helper/ConfigHelper.h src/geometry/Point.cpp src/geometry/Point.h src/geometry/BoundingBox.cpp src/geometry/BoundingBox.h src/tetromino/TetrominoI.cpp src/tetromino/TetrominoI.h src/tetromino/TetrominoJ.cpp src/tetromino/TetrominoJ.h src/tetromino/TetrominoL.cpp src/tetromino/TetrominoL.h src/tetromino/TetrominoO.cpp src/tetromino/TetrominoO.h src/tetromino/TetrominoS.cpp src/tetromino/TetrominoS.h src/tetromino/TetrominoT.cpp src/tetromino/TetrominoT.h src/tetromino/TetrominoZ.cpp src/tetromino/TetrominoZ.h)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ncurses)


#ifndef CONSOLE_TETRIS_TETROMINOI_H
#define CONSOLE_TETRIS_TETROMINOI_H


#include "../Tetris.h"
#include "../helper/TetrisHelper.h"

class TetrominoI: public Tetris {
public:
    void rotate() override;

    explicit TetrominoI(int startX = 0, int startY = START_Y);
};


#endif //CONSOLE_TETRIS_TETROMINOI_H

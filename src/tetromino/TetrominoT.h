#ifndef CONSOLE_TETRIS_TETROMINOT_H
#define CONSOLE_TETRIS_TETROMINOT_H


#include "../Tetris.h"
#include "../helper/TetrisHelper.h"

class TetrominoT: public Tetris {
public:
    void rotate() override;

    explicit TetrominoT(int startX = 0, int startY = START_Y);
};


#endif //CONSOLE_TETRIS_TETROMINOT_H

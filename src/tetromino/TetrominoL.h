#ifndef CONSOLE_TETRIS_TETROMINOL_H
#define CONSOLE_TETRIS_TETROMINOL_H


#include "../Tetris.h"
#include "../helper/TetrisHelper.h"

class TetrominoL : public Tetris {
public:
    void rotate() override;

    explicit TetrominoL(int startX = 0, int startY = START_Y);
};


#endif //CONSOLE_TETRIS_TETROMINOL_H

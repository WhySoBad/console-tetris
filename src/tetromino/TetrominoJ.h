#ifndef CONSOLE_TETRIS_TETROMINOJ_H
#define CONSOLE_TETRIS_TETROMINOJ_H


#include "../Tetris.h"
#include "../helper/TetrisHelper.h"

class TetrominoJ: public Tetris {
public:
    void rotate() override;

    explicit TetrominoJ(int startX = 0, int startY = START_Y);

};


#endif //CONSOLE_TETRIS_TETROMINOJ_H

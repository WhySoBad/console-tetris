#ifndef CONSOLE_TETRIS_TETROMINOO_H
#define CONSOLE_TETRIS_TETROMINOO_H


#include "../Tetris.h"
#include "../helper/TetrisHelper.h"

class TetrominoO: public Tetris {
public:
    void rotate() override;

    explicit TetrominoO(int startX = 0, int startY = START_Y);
};


#endif //CONSOLE_TETRIS_TETROMINOO_H

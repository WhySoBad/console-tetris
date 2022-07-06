#ifndef CONSOLE_TETRIS_TETROMINOZ_H
#define CONSOLE_TETRIS_TETROMINOZ_H


#include "../Tetris.h"
#include "../helper/TetrisHelper.h"

class TetrominoZ : public Tetris {
public:
    void rotate() override;

    explicit TetrominoZ(int startX = 0, int startY = START_Y);
};


#endif //CONSOLE_TETRIS_TETROMINOZ_H

#include "TetrominoO.h"
#include "memory.h"

void TetrominoO::rotate() {
    // no functionality because a square can't rotate
}

TetrominoO::TetrominoO(int startX, int startY): Tetris(startX, startY, o) {
    Point pts[4] = {Point(startX, startY + 1), Point(startX + 1, startY + 1), Point(startX, startY), Point(startX + 1, startY)};
    memcpy(points, pts, sizeof pts);
    width = 2;
    height = 2;
}

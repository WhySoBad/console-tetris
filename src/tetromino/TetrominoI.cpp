#include "TetrominoI.h"
#include "memory.h"

void TetrominoI::rotate()  {
    if (step == 4) step = 1;
    else step++;

    height += width;
    width = height - width;
    height -= width;

    x = step % 2 == 0 ? x + 1 : x - 1;
    y = step % 2 == 0 ? y - 2 : y + 2;
    for (int i = 0; i < 4; i++) {
        points[i].setX(step % 2 == 0 ? x : x + i);
        points[i].setY(step % 2 == 0 ? y + i : y);
    }
}

TetrominoI::TetrominoI(int startX, int startY): Tetris(startX, startY, i) {
    Point pts[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 3, startY)};
    memcpy(points, pts, sizeof pts);
    width = 4;
    height = 1;
}

#include "TetrominoL.h"
#include "memory.h"

void TetrominoL::rotate() {
    if (step == 4) step = 1;
    else step++;

    height += width;
    width = height - width;
    height -= width;

    x = step == 1 ? x - 1 : step == 4 ? x + 1 : x;
    y = step == 1 ? y + 1 : step == 2 ? y - 1 : y;
    for (int i = 0; i < 3; i++) {
        points[i].setX(step % 2 == 1 ? x + i : step == 2 ? x + 1 : x);
        points[i].setY(step % 2 == 0 ? y + i : step == 1 ? y : y + 1);
    }
    points[3].setX((step == 1 || step == 2) ? x : step == 3 ? x + 2 : x + 1);
    points[3].setY((step == 2 || step == 3) ? y : step == 1 ? y + 1 : y + 2);
}

TetrominoL::TetrominoL(int startX, int startY) : Tetris(startX, startY, l) {
    Point pts[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 2, startY + 1)};
    memcpy(points, pts, sizeof pts);
    width = 3;
    height = 2;
}

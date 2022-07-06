#include "TetrominoZ.h"
#include "memory.h"

void TetrominoZ::rotate() {
    if (step == 4) step = 1;
    else step++;

    height += width;
    width = height - width;
    height -= width;

    y = step % 2 == 0 ? y - 1 : y + 1;
    points[0].setX(x);
    points[0].setY(step % 2 == 1 ? y : y + 2);
    points[1].setX(step % 2 == 1 ? x + 2 : x + 1);
    points[1].setY(step % 2 == 1 ? y + 1 : y);
    points[2].setX(step % 2 == 1 ? x + 1 : x);
    points[2].setY(y + 1);
    points[3].setX(x + 1);
    points[3].setY(step % 2 == 0 ? y + 1 : y);
}

TetrominoZ::TetrominoZ(int startX, int startY) : Tetris(startX, startY, z) {
    Point pts[4] = {Point(startX + 1, startY + 1), Point(startX + 2, startY + 1), Point(startX, startY), Point(startX + 1, startY)};
    memcpy(points, pts, sizeof pts);
    width = 3;
    height = 2;
}

#include "Tetris.h"
#include "array"

using namespace std;

Tetris::Tetris(int startX, int startY, Tetromino type) : BoundingBox(startX, startY, 0, 0) {
   this->type = static_cast<Tetromino>(type);
   step = 1;
}

array<Point, 4> Tetris::getPoints() {
   array<Point, 4> pts;
   move(begin(points), end(points), pts.begin());
   return pts;
}

Tetromino Tetris::getType() const { return type; }

void Tetris::moveX(int dx) {
   x += dx;
   for (auto & point : points) point.moveX(dx);
}

void Tetris::moveY(int dy) {
   y += dy;
   for (auto & point : points) point.moveY(dy);
}

int Tetris::getRotation() const { return step; }

void Tetris::rotate() {
}

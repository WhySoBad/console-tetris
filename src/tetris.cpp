#include "tetris.h"

#include <memory.h>

#include <array>
#include <iostream>
#include <string>
using namespace std;

Point::Point(int x = 0, int y = 0) {
   _x = x;
   _y = y;
}

const int Point::getX() { return _x; }

const int Point::getY() { return _y; }

void Point::setX(int x) { _x = x; }

void Point::setY(int y) { _y = y; }

void Point::moveX(int dx) { _x += dx; }

void Point::moveY(int dy) { _y += dy; }

BoundingBox::BoundingBox(int _x, int _y, int _height, int _width) {
   x = _x;
   y = _y;
   width = _width;
   height = _height;
}

int BoundingBox::getX() { return x; }

int BoundingBox::getY() { return y; }

int BoundingBox::getHeight() { return height; }

int BoundingBox::getWidth() { return width; }

Tetris::Tetris(int startX = 0, int startY = 0, Tetromino type = (Tetromino)0) : BoundingBox(startX, startY, 0, 0) {
   switch (type) {
      case 0:  // I-Tetromino
      {
         Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 3, startY)};
         memcpy(_points, points, sizeof(points));
         width = 4;
         height = 1;
         _type = Tetromino::i;
         break;
      }
      case 1:  // J-Tetromino
      {
         Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 2, startY + 1)};
         memcpy(_points, points, sizeof(points));
         width = 3;
         height = 2;
         break;
      }
      case 2:  // L-Tetromino
      {
         Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 2, startY + 1)};
         memcpy(_points, points, sizeof(points));
         width = 3;
         height = 2;
         break;
      }
      case 3:  // O-Tetromino
      {
         Point points[4] = {Point(startX, startY + 1), Point(startX + 1, startY + 1), Point(startX, startY), Point(startX + 1, startY)};
         memcpy(_points, points, sizeof(points));
         width = 2;
         height = 2;
         break;
      }
      case 4:  // S-Tetromino
      {
         Point points[4] = {Point(startX, startY + 1), Point(startX + 1, startY + 1), Point(startX + 1, startY), Point(startX + 2, startY)};
         memcpy(_points, points, sizeof(points));
         width = 3;
         height = 2;
         break;
      }
      case 5:  // T-Tetromino
      {
         Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 1, startY + 1)};
         memcpy(_points, points, sizeof(points));
         width = 3;
         height = 2;
         break;
      }
      case 6:  // Z-Tetromino
      {
         Point points[4] = {Point(startX + 1, startY + 1), Point(startX + 2, startY + 1), Point(startX, startY), Point(startX + 1, startY)};
         memcpy(_points, points, sizeof(points));
         width = 3;
         height = 2;
         break;
      }
   }
   _type = (Tetromino)type;
   step = 1;
}

array<Point, 4> Tetris::getPoints() {
   array<Point, 4> points;
   move(begin(_points), end(_points), points.begin());
   return points;
}

const Tetromino Tetris::getType() { return _type; }

void Tetris::moveX(int dx) {
   x += dx;
   for (int i = 0; i < 4; i++) _points[i].moveX(dx);
}

void Tetris::moveY(int dy) {
   y += dy;
   for (int i = 0; i < 4; i++) _points[i].moveY(dy);
}

int Tetris::getRotation() { return step; }

void Tetris::rotate()  // rotate the tetris-object 90deg to the right
{
   if (_type == o) return;  // the o-Tetromino can't be rotated because it's a square
   if (step == 4) {
      step = 1;
   } else {
      step++;
   }

   // swap height and width
   height += width;
   width = height - width;
   height -= width;

   if (_type == j || _type == t || _type == l) {
      x = step == 1 ? x - 1 : step == 4 ? x + 1 : x;
      y = step == 1 ? y + 1 : step == 2 ? y - 1 : y;
      for (int i = 0; i < 3; i++) {
         _points[i].setX(step % 2 == 1 ? x + i : step == 2 ? x + 1 : x);
         _points[i].setY(step % 2 == 0 ? y + i : step == 1 ? y : y + 1);
      }
      if (_type == j) {
         _points[3].setX((step == 2 || step == 3) ? x : step == 1 ? x + 2 : x + 1);
         _points[3].setY((step == 3 || step == 4) ? y : step == 1 ? y + 1 : y + 2);
      } else if (_type == l) {
         _points[3].setX((step == 1 || step == 2) ? x : step == 3 ? x + 2 : x + 1);
         _points[3].setY((step == 2 || step == 3) ? y : step == 1 ? y + 1 : y + 2);
      } else if (_type == t) {
         _points[3].setX(step == 2 ? x : x + 1);
         _points[3].setY(step == 3 ? y : y + 1);
      }

   } else if (_type == s || _type == z) {
      y = step % 2 == 0 ? y - 1 : y + 1;
      if (_type == s) {
         _points[0].setX(x);
         _points[0].setY(step % 2 == 1 ? y + 1 : y);
         _points[1].setX(step % 2 == 1 ? x + 2 : x + 1);
         _points[1].setY(step % 2 == 1 ? y : y + 2);
      } else if (_type == z) {
         _points[0].setX(x);
         _points[0].setY(step % 2 == 1 ? y : y + 2);
         _points[1].setX(step % 2 == 1 ? x + 2 : x + 1);
         _points[1].setY(step % 2 == 1 ? y + 1 : y);
      }
      _points[2].setX(step % 2 == 1 ? x + 1 : x);
      _points[2].setY(y + 1);
      _points[3].setX(x + 1);
      _points[3].setY(step % 2 == 0 ? y + 1 : y);
   } else if (_type == i) {
      x = step % 2 == 0 ? x + 1 : x - 1;
      y = step % 2 == 0 ? y - 2 : y + 2;
      for (int i = 0; i < 4; i++) {
         _points[i].setX(step % 2 == 0 ? x : x + i);
         _points[i].setY(step % 2 == 0 ? y + i : y);
      }
   }
}

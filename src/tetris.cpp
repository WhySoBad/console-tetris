#include "tetris.h"
#include <string>
#include <iostream>
#include <array>
#include <memory.h>
using namespace std;

Point::Point(unsigned int x = 0, unsigned int y = 0)
{
  _x = x;
  _y = y;
}

const unsigned int Point::getX()
{
  return _x;
}

const unsigned int Point::getY()
{
  return _y;
}

void Point::setX(unsigned int x)
{
  _x = x;
}

void Point::setY(unsigned int y)
{
  _y = y;
}

void Point::moveX(unsigned int dx)
{
  _x += dx;
}

void Point::moveY(unsigned int dy)
{
  _y += dy;
}

Tetris::Tetris(unsigned int startX = 0, unsigned int startY = 0)
{
  int variant = rand() % 7;
  switch (variant)
  {
  case 0: // I-Tetromino
  {
    Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 3, startY)};
    memcpy(_points, points, sizeof(points));
    _color = "\u001b[36;1m"; // cyan
    _type = i;
    break;
  }
  case 1: // J-Tetromino
  {
    Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX, startY - 1)};
    memcpy(_points, points, sizeof(points));
    _color = "\u001b[34;1m"; // blue
    _type = j;
    break;
  }
  case 2: // L-Tetromino
  {
    Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 2, startY - 1)};
    memcpy(_points, points, sizeof(points));
    _color = "\u001b[31;1m"; // orange
    _type = l;
    break;
  }
  case 3: // O-Tetromino
  {
    Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX, startY - 1), Point(startX + 1, startY - 1)};
    memcpy(_points, points, sizeof(points));
    _color = "\u001b[43m"; // yellow
    _type = o;
    break;
  }
  case 4: // S-Tetromino
  {
    Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 1, startY - 1), Point(startX + 2, startY - 1)};
    memcpy(_points, points, sizeof(points));
    _color = "\u001b[32;1m"; // green
    _type = s;
    break;
  }
  case 5: // T-Tetromino
  {
    Point points[4] = {Point(startX, startY), Point(startX + 1, startY), Point(startX + 2, startY), Point(startX + 1, startY - 1)};
    memcpy(_points, points, sizeof(points));
    _color = "\u001b[35;1m"; // magenta
    _type = t;
    break;
  }
  case 6: // Z-Tetromino
  {
    Point points[4] = {Point(startX + 1, startY), Point(startX + 2, startY), Point(startX, startY - 1), Point(startX + 1, startY - 1)};
    memcpy(_points, points, sizeof(points));
    _color = "\u001b[41m"; // red
    _type = z;
    break;
  }
  }
}

array<Point, 4> Tetris::getPoints()
{
  array<Point, 4> points;
  move(begin(_points), end(_points), points.begin());
  return points;
}

const string Tetris::getColor()
{
  return _color;
}

const Tetromino Tetris::getType()
{
  return _type;
}

void Tetris::moveX(unsigned int dx)
{
  for (int i = 0; i < 4; i++)
    _points[i].moveX(dx);
}

void Tetris::moveY(unsigned int dy)
{
  for (int i = 0; i < 4; i++)
    _points[i].moveY(dy);
}

void Tetris::rotate(unsigned int direction)
{
}

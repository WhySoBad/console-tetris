#include "Point.h"

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

int Point::getX() const { return x; }

int Point::getY() const { return y; }

void Point::setX(int x) { this->x = x; }

void Point::setY(int y) { this->y = y; }

void Point::moveX(int dx) { x += dx; }

void Point::moveY(int dy) { y += dy; }
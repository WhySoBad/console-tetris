#include "BoundingBox.h"

BoundingBox::BoundingBox(int x, int y, int height, int width): x(x), y(y), width(width), height(height) {}

int BoundingBox::getX() const { return x; }

int BoundingBox::getY() const { return y; }

int BoundingBox::getHeight() const { return height; }

int BoundingBox::getWidth() const { return width; }
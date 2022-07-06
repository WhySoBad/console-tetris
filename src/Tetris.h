#ifndef CONSOLE_TETRIS_TETRIS_H
#define CONSOLE_TETRIS_TETRIS_H

#include "array"
#include "string"
#include "geometry/BoundingBox.h"
#include "geometry/Point.h"

using namespace std;

enum Tetromino {
    i, j, l, o, s, t, z
};

class Tetris : public BoundingBox {
public:

    /**
     * Get the type of the tetris-object
     * @return type of the tetris-object
     */

    [[nodiscard]] Tetromino getType() const;

    /**
     * Get the points of the tetris-object
     * @return points of the tetris-object
     */

    [[nodiscard]] array<Point, 4> getPoints();

    /**
     * Move the tetris-object on the x-axis
     * @param dx amount of coordinates to move
     */

    void moveX(int dx);

    /**
     * Move the tetris-object on the y-axis
     * @param dy amount of coordinates to move
     */

    void moveY(int dy);

    /**
     * Rotate the tetris-object 90 degrees to the right
     */

    virtual void rotate() = 0;

    /**
     * Get the rotation of the tetris-object
     * @return rotation of the tetris-object
     */

    [[nodiscard]] int getRotation() const;

    Tetris(int startX, int startY, Tetromino type = static_cast<Tetromino>(0));

protected:
    Tetromino type;
    Point points[4];
    int step;
};

#endif
#ifndef CONSOLE_TETRIS_POINT_H
#define CONSOLE_TETRIS_POINT_H


class Point {
public:

    /**
     * Get the x-coordinate of the point
     * @return x-coordinate of the point
     */

    [[nodiscard]] int getX() const;

    /**
     * Get the y-coordinate of the point
     * @return y-coordinate of the point
     */

    [[nodiscard]] int getY() const;

    /**
     * Set the x-coordinate of the point
     * @param x x-coordinate of the point
     */

    void setX(int x);

    /**
     * Set the y-coordinate of the point
     * @param y y-coordinate of the point
     */

    void setY(int y);

    /**
     * Move the point on the x-axis
     * @param dx amount of coordinates to be moved
     */

    void moveX(int dx);

    /**
     * Move the point on the y-axis
     * @param dy amount of coordinates to be moved
     */

    void moveY(int dy);

    explicit Point(int x = 0, int y = 0);

private:
    int x;
    int y;
};


#endif //CONSOLE_TETRIS_POINT_H

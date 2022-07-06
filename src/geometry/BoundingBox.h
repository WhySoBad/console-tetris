#ifndef CONSOLE_TETRIS_BOUNDINGBOX_H
#define CONSOLE_TETRIS_BOUNDINGBOX_H


class BoundingBox {
public:

    /**
     * Get the x-coordinate of the bounding box
     * @return x-coordinate of the bounding box
     */

    [[nodiscard]] int getX() const;

    /**
     * Get the y-coordinate of the bounding box
     * @return y-coordinate of the bounding box
     */

    [[nodiscard]] int getY() const;

    /**
     * Get the width of the bounding box
     * @return width of the bounding box
     */

    [[nodiscard]] int getWidth() const;

    /**
     * Get the height of the bounding box
     * @return height of the bounding box
     */

    [[nodiscard]] int getHeight() const;

    BoundingBox(int x, int y, int height, int width);

protected:
    int x;
    int y;
    int width;
    int height;
};


#endif //CONSOLE_TETRIS_BOUNDINGBOX_H

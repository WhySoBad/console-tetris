#include <array>
#include <string>
using namespace std;

class Point {
  public:
   const int getX();    // x-coordinate of the point
   const int getY();    // y-coordinate of the point
   void setX(int x);    // set the x-coordinate of the point
   void setY(int y);    // set the y-coordinate of the point
   void moveX(int dx);  // move the x-coordinate of the point by dx
   void moveY(int dy);  // move the y-coordinate of the point by dy
   Point(int x, int y);

  private:
   int _x;  // modifiable x-coordinate of the point
   int _y;  // modifiable y-coordinate of the point
};

enum Tetromino { i, j, l, o, s, t, z };

class BoundingBox {
  public:
   int getX();       // get the highest x-coordinate of the bounding box
   int getY();       // get the highest y-coordinate of the bounding box
   int getWidth();   // get the width of the bounding box
   int getHeight();  // get the height of the bounding box
   BoundingBox(int x, int y, int height, int width);

  protected:
   int x;       // modifiable x-coordinate of the bounding box
   int y;       // modifiable y-coordinate of the bounding box
   int width;   // modifiable width of the bounding box
   int height;  // modifiable height of the bounding box
};

class Tetris : public BoundingBox {
  public:
   const Tetromino getType();    // get the type of the tetris-object
   array<Point, 4> getPoints();  // get the points of the tetris-object
   void moveX(int dx);           // move the tetris-object on the x-axis
   void moveY(int dy);           // move the tetris-object on the y-axis
   void rotate();                // rotate the tetris-object
   Tetris(int startX, int startY);

  private:
   Tetromino _type;   // modifiable type of the tetris-object
   Point _points[4];  // modifiable points of the tetris-object
   int step;          // modifiable rotation step of the tetris-object
};

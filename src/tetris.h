#include <string>
#include <array>
using namespace std;

class Point
{
public:
  const unsigned int getX();   // x-coordinate of the point
  const unsigned int getY();   // y-coordinate of the point
  void setX(unsigned int x);   // set the x-coordinate of the point
  void setY(unsigned int y);   // set the y-coordinate of the point
  void moveX(unsigned int dx); // move the x-coordinate of the point by dx
  void moveY(unsigned int dy); // move the y-coordinate of the point by dy
  Point(unsigned int x, unsigned int y);

private:
  unsigned int _x; // modifiable x-coordinate of the point
  unsigned int _y; // modifiable y-coordinate of the point
};

enum Tetromino
{
  i,
  j,
  l,
  o,
  s,
  t,
  z
};

class Tetris
{
public:
  const Tetromino getType();           // get the type of the tetris-object
  const string getColor();             // color of the tetris-object
  array<Point, 4> getPoints();         // get the points of the tetris-object
  void moveX(unsigned int dx);         // move the tetris-object on the x-axis
  void moveY(unsigned int dy);         // move the tetris-object on the y-axis
  void rotate(unsigned int direction); // rotate the tetris-object [positive = left, negative = right]
  Tetris(unsigned int startX, unsigned int startY);

private:
  Tetromino _type;  // modifiable type of the tetris-object
  string _color;    // modifiable color of the tetris-object
  Point _points[4]; // modifiable points of the tetris-object
};

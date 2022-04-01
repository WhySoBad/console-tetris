#include <ncurses.h>

#include <string>
#include <vector>

#include "tetris.h"
using namespace std;

enum Direction { vertical, horizontal };

class Game {
  public:
   Tetris getActive();                 // get the active tetris-object
   Tetris getUpcoming();               // get the upcoming tetris-object
   void start();                       // start the game cycle
   void stop();                        // stop the game cycle
   bool isRunning();                   // boolean whether the game cycle is currently running
   const int &height;                  // height of the game
   const int &width;                   // width of the game
   const int &framerate;               // framerate of the game
   void setFramerate(int &framerate);  // set the framerate of the game
   Game(int framerate);

  private:
   void draw();                                                          // draw method
   bool running = false;                                                 // boolean whether the game cycle is running or not
   int _height;                                                          // modifiable height of the game
   int _width;                                                           // modifiable width of the game
   int _framerate;                                                       // modifiable framerate of the game
   bool isInScreen(Point &point);                                        // method to get whether a point is in the screen
   bool isInField(Point &point);                                         // method to get whether a point is in the screen whereby the point is allowed to be out of the field at the top
   bool canRotate(Tetris tetris);                                        // method to check whether a tetris-object is allowed to rotate
   bool canMove(Tetris tetris, Direction direction, int delta);          // method to check whether a tetris-object is allowed to move in a given direction
   bool intersectsFixed(Tetris &tetris);                                 // method to check whether a tetris-object intersects with pixed points
   int getHighestFixed(int x);                                           // method to get the highest y-coordinate of a fixed point for a x-coordinate
   void drawPreviewLine(Tetris &tetris, const char *character);          // method to draw the line where the tetris-object will land
   void drawPoints(array<Point, 4>, const char *character, int c = -1);  // draw multiple points to the screen
   vector<vector<bool>> fixed;                                           // list with the "coordinated" of all fixed points
   WINDOW *window;                                                       // active window
   Tetris active;                                                        // active tetris-object
   Tetris upcoming;                                                      // next tetris-object
};
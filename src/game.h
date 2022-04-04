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
   int getHeight();                    // get the height of the screen
   int getWidth();                     // get the width of the screen
   int getFramerate();                 // get the framerate of the game
   void setFramerate(int &framerate);  // set the framerate of the game
   Game(int framerate);

  private:
   void draw();                                                          // draw method
   bool running = false;                                                 // boolean whether the game cycle is running or not
   int height;                                                           // modifiable height of the game
   int width;                                                            // modifiable width of the game
   int framerate;                                                        // modifiable framerate of the game
   bool isInScreen(Point &point);                                        // method to get whether a point is in the screen
   bool isInField(Point &point);                                         // method to get whether a point is in the screen whereby the point is allowed to be out of the field at the top
   bool canRotate(Tetris tetris);                                        // method to check whether a tetris-object is allowed to rotate
   bool canMove(Tetris tetris, Direction direction, int delta);          // method to check whether a tetris-object is allowed to move in a given direction
   bool intersectsFixed(Tetris &tetris);                                 // method to check whether a tetris-object intersects with pixed points
   int getHighestFixed(int x, int higher = 0);                           // method to get the highest y-coordinate of a fixed point for a x-coordinate
   void fixActive();                                                     // method to fix the active tetris-object
   void drawPreviewLine(Tetris &tetris, const char *character);          // method to draw the line where the tetris-object will land
   void drawPoints(array<Point, 4>, const char *character, int c = -1);  // draw multiple points to the screen
   void drawPoint(Point &point, const char *character, int c = -1);      // draw a single point on the screen
   void drawBorder();                                                    // draw the border around the field
   vector<vector<short>> fixed;                                          // list with the "coordinates" of all fixed points [0 = not fixed, all other numbers equal the fixed color]
   WINDOW *window;                                                       // active window
   Tetris active;                                                        // active tetris-object
   Tetris upcoming;                                                      // next tetris-object
   int paddingX;                                                         // padding to center the field on the x-axis
   int paddingY;                                                         // padding to center the field on the y-axis
   int level;                                                            // level of the current tetris round
   int score;                                                            // score of the current tetris round
   int cleared;                                                          // amount of cleared lines
};
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
   bool isInScreen(Point &point);                                                         // method to get whether a point is in the screen
   bool isInField(Point &point);                                                          // method to get whether a point is in the screen whereby the point is allowed to be out of the field at the top
   bool canRotate(Tetris tetris);                                                         // method to check whether a tetris-object is allowed to rotate
   bool canMove(Tetris tetris, Direction direction, int delta);                           // method to check whether a tetris-object is allowed to move in a given direction
   bool intersectsFixed(Tetris &tetris);                                                  // method to check whether a tetris-object intersects with pixed points
   int getHighestFixed(int x, int higher = 0);                                            // method to get the highest y-coordinate of a fixed point for a x-coordinate
   void fixActive();                                                                      // method to fix the active tetris-object
   void drawPreviewLine(Tetris &tetris, const char *character);                           // method to draw the line where the tetris-object will land
   void drawPoints(array<Point, 4>, const char *character, int c = -1);                   // draw multiple points to the screen
   void drawPoint(Point &point, const char *character, int c = -1, bool ignore = false);  // draw a single point on the screen
   void drawBorder();                                                                     // draw the border around the field
   void drawUpcoming();                                                                   // draw the upcoming object in its box
   void drawStatistics();                                                                 // draw the statistics below the upcoming object
   void drawPaused(bool clear = false);                                                   // draw the paused icon
   void gameOver();                                                                       // handle a game over
   void draw();                                                                           // draw method
   int getDroppingFrames();                                                               // get how many frames have to pass until the tile drops one row
   vector<vector<short>> fixed;                                                           // list with the "coordinates" of all fixed points [0 = not fixed, all other numbers equal the fixed color]
   array<int, 7> block;                                                                   // block with the upcoming 7 object types
   WINDOW *window;                                                                        // active window
   Tetris active;                                                                         // active tetris-object
   Tetris upcoming;                                                                       // next tetris-object
   int paddingX;                                                                          // padding to center the field on the x-axis
   int paddingY;                                                                          // padding to center the field on the y-axis
   int level = 0;                                                                         // level of the current tetris round
   int score = 0;                                                                         // score of the current tetris round
   int cleared = 0;                                                                       // amount of cleared lines
   int height;                                                                            // modifiable height of the game
   int width;                                                                             // modifiable width of the game
   int framerate;                                                                         // modifiable framerate of the game
   int currentBlock = 2;                                                                  // the current block
   bool running = false;                                                                  // boolean whether the game cycle is running or not
   bool paused = false;                                                                   // boolean whether the game is currently paused
   bool inMenu = false;                                                                   // boolean whether the game is currently in menu
};
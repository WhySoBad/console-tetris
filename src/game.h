#ifndef CONSOLE_TETRIS_GAME_H
#define CONSOLE_TETRIS_GAME_H
#include <ncurses.h>

#include <string>
#include <vector>

#include "tetris.h"
using namespace std;

#define TOP_LEFT_CORNER         "┏"
#define TOP_RIGHT_CORNER        "┓"
#define BOTTOM_LEFT_CORNER      "┗"
#define BOTTOM_RIGHT_CORNER     "┛"
#define VERTICAL_BORDER         "┃"
#define HORIZONTAL_BORDER       "━"
#define TOP_CROSSING_BORDER     "┳"
#define BOTTOM_CROSSING_BORDER  "┻"
#define RIGHT_CROSSING_BORDER   "┫"
#define LEFT_CROSSING_BORDER    "┣"
#define EMPTY_SPACE             " "
#define OCCUPIED_SPACE          " "
#define PREVIEW_DOT             "•"

#define FIELD_WIDTH     10 // width of the game field
#define MENU_WIDTH      7 // width of the right sided menu
#define UPCOMING_HEIGHT 6 // height of the upcoming box
#define FIELD_HEIGHT    24 // height of the game field
#define STEPS           30 // should optimally be 0.5x the framerate
#define START_Y         -2
#define POINT_SIZE      1 // ui size [1 = small; 2 = big]
#define LETTER_SIZE     1
#define POINT_WIDTH     2 // point width [2 for smoother bricks]
#define LETTER_HEIGHT   5
#define LETTER_WIDTH    3

enum Direction { vertical, horizontal };

class Game {
  public:
   void start();                       // start the game cycle
   void stop();                        // stop the game cycle
   Game(int framerate);

  private:
   void fixActive();                                                                                           // method to fix the active tetris-object
   void drawPaused(bool clear = false);                                                                        // draw the paused icon
   void gameOver();                                                                                            // handle a game over
   void draw();                                                                                                // draw method
   [[nodiscard]] int getDroppingFrames() const;                                                                                    // get how many frames have to pass until the tile drops one row
   array<int, 7> block;                                                                                        // block with the upcoming 7 object types
   Tetris active;                                                                                              // active tetris-object
   Tetris upcoming;                                                                                            // next tetris-object
   int level = 0;                                                                                              // level of the current tetris round
   int score = 0;                                                                                              // score of the current tetris round
   int cleared = 0;                                                                                            // amount of cleared lines
   int framerate;                                                                                              // modifiable framerate of the game
   int currentBlock = 2;                                                                                       // the current block
   bool running = false;                                                                                       // boolean whether the game cycle is running or not
   bool paused = false;                                                                                        // boolean whether the game is currently paused
   bool inMenu = false;                                                                                        // boolean whether the game is currently in menu
};
#endif
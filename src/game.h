#include <string>
#include <vector>
#include "tetris.h"
using namespace std;

class Game
{
public:
  Tetris getActive();                // get the active tetris-object
  Tetris getUpcoming();              // get the upcoming tetris-object
  void start();                      // start the game cycle
  void stop();                       // stop the game cycle
  bool isRunning();                  // boolean whether the game cycle is currently running
  const int &height;                 // height of the game
  const int &width;                  // width of the game
  const int &framerate;              // framerate of the game
  void setFramerate(int &framerate); // set the framerate of the game
  Game(int framerate);

private:
  void draw();                    // draw method
  bool running = false;           // boolean whether the game cycle is running or not
  void onKeypress(int &key);      // method called when a key is pressed
  int _height;                    // modifiable height of the game
  int _width;                     // modifiable width of the game
  int _framerate;                 // modifiable framerate of the game
  vector<vector<string>> points;  // list with all points to draw
  Tetris active = Tetris(0, 0);   // active tetris-object
  Tetris upcoming = Tetris(0, 0); // next tetris-object
};
#include "game.h"
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <sys/ioctl.h>
#include <vector>
#include <unistd.h>
#include <ctime>
using namespace std;

#define CONSOLE_CLEAR "\x1B[2J\x1B[H";

const string TOP_LEFT_CORNER = "┏";
const string TOP_RIGHT_CORNER = "┓";
const string BOTTOM_LEFT_CORNER = "┗";
const string BOTTOM_RIGHT_CORNER = "┛";
const string VERTICAL_BORDER = "┃";
const string HORIZONTAL_BORDER = "━";

int step = 0; // step indicating the current cycle tick; the object should only move every fourth tick

auto last = chrono::system_clock::now();

Game::Game(int framerate = 60) : height(_height), width(_width), framerate(_framerate)
{
  _framerate = framerate;
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); // get terminal size
  _height = size.ws_row - 1;               // keyboard line subtracted
  _width = size.ws_col;

  active = Tetris(rand() % width, 2);
  upcoming = Tetris(rand() % width, 2);

  for (int i = 0; i < height; i++)
  {
    vector<string> row = vector<string>(width);
    for (int j = 0; j < width; j++)
      row[j] = " ";
    points.push_back(row);
  }
}

void Game::draw()
{
  auto now = chrono::system_clock::now();
  chrono::duration<double> duration = now - last;
  last = now;

  // clear console
  cout << CONSOLE_CLEAR;

  // clear current points

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      points[i][j] = " ";

  // get current key

  int key = fgetc(stdin);
  if (key == 27 && fgetc(stdin) == 91)
  {
    key = fgetc(stdin); // arrow key number
    // cout << key << "\n";
  }

  // draw active tetris

  for (Point point : active.getPoints())
  {
    if (point.getX() >= 1 && point.getX() <= width - 2 && point.getY() >= 1 && point.getY() <= height - 2)
    {
      points[point.getY()][point.getX()] = active.getColor() + "#" + "\u001b[0m";
    }
  }
  if (step == 4)
    active.moveY(1);

  switch (key)
  {
  case 65: // UP
    // active.moveY(-1); // user shouldn't be able to move up [TODO -> Add object rotation here]
    break;
  case 66: // DOWN
    active.moveY(1);
    break;
  case 67: // RIGHT
    active.moveX(1);
    break;
  case 68: // LEFT
    active.moveX(-1);
    break;
  }

  for (Point point : upcoming.getPoints())
  {
    if (point.getX() >= 1 && point.getX() <= width - 2 && point.getY() >= 1 && point.getY() <= height - 2)
    {
      points[point.getY()][point.getX()] = upcoming.getColor() + "#" + "\u001b[0m";
    }
  }
  if (step == 4)
    upcoming.moveY(1);

  // add border
  for (int i = 0; i < height; i++)
  {
    points[i][0] = VERTICAL_BORDER;
    points[i][width - 1] = VERTICAL_BORDER;

    if (i == 0 || i == height - 1)
    {
      for (int j = 0; j < width; j++)
      {
        if (j == 0 && i == 0)
          points[i][j] = TOP_LEFT_CORNER;
        else if (j == width - 1 && i == 0)
          points[i][j] = TOP_RIGHT_CORNER;
        else if (j == 0)
          points[i][j] = BOTTOM_LEFT_CORNER;
        else if (j == width - 1)
          points[i][j] = BOTTOM_RIGHT_CORNER;
        else
          points[i][j] = HORIZONTAL_BORDER;
      }
    }

    for (int j = 0; j < width; j++)
    {
      if (running)
        cout << points[i][j];
    }
    if (running)
      cout << "\n";
    else
      cout << CONSOLE_CLEAR;
  }
  if (step == 4)
    step = 0;
  else
    step++;
}

Tetris Game::getActive()
{
  return active;
}

Tetris Game::getUpcoming()
{
  return upcoming;
}

void Game::start()
{
  running = true;
  const int tick = 1000 / framerate; // calculate time for one tick [TODO -> use float instead of rounded int]
  thread([this, tick]()
         {
    while(running) {
      const auto next = chrono::steady_clock::now() + chrono::milliseconds(tick);
     if(isRunning()) draw(); // execute current draw cycle
      this_thread::sleep_until(next); // pause thread until next draw cycle
    } })
      .detach();
}

void Game::stop()
{
  running = false;       // stop the game cycle
  cout << CONSOLE_CLEAR; // clear the console
}

bool Game::isRunning()
{
  return running;
}

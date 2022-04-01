#include "game.h"

#include <locale.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

const char *TOP_LEFT_CORNER = "┏";
const char *TOP_RIGHT_CORNER = "┓";
const char *BOTTOM_LEFT_CORNER = "┗";
const char *BOTTOM_RIGHT_CORNER = "┛";
const char *VERTICAL_BORDER = "┃";
const char *HORIZONTAL_BORDER = "━";
const char *EMPTY_SPACE = " ";
const char *OCCUPIED_SPACE = " ";
const char *PREVIEW_DOT = "•";
const int steps = 10;  // should optionally be 0.5x the framerate
const int startY = -2;

int step = steps;  // step indicating the current cycle tick; the object should only move every fourth tick

Game::Game(int framerate = 60) : height(_height), width(_width), framerate(_framerate), active(Tetris(2, 2)), upcoming(Tetris(2, 2)) {
   auto seed = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now()).time_since_epoch().count();
   srand(seed);  // add seed to rand();
   setlocale(LC_ALL, "");
   _framerate = framerate;
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);  // get terminal size
   _height = size.ws_row;                    // keyboard line subtracted
   _width = size.ws_col;

   WINDOW *win = newwin(height, width, 0, 0);  // create the new window
   initscr();                                  // init ncurses screen
   cbreak();                                   // enable cbreak mode to get inputs without pressing Enter and interruptions signals
   noecho();                                   // disable character echoing
   keypad(stdscr, TRUE);                       // enable arrow-key inputs
   use_default_colors();                       // enable default terminal colors in order to get transparent background
   start_color();                              // enable color
   curs_set(0);                                // disable cursor
   mousemask(ALL_MOUSE_EVENTS, NULL);

   active = Tetris((rand() % (width - 2)) + 1, startY);
   upcoming = Tetris((rand() % (width - 2)) + 1, startY);

   init_color(9, 1000, 533, 0);    // orange color
   init_color(10, 309, 309, 309);  // light gray color

   init_pair(1, -1, COLOR_CYAN);     // I-Tetromino
   init_pair(2, -1, COLOR_BLUE);     // J-Tetromino
   init_pair(3, -1, 9);              // L-Tetromino
   init_pair(4, -1, COLOR_YELLOW);   // O-Tetromino
   init_pair(5, -1, COLOR_GREEN);    // S-Tetromino
   init_pair(6, -1, COLOR_MAGENTA);  // T-Tetromino
   init_pair(7, -1, COLOR_RED);      // Z-Tetromino
   init_pair(8, 10, -1);             // preview dot

   window = win;

   for (int i = 0; i < height; i++) {
      vector<bool> row = vector<bool>(width);
      for (int j = 0; j < width; j++) {
         row[j] = false;
         if (i == 0 && j == 0)
            mvprintw(i, j, TOP_LEFT_CORNER);
         else if (i == 0 && j == width - 1)
            mvprintw(i, j, TOP_RIGHT_CORNER);
         else if (i == height - 1 && j == 0)
            mvprintw(i, j, BOTTOM_LEFT_CORNER);
         else if (i == height - 1 && j == width - 1)
            mvprintw(i, j, BOTTOM_RIGHT_CORNER);
         else if (i == 0 || i == height - 1)
            mvprintw(i, j, HORIZONTAL_BORDER);
         else if (j == 0 || j == width - 1)
            mvprintw(i, j, VERTICAL_BORDER);
      }
      fixed.push_back(row);
   }

   refresh();
}

void Game::draw() {
   // draw active object

   if (step == steps) {
      if (canMove(active, Direction::vertical, 1)) {
         drawPoints(active.getPoints(), EMPTY_SPACE);
         active.moveY(1);
         drawPreviewLine(active, PREVIEW_DOT);
         drawPoints(active.getPoints(), OCCUPIED_SPACE, active.getType() + 1);
      } else {
         drawPreviewLine(active, EMPTY_SPACE);
         for (int i = 0; i < active.getPoints().size(); i++) {
            Point point = active.getPoints()[i];
            fixed[point.getY()][point.getX()] = true;
         }
         active = upcoming;
         upcoming = Tetris((rand() % (width - 2)) + 1, startY);
      }
   }

   // get current key

   timeout(0);  // don't wait until user input
   int key = getch();
   flushinp();  // clear input buffer

   // TODO -> Move left/right on left/rightclick

   switch (key) {
      case KEY_UP:
         if (canRotate(active)) {
            drawPoints(active.getPoints(), EMPTY_SPACE);
            drawPreviewLine(active, EMPTY_SPACE);
            active.rotate();
            drawPreviewLine(active, PREVIEW_DOT);
            drawPoints(active.getPoints(), OCCUPIED_SPACE, active.getType() + 1);
         }
         break;
      case KEY_DOWN:
         if (canMove(active, Direction::vertical, 1)) {
            drawPoints(active.getPoints(), EMPTY_SPACE);
            active.moveY(1);
            drawPreviewLine(active, PREVIEW_DOT);
            drawPoints(active.getPoints(), OCCUPIED_SPACE, active.getType() + 1);
         }
         break;
      case KEY_RIGHT:
         if (canMove(active, Direction::horizontal, 1)) {
            drawPoints(active.getPoints(), EMPTY_SPACE);
            drawPreviewLine(active, EMPTY_SPACE);
            active.moveX(1);
            drawPreviewLine(active, PREVIEW_DOT);
            drawPoints(active.getPoints(), OCCUPIED_SPACE, active.getType() + 1);
         }
         break;
      case KEY_LEFT:
         if (canMove(active, Direction::horizontal, -1)) {
            drawPoints(active.getPoints(), EMPTY_SPACE);
            drawPreviewLine(active, EMPTY_SPACE);
            active.moveX(-1);
            drawPreviewLine(active, PREVIEW_DOT);
            drawPoints(active.getPoints(), OCCUPIED_SPACE, active.getType() + 1);
         }
         break;
   };

   if (step % 2 == 0) refresh();
   if (step == steps)
      step = 0;
   else
      step += 1;
}

Tetris Game::getActive() { return active; }

Tetris Game::getUpcoming() { return upcoming; }

void Game::start() {
   step = 0;
   running = true;
   const float tick = 1000 / framerate;
   do {
      chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
      draw();
      chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
      float elapsed = chrono::duration_cast<chrono::milliseconds>(after - now).count();
      napms(tick - elapsed);  // await remaining milliseconds of the tick
   } while (running);
}

void Game::stop() {
   running = false;  // stop the game cycle
   endwin();         // stop the ncurses mode and reenter the default terminal
}

void Game::drawPoints(array<Point, 4> points, const char *character, int color) {
   for (int i = 0; i < points.size(); i++) {
      Point point = points[i];
      if (isInScreen(point)) {
         if (color >= 0) attron(COLOR_PAIR(color));
         mvprintw(point.getY(), point.getX(), character);
         if (color >= 0) attroff(COLOR_PAIR(color));
      }
   }
}

bool Game::isRunning() { return running; }

bool Game::canRotate(Tetris tetris) {
   tetris.rotate();
   for (int i = 0; i < tetris.getPoints().size(); i++) {
      if (!isInField(tetris.getPoints()[i])) return false;
   }
   return !intersectsFixed(tetris);
}

bool Game::canMove(Tetris tetris, Direction direction, int delta) {
   if (direction == Direction::vertical) {
      tetris.moveY(delta);
   } else {
      tetris.moveX(delta);
   }
   for (int i = 0; i < tetris.getPoints().size(); i++) {
      if (!isInField(tetris.getPoints()[i])) return false;
   }
   return !intersectsFixed(tetris);
}

bool Game::intersectsFixed(Tetris &tetris) {
   for (int i = 0; i < tetris.getPoints().size(); i++) {
      Point point = tetris.getPoints()[i];
      if (isInScreen(point)) {
         if (fixed[point.getY()][point.getX()]) return true;
      }
   }
   return false;
}

void Game::drawPreviewLine(Tetris &tetris, const char *character) {
   for (int i = 0; i < tetris.getPoints().size(); i++) {
      Point point = tetris.getPoints()[i];
      if (point.getY() != tetris.getY() + tetris.getHeight() - 1) continue;
      for (int j = 0; j < getHighestFixed(point.getX()) - point.getY() - 1; j++) {
         Point dot = Point(point.getX(), point.getY() + 1 + j);
         if (isInScreen(dot) && !fixed[dot.getY()][dot.getX()]) {
            attron(COLOR_PAIR(8));
            mvprintw(dot.getY(), dot.getX(), character);
            attroff(COLOR_PAIR(8));
         }
      }
   }
}

int Game::getHighestFixed(int x) {
   for (int i = 0; i < height; i++) {
      if (fixed[i][x]) return i;
   }
   return height - 1;
}

bool Game::isInScreen(Point &point) { return point.getX() > 0 && point.getX() < width - 1 && point.getY() > 0 && point.getY() < height - 1; }

bool Game::isInField(Point &point) { return point.getX() > 0 && point.getX() < width - 1 && point.getY() < height - 1; }
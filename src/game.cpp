#include "game.h"

#include <locale.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const char *TOP_LEFT_CORNER = "┏";
const char *TOP_RIGHT_CORNER = "┓";
const char *BOTTOM_LEFT_CORNER = "┗";
const char *BOTTOM_RIGHT_CORNER = "┛";
const char *VERTICAL_BORDER = "┃";
const char *HORIZONTAL_BORDER = "━";
const char *TOP_CROSSING_BORDER = "┳";
const char *BOTTOM_CROSSING_BORDER = "┷";
const char *EMPTY_SPACE = " ";
const char *OCCUPIED_SPACE = " ";
const char *PREVIEW_DOT = "•";
const int fieldWidth = 10;
const int menuWidth = 7;
const int fieldHeight = 24;
const int steps = 10;  // should optionally be 0.5x the framerate
const int startY = -2;
const int pointSize = 2;
const int pointWidth = 2;

int step = steps;  // step indicating the current cycle tick; the object should only move every fourth tick

Game::Game(int framerate = 60) : active(Tetris(0, 0)), upcoming(Tetris(0, 0)) {
   auto seed = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now()).time_since_epoch().count();
   srand(seed);  // add seed to rand();
   setlocale(LC_ALL, "");
   Game::framerate = framerate;
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);  // get terminal size
   height = size.ws_row;                     // keyboard line subtracted
   width = size.ws_col;

   WINDOW *win = newwin(height, width, 0, 0);  // create the new window
   initscr();                                  // init ncurses screen
   cbreak();                                   // enable cbreak mode to get inputs without pressing Enter and interruptions signals
   noecho();                                   // disable character echoing
   keypad(stdscr, TRUE);                       // enable arrow-key inputs
   use_default_colors();                       // enable default terminal colors in order to get transparent background
   start_color();                              // enable color
   curs_set(0);                                // disable cursor
   mousemask(ALL_MOUSE_EVENTS, NULL);

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

   drawBorder();

   // TODO -> Check if tetris-object fits into the width

   active = Tetris(rand() % (fieldWidth - 1), startY);
   upcoming = Tetris(rand() % (fieldWidth - 1), startY);

   for (int i = 0; i < fieldHeight; i++) {
      vector<bool> row = vector<bool>(fieldWidth);
      for (int j = 0; j < fieldWidth; j++) {
         row[j] = false;
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
      }
   }

   // get current key
   timeout(0);  // don't wait until user input
   int key = getch();
   flushinp();  // clear input buffer

   mvprintw(2, 5, to_string(active.getX()).c_str());
   mvprintw(3, 5, to_string(active.getY()).c_str());
   mvprintw(4, 5, to_string(active.getWidth()).c_str());
   mvprintw(5, 5, to_string(active.getHeight()).c_str());

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
         if (canMove(active, Direction::vertical, fieldHeight - active.getY() - active.getHeight())) {
            drawPoints(active.getPoints(), EMPTY_SPACE);
            drawPreviewLine(active, EMPTY_SPACE);
            active.moveY(fieldHeight - active.getY() - active.getHeight());
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

   if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_LEFT || step == steps) {
      if (!canMove(active, Direction::vertical, 1)) {
         mvprintw(6, 5, "true ");
         drawPreviewLine(active, EMPTY_SPACE);
         for (int i = 0; i < active.getPoints().size(); i++) {
            Point point = active.getPoints()[i];
            fixed[point.getY()][point.getX()] = true;
         }
         active = upcoming;
         upcoming = Tetris(rand() % (fieldWidth - 1), startY);
         int rotation = (rand() % 3) + 1;
         while (canRotate(upcoming) && upcoming.getRotation() < rotation) upcoming.rotate();  // add random rotation to the tetris-object
      } else
         mvprintw(6, 5, "false");
   }

   if (step % 2 == 0) refresh();
   if (step == steps)
      step = 0;
   else
      step += 1;
}

Tetris Game::getActive() { return active; }

Tetris Game::getUpcoming() { return upcoming; }

int Game::getFramerate() { return framerate; }

int Game::getHeight() { return height; }

int Game::getWidth() { return width; }

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
         for (int i = 0; i < pow(pointSize, 2) * pointWidth; i++) {
            int row = i / pointWidth / pointSize;
            mvprintw(pointSize * point.getY() + paddingY + row, pointWidth * pointSize * point.getX() + paddingX + i - (row * pointWidth * pointSize), character);
         }
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
      if (point.getY() != tetris.getY() + tetris.getHeight() - 1) continue;  // filter out lowest points of the tetris-object
      for (int j = 0; j < getHighestFixed(point.getX()) - point.getY(); j++) {
         Point dot = Point(point.getX(), point.getY() + 1 + j);
         if (isInScreen(dot) && !fixed[dot.getY()][dot.getX()]) {
            attron(COLOR_PAIR(8));
            for (int i = 0; i < pow(pointSize, 2) * pointWidth; i++) {
               int row = i / pointWidth / pointSize;
               mvprintw(pointSize * dot.getY() + paddingY + row, pointWidth * pointSize * dot.getX() + paddingX + i - (row * pointWidth * pointSize), character);
            }
            attroff(COLOR_PAIR(8));
         }
      }
   }
}

int Game::getHighestFixed(int x) {
   for (int i = 0; i < fixed.size(); i++) {
      if (fixed[i][x]) return i;
   }
   return fixed.size() - 1;
}

bool Game::isInScreen(Point &point) { return point.getX() >= 0 && point.getX() < fieldWidth && point.getY() >= 0 && point.getY() < fieldHeight; }

bool Game::isInField(Point &point) { return point.getX() >= 0 && point.getX() < fieldWidth && point.getY() < fieldHeight; }

void Game::drawBorder() {
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
   height = size.ws_row;
   width = size.ws_col;
   if (width > ((fieldWidth + menuWidth) * pointSize * pointWidth + 3)) {
      paddingX = (int)((width - (fieldHeight - menuWidth) * pointSize * pointWidth - 3) / 2);
   }
   if (height > (fieldHeight * pointSize + 2)) {
      paddingY = (int)((height - fieldHeight * pointSize) / 2);
   }

   // draw field

   for (int i = 0; i < fieldHeight * pointSize + 2; i++) {
      for (int j = 0; j < fieldWidth * pointSize * pointWidth + 2; j++) {
         if (i == 0 && j == 0)
            mvprintw(paddingY + i - 1, paddingX + j - 1, TOP_LEFT_CORNER);
         else if (i == 0 && j == fieldWidth * pointSize * pointWidth + 1)
            mvprintw(paddingY + i - 1, paddingX + j - 1, TOP_CROSSING_BORDER);
         else if (i == fieldHeight * pointSize + 1 && j == 0)
            mvprintw(paddingY + i - 1, paddingX + j - 1, BOTTOM_LEFT_CORNER);
         else if (i == fieldHeight * pointSize + 1 && j == fieldWidth * pointSize * pointWidth + 1)
            mvprintw(paddingY + i - 1, paddingX + j - 1, BOTTOM_CROSSING_BORDER);
         else if (i == 0 || i == fieldHeight * pointSize + 1)
            mvprintw(paddingY + i - 1, paddingX + j - 1, HORIZONTAL_BORDER);
         else if (j == 0 || j == fieldWidth * pointSize * pointWidth + 1)
            mvprintw(paddingY + i - 1, paddingX + j - 1, VERTICAL_BORDER);
      }
   }

   // draw menu

   for (int i = 0; i < fieldHeight * pointSize + 2; i++) {
      for (int j = 0; j < menuWidth * pointSize * pointWidth + 1; j++) {
         if (i == 0 && j == menuWidth * pointSize * pointWidth)
            mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, TOP_RIGHT_CORNER);
         else if (i == fieldHeight * pointSize + 1 && j == menuWidth * pointSize * pointWidth)
            mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, BOTTOM_RIGHT_CORNER);
         else if (i == 0 || i == fieldHeight * pointSize + 1)
            mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, HORIZONTAL_BORDER);
         else if (j == menuWidth * pointSize * pointWidth)
            mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, VERTICAL_BORDER);
      }
   }
}

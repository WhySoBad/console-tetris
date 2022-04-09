#include "game.h"

#include <locale.h>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <set>
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
const char *BOTTOM_CROSSING_BORDER = "┻";
const char *RIGHT_CROSSING_BORDER = "┫";
const char *LEFT_CROSSING_BORDER = "┣";
const char *EMPTY_SPACE = " ";
const char *OCCUPIED_SPACE = " ";
const char *PREVIEW_DOT = "•";
const int fieldWidth = 10;
const int menuWidth = 7;
const int upcomingHeight = 6;  // height of the upcoming box
const int fieldHeight = 24;
const int steps = 30;  // should optionally be 0.5x the framerate
const int startY = -2;
const int pointSize = 2;
const int pointWidth = 2;

bool gameover = false;
int step = 53;  // step indicating the current cycle tick; the object should only move every fourth tick

Game::Game(int framerate = 60) : active(Tetris(0, 0, (Tetromino)0)), upcoming(Tetris(0, 0, (Tetromino)0)) {
   auto seed = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now()).time_since_epoch().count();
   srand(seed);  // add seed to rand();
   setlocale(LC_ALL, "");
   Game::framerate = framerate;
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);  // get terminal size
   height = size.ws_row;
   width = size.ws_col;
   for (int i = 0; i < block.size(); i++) block[i] = i;
   random_shuffle(begin(block), end(block));  // randomize the entries

   WINDOW *win = newwin(height, width, 0, 0);  // create the new window
   initscr();                                  // init ncurses screen
   cbreak();                                   // enable cbreak mode to get inputs without pressing Enter and interruptions signals
   noecho();                                   // disable character echoing
   keypad(stdscr, TRUE);                       // enable arrow-key inputs
   use_default_colors();                       // enable default terminal colors in order to get transparent background
   start_color();                              // enable color
   curs_set(0);                                // disable cursor
   nodelay(win, TRUE);                         // disable key wait delay
   // mousemask(ALL_MOUSE_EVENTS, NULL);          // override all mouse events

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
   init_pair(9, -1, COLOR_WHITE);    // menu stuff

   window = win;

   drawBorder();

   active = Tetris(rand() % (fieldWidth - 1), startY, (Tetromino)block[0]);
   if (active.getX() + active.getWidth() >= fieldWidth) active.moveX(fieldWidth - (active.getX() + active.getWidth()));
   upcoming = Tetris(rand() % (fieldWidth - 1), startY, (Tetromino)block[1]);
   if (upcoming.getX() + upcoming.getWidth() >= fieldWidth) upcoming.moveX(fieldWidth - (upcoming.getX() + upcoming.getWidth()));

   for (int i = 0; i < fieldHeight; i++) {
      vector<short> row = vector<short>(fieldWidth);
      for (int j = 0; j < fieldWidth; j++) {
         row[j] = 0;  // not occupied
      }
      fixed.push_back(row);
   }

   drawUpcoming();
   refresh();
}

void Game::draw() {
   // get current key
   timeout(0);  // don't wait for user input
   int key = getch();
   flushinp();  // clear input buffer

   if (key == 27 && !paused) {  // catch ESC key
      paused = true;
      key = -1;  // make sur the key information isn't used twice
   }

   // TODO -> "Custom" font adjustable to the point size
   // TODO -> Save file
   // TODO -> Auto adjust to screen size -> Error when terminal is too small

   if (!gameover && !paused && !inMenu) {
      // draw active object
      if (step >= getDroppingFrames()) {
         if (canMove(active, Direction::vertical, 1)) {
            drawPoints(active.getPoints(), EMPTY_SPACE);
            active.moveY(1);
            drawPreviewLine(active, PREVIEW_DOT);
            drawPoints(active.getPoints(), OCCUPIED_SPACE, active.getType() + 1);
         }
      }

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
         case KEY_DOWN: {
            int shortest = fieldHeight - active.getY() - active.getHeight();  // default shortest distance
            for (int i = 0; i < active.getPoints().size(); i++) {
               Point point = active.getPoints()[i];
               int highestFixed = getHighestFixed(point.getX(), active.getHeight() + active.getY());
               int _shortest = highestFixed - point.getY() - 1;
               if (_shortest < shortest) shortest = _shortest;
            }
            if (canMove(active, Direction::vertical, shortest)) {
               drawPoints(active.getPoints(), EMPTY_SPACE);
               drawPreviewLine(active, EMPTY_SPACE);
               active.moveY(shortest);
               score += shortest;
               drawPreviewLine(active, PREVIEW_DOT);
               drawPoints(active.getPoints(), OCCUPIED_SPACE, active.getType() + 1);
            }
         } break;
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

      if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_LEFT || step == getDroppingFrames()) {
         if (!canMove(active, Direction::vertical, 1)) fixActive();
      }
   } else if (paused) {
      if (key == 27) {
         // restore game progress
         drawPaused(true);
         paused = false;  // unpause game
      } else {
         drawPaused();
      }
   }

   drawStatistics();
   refresh();
   if (step == getDroppingFrames())
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
      if (!gameover) draw();
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
      drawPoint(points[i], character, color);
   }
}

void Game::drawPoint(Point &point, const char *character, int color, bool ignore) {
   if (ignore || isInScreen(point)) {
      if (color >= 0) attron(COLOR_PAIR(color));
      for (int i = 0; i < pow(pointSize, 2) * pointWidth; i++) {
         int row = i / pointWidth / pointSize;
         mvprintw(pointSize * point.getY() + paddingY + row, pointWidth * pointSize * point.getX() + paddingX + i - (row * pointWidth * pointSize), character);
      }
      if (color >= 0) attroff(COLOR_PAIR(color));
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
      if (isInScreen(point)) {  // segmentation fault when object is out of field on the top
         if (getHighestFixed(point.getX()) == 0 && point.getY() < 0) return true;
         if (fixed[point.getY()][point.getX()]) return true;
      }
   }
   return false;
}

void Game::drawPreviewLine(Tetris &tetris, const char *character) {
   for (int i = 0; i < tetris.getPoints().size(); i++) {
      Point point = tetris.getPoints()[i];
      if (point.getY() != tetris.getY() + tetris.getHeight() - 1) continue;  // filter out lowest points of the tetris-object
      for (int j = 0; j < getHighestFixed(point.getX(), active.getHeight() + active.getY()) - point.getY(); j++) {
         Point dot = Point(point.getX(), point.getY() + 1 + j);
         if (isInScreen(dot) && !fixed[dot.getY()][dot.getX()]) {
            drawPoint(dot, character, 8);
         }
      }
   }
}

int Game::getHighestFixed(int x, int higher) {
   for (int i = 0; i < fixed.size(); i++) {
      if (fixed[i][x] && i >= higher) return i;
   }
   return fixed.size();
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

         // draw upcoming box

         if (i == (upcomingHeight + 1) * pointSize) {
            if (j == 0) {
               mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + j, LEFT_CROSSING_BORDER);
               mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, HORIZONTAL_BORDER);
            } else if (j == menuWidth * pointSize * pointWidth) {
               mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, RIGHT_CROSSING_BORDER);
            } else {
               mvprintw(paddingY + i - 1, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, HORIZONTAL_BORDER);
            }
         }
      }
   }
}

void Game::drawUpcoming() {
   // clear the current upcoming
   for (int i = 0; i < upcomingHeight * pointSize; i++) {
      for (int j = 0; j < menuWidth * pointSize * pointWidth; j++) {
         mvprintw(paddingY + i, paddingX + fieldWidth * pointSize * pointWidth + 1 + j, EMPTY_SPACE);
      }
   }

   // calculate the padding of the new upcoming
   int _paddingX = (menuWidth - upcoming.getHeight()) / 2;
   int _paddingY = (upcomingHeight - upcoming.getWidth()) / 2;

   // draw the new upcoming
   for (int i = 0; i < upcoming.getPoints().size(); i++) {
      Point point = upcoming.getPoints()[i];
      Point previewPoint = Point(fieldWidth + 1 + _paddingY + point.getX() - upcoming.getX(), _paddingX + point.getY() - upcoming.getY());
      drawPoint(previewPoint, OCCUPIED_SPACE, upcoming.getType() + 1, true);
   }
}

void Game::drawPaused(bool clear) {
   const int pausedWidth = 3;
   const int pausedHeight = 3;

   // calculate the padding of the paused icon
   int _paddingX = (fieldWidth - 3) / 2 + 1;
   int _paddingY = (fieldHeight - 3) / 2;

   for (int i = 0; i < fixed.size(); i++) {
      vector<short> row = fixed[i];
      for (int j = 0; j < row.size(); j++) {
         if (row[j]) {
            Point point = Point(j, i);
            drawPoint(point, clear ? OCCUPIED_SPACE : EMPTY_SPACE, clear ? row[j] : -1);
         }
      }
   }
   drawPoints(active.getPoints(), clear ? OCCUPIED_SPACE : EMPTY_SPACE, clear ? active.getType() + 1 : -1);
   if (!clear) drawPreviewLine(active, EMPTY_SPACE);

   for (int i = 0; i < pausedWidth * pausedHeight; i++) {
      int row = i / pausedWidth;
      int rest = i % pausedWidth;
      Point point = Point(_paddingX + rest, _paddingY + row);
      if (rest == 0 || rest == pausedWidth - 1) drawPoint(point, clear ? EMPTY_SPACE : OCCUPIED_SPACE, clear ? -1 : 9);
   }

   if (clear) drawPreviewLine(active, PREVIEW_DOT);
}

void Game::fixActive() {
   drawPreviewLine(active, EMPTY_SPACE);  // remove preview line
   bool outOfScreen = false;
   vector<int> modifiedRows = vector<int>();
   for (int i = 0; i < active.getPoints().size(); i++) {  // update fixed points in the array
      Point point = active.getPoints()[i];
      if (point.getY() >= 0) {
         fixed[point.getY()][point.getX()] = active.getType() + 1;  // store color for the point
      } else {
         outOfScreen = true;
      }
      bool existing = false;
      for (int j = 0; j < modifiedRows.size(); j++) {
         if (modifiedRows[j] == point.getY()) {
            existing = true;
            break;
         }
      }
      if (!existing) modifiedRows.push_back(point.getY());
   }

   if (outOfScreen || active.getY() <= 0) {
      gameOver();
      return;
   }

   // clear full rows
   vector<int> fullRows = vector<int>();

   for (int i = 0; i < modifiedRows.size(); i++) {
      vector<short> row = fixed[modifiedRows[i]];
      bool isFull = true;
      for (int j = 0; j < row.size(); j++) {
         if (!row[j]) {
            isFull = false;
            break;
         }
      }
      if (isFull) fullRows.push_back(modifiedRows[i]);
   }

   for (int i = 0; i < fullRows.size(); i++) {
      for (int j = 0; j < fixed[fullRows[i]].size(); j++) {
         fixed[fullRows[i]][j] = 0;  // not occupied
         Point point = Point(j, fullRows[i]);
         drawPoint(point, EMPTY_SPACE);
      }
   }

   // sort full rows in ascending order
   sort(fullRows.begin(), fullRows.end());

   for (int i = 0; i < fullRows.size(); i++) {
      int highestY = fieldHeight;
      for (int i = 0; i < fieldWidth; i++) {  // determine highest occupied row
         int highest = getHighestFixed(i);
         if (highest < highestY) highestY = highest;
      }

      if (highestY > fullRows[i]) continue;  // if no higher occupied row go to next one
      for (int j = 0; j <= fullRows[i] - highestY; j++) {
         int _y = fullRows[i] - j;
         if (j == fullRows[i] - highestY) {  // last occupied row should be cleared
            for (int k = 0; k < fieldWidth; k++) {
               fixed[_y][k] = 0;  // not occupied
               Point point = Point(k, _y);
               drawPoint(point, EMPTY_SPACE);
            }
         } else {  // every not-last occupied row gets moved one index lower
            fixed[_y] = fixed[_y - 1];
            for (int k = 0; k < fieldWidth; k++) {
               Point point = Point(k, _y);
               if (fixed[_y][k]) {
                  drawPoint(point, OCCUPIED_SPACE, fixed[_y][k]);
               } else {
                  drawPoint(point, EMPTY_SPACE);
               }
            }
         }
      }
   }

   // update level and score

   cleared += fullRows.size();
   if (level == 0 ? cleared >= 10 : cleared >= level * 30) level++;

   switch (fullRows.size()) {
      case 1:
         score += (level + 1) * 40;
         break;
      case 2:
         score += (level + 1) * 100;
         break;
      case 3:
         score += (level + 1) * 300;
         break;
      case 4:
         score += (level + 1) * 1200;
         break;
   }

   // update active object

   active = upcoming;
   if (currentBlock == block.size() - 1) {
      random_shuffle(begin(block), end(block));
      currentBlock = 0;
   }
   upcoming = Tetris(rand() % (fieldWidth - 1), startY, (Tetromino)block[currentBlock]);
   currentBlock++;
   if (upcoming.getX() + upcoming.getWidth() >= fieldWidth) upcoming.moveX(fieldWidth - (upcoming.getX() + upcoming.getWidth()));  // make sure the object is in the field
   for (int i = 0; i < (rand() % 4) + 1; i++) {                                                                                    // add random rotation
      if (canRotate(upcoming)) {
         upcoming.rotate();
      } else {
         break;
      }
   }
   drawUpcoming();
}

void Game::drawStatistics() {
   Point point = Point(fieldWidth * pointSize * pointWidth + 2, upcomingHeight * pointSize + 1 * pointSize);
   attron(A_BOLD);
   mvprintw(paddingY + point.getY(), paddingX + point.getX(), "Score");
   attroff(A_BOLD);

   point.moveY(1);
   mvprintw(paddingY + point.getY(), paddingX + point.getX(), to_string(score).c_str());

   point.moveY(2);
   attron(A_BOLD);
   mvprintw(paddingY + point.getY(), paddingX + point.getX(), "Level");
   attroff(A_BOLD);

   point.moveY(1);
   mvprintw(paddingY + point.getY(), paddingX + point.getX(), to_string(level).c_str());

   point.moveY(2);
   attron(A_BOLD);
   mvprintw(paddingY + point.getY(), paddingX + point.getX(), "Lines");
   attroff(A_BOLD);

   point.moveY(1);
   mvprintw(paddingY + point.getY(), paddingX + point.getX(), to_string(cleared).c_str());
}

int Game::getDroppingFrames() {
   switch (level) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
         return 53 - level * 4;
      case 6:
         return 28;
      case 7:
         return 22;
      case 8:
         return 17;
      case 9:
         return 11;
      case 10:
      case 11:
      case 12:
      case 13:
         return 10 + (10 - level);
      case 14:
      case 15:
         return 6;
      case 16:
      case 17:
         return 5;
      case 18:
      case 19:
         return 4;
      case 20:
         return 3;
      default:
         return 60;
   }
}

void Game::gameOver() {
   mvprintw(30, 5, "Game over");
   refresh();
   gameover = true;
}
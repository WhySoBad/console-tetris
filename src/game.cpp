#include "game.h"
#include "clocale"
#include "ncurses.h"
#include "sys/ioctl.h"
#include "unistd.h"
#include "algorithm"
#include "chrono"
#include "cmath"
#include "ctime"
#include "iostream"
#include "random"
#include "set"
#include "string"
#include "vector"
#include "helper/TetrisHelper.h"
#include "helper/DrawHelper.h"
#include "helper/ConfigHelper.h"

using namespace std;

bool gameover = false;
int step = 53;  // step indicating the current cycle tick; the object should only move every fourth tick

Game::Game(int framerate = 60) : active(Tetris(0, 0, (Tetromino)0)), upcoming(Tetris(0, 0, (Tetromino)0)) {
    setlocale(LC_ALL, "");
    DrawHelper::initialize();
    TetrisHelper::initialize();
    ConfigHelper::initialize();
    ConfigHelper::addRound();
    auto seed = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now()).time_since_epoch().count();
   srand(seed);  // add seed to rand();
   Game::framerate = framerate;
   for (int i = 0; i < block.size(); i++) block[i] = i;
   shuffle(begin(block), end(block), std::mt19937(std::random_device()()));  // randomize the entries

   DrawHelper::drawBorder();
   active = Tetris(rand() % (FIELD_WIDTH - 1), START_Y, (Tetromino)block[0]);
   if (active.getX() + active.getWidth() >= FIELD_WIDTH) active.moveX(FIELD_WIDTH - (active.getX() + active.getWidth()));
   upcoming = Tetris(rand() % (FIELD_WIDTH - 1), START_Y, (Tetromino)block[1]);
   if (upcoming.getX() + upcoming.getWidth() >= FIELD_WIDTH) upcoming.moveX(FIELD_WIDTH - (upcoming.getX() + upcoming.getWidth()));

   DrawHelper::drawUpcoming(&upcoming);
   refresh();
}

void Game::draw() {
   // get current key
   timeout(0);  // don't wait for user input
   int key = getch();
   flushinp();  // clear input buffer

   if (key == 27 && !paused) {  // catch ESC key
      paused = true;
      key = -1;  // make sure the key information isn't used twice
   }

   // TODO -> "Custom" font adjustable to the point size
   // TODO -> Save file
   // TODO -> Auto adjust to screen size -> Error when terminal is too small
   // TODO -> Index out of bounds when rotating directly at border

   if (!gameover && !paused && !inMenu) {
      // draw active object
      if (step >= getDroppingFrames()) {
         if (TetrisHelper::canTetrisMove(active, Direction::vertical, 1)) {
            DrawHelper::clearTetris(&active);
            active.moveY(1);
            DrawHelper::drawPreviewLine(&active);
            DrawHelper::drawTetris(&active);
         }
      }

      switch (key) {
         case KEY_UP:
            if (TetrisHelper::canTetrisRotate(active)) {
               DrawHelper::clearTetris(&active);
               DrawHelper::drawPreviewLine(&active, EMPTY_SPACE);
               active.rotate();
               DrawHelper::drawPreviewLine(&active);
               DrawHelper::drawTetris(&active);
            }
            break;
         case KEY_DOWN: {
            int shortest = FIELD_HEIGHT - active.getY() - active.getHeight();  // default shortest distance
            for (int i = 0; i < active.getPoints().size(); i++) {
               Point point = active.getPoints()[i];
               int highestFixed = TetrisHelper::getHighestFixed(point.getX(), active.getHeight() + active.getY());
               int _shortest = highestFixed - point.getY() - 1;
               if (_shortest < shortest) shortest = _shortest;
            }
            if (TetrisHelper::canTetrisMove(active, Direction::vertical, shortest)) {
               DrawHelper::clearTetris(&active);
               DrawHelper::drawPreviewLine(&active, EMPTY_SPACE);
               active.moveY(shortest);
               score += shortest;
               DrawHelper::drawPreviewLine(&active);
               DrawHelper::drawTetris(&active);
            }
         } break;
         case KEY_RIGHT:
            if (TetrisHelper::canTetrisMove(active, Direction::horizontal, 1)) {
               DrawHelper::clearTetris(&active);
               DrawHelper::drawPreviewLine(&active, EMPTY_SPACE);
               active.moveX(1);
               DrawHelper::drawPreviewLine(&active);
               DrawHelper::drawTetris(&active);
            }
            break;
         case KEY_LEFT:
            if (TetrisHelper::canTetrisMove(active, Direction::horizontal, -1)) {
               DrawHelper::clearTetris(&active);
               DrawHelper::drawPreviewLine(&active, EMPTY_SPACE);
               active.moveX(-1);
               DrawHelper::drawPreviewLine(&active);
               DrawHelper::drawTetris(&active);
            }
            break;
      };

      if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_LEFT || step == getDroppingFrames()) {
         if (!TetrisHelper::canTetrisMove(active, Direction::vertical, 1)) fixActive();
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
   DrawHelper::drawStatistics(score, level, cleared);
   // drawStatistics();
   refresh();
   if (step == getDroppingFrames())
      step = 0;
   else
      step += 1;
}

void Game::start() {
   step = 0;
   running = true;
   const float tick = 1000.0f / (float) framerate;
   do {
      chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
      if (!gameover) draw();
      chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
      float elapsed = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(after - now).count());
      napms(static_cast<int>(tick - elapsed));  // await remaining milliseconds of the tick
   } while (running);
}

void Game::stop() {
   running = false;  // stop the game cycle
   endwin();         // stop the ncurses mode and reenter the default terminal
}

void Game::drawPaused(bool clear) {
   const int pausedWidth = 3;
   const int pausedHeight = 3;

   // calculate the padding of the paused icon
   int _paddingX = (FIELD_WIDTH - 3) / 2 + 1;
   int _paddingY = (FIELD_HEIGHT - 3) / 2;

   for (int i = 0; i < FIELD_HEIGHT; i++) {
      array<short, FIELD_WIDTH> row = TetrisHelper::getFixedLine(i);
      for (int j = 0; j < row.size(); j++) {
         if (row[j]) {
            Point point = Point(j, i);
            DrawHelper::drawPoint(&point, clear ? OCCUPIED_SPACE : EMPTY_SPACE, clear ? row[j] : -1);
         }
      }
   }
   DrawHelper::drawTetris(&active, clear ? OCCUPIED_SPACE : EMPTY_SPACE, clear ? active.getType() + 1 : -1);
   if (!clear) DrawHelper::drawPreviewLine(&active, EMPTY_SPACE);

   for (int i = 0; i < pausedWidth * pausedHeight; i++) {
      int row = i / pausedWidth;
      int rest = i % pausedWidth;
      Point point = Point(_paddingX + rest, _paddingY + row);
      if (rest == 0 || rest == pausedWidth - 1) DrawHelper::drawPoint(&point, clear ? EMPTY_SPACE : OCCUPIED_SPACE, clear ? -1 : 9);
   }

   if (clear) DrawHelper::drawPreviewLine(&active);
}


void Game::fixActive() {
   DrawHelper::drawPreviewLine(&active, EMPTY_SPACE);  // remove preview line
   bool outOfScreen = false;

   vector<int> modifiedRows = vector<int>();
    for (Point point : active.getPoints()) { // update fixed points in the array
        if(point.getY() >= 0) TetrisHelper::setFixedPoint(point.getX(), point.getY(), active.getType() + 1); // store color for the point
        else outOfScreen = true;

        bool existing = false;
        for (int row: modifiedRows) {
            if(row == point.getY()) { existing = true; break; }
        }
        if(!existing) modifiedRows.push_back(point.getY());
    }

   if (outOfScreen || active.getY() <= 0) { gameOver(); return; }

   // clear full rows
   vector<int> fullRows = vector<int>();
    for (int row : modifiedRows) {
        array<short, FIELD_WIDTH> line = TetrisHelper::getFixedLine(row);
        bool isFull = true;
        for (short entry : line) {
            if(!entry) { isFull = false; break; }
        }
        if(isFull) fullRows.push_back(row);
    }

   for (int row : fullRows) {
      for (int j = 0; j < FIELD_WIDTH; j++) {
          TetrisHelper::setFixedPoint(j, row, 0); // not occupied
         auto * point = new Point(j, row);
         DrawHelper::drawPoint(point, EMPTY_SPACE);
      }
   }

   // sort full rows in ascending order
   sort(fullRows.begin(), fullRows.end());

   for (int row : fullRows) {
      int highestY = FIELD_HEIGHT;
      for (int i = 0; i < FIELD_WIDTH; i++) {  // determine highest occupied row
         int highest = TetrisHelper::getHighestFixed(i);
         if (highest < highestY) highestY = highest;
      }

      if (highestY > row) continue;  // if no higher occupied row go to next one
      for (int j = 0; j <= row - highestY; j++) {
         int _y = row - j;
         if (j == row - highestY) {  // last occupied row should be cleared
            for (int k = 0; k < FIELD_WIDTH; k++) {
                TetrisHelper::setFixedPoint(k, _y, 0); // not occupied
                auto * point = new Point(k, _y);
                DrawHelper::drawPoint(point, EMPTY_SPACE);
            }
         } else {  // every not-last occupied row gets moved one index lower
             TetrisHelper::setFixedLine(_y, TetrisHelper::getFixedLine(_y - 1));
            for (int k = 0; k < FIELD_WIDTH; k++) {
               auto * point = new Point(k, _y);
               if (TetrisHelper::getFixedPoint(k, _y)) {
                  DrawHelper::drawPoint(point, OCCUPIED_SPACE, TetrisHelper::getFixedPoint(k, _y));
               } else {
                  DrawHelper::drawPoint(point, EMPTY_SPACE);
               }
            }
         }
      }
   }

   // update level and score

   cleared += fullRows.size();
   if (level == 0 ? cleared >= 10 : cleared >= level * 30) level++;

   switch (fullRows.size()) {
      case 1: score += (level + 1) * 40; break;
      case 2: score += (level + 1) * 100; break;
      case 3: score += (level + 1) * 300; break;
      case 4: score += (level + 1) * 1200; break;
   }

   // update active object

   active = upcoming;
   if (currentBlock == block.size() - 1) {
      shuffle(begin(block), end(block), std::mt19937(std::random_device()()));
      currentBlock = 0;
   }
   upcoming = Tetris(rand() % (FIELD_WIDTH - 1), START_Y, (Tetromino)block[currentBlock]);
   currentBlock++;
   if (upcoming.getX() + upcoming.getWidth() >= FIELD_WIDTH) upcoming.moveX(FIELD_WIDTH - (upcoming.getX() + upcoming.getWidth()));  // make sure the object is in the field
   for (int i = 0; i < (rand() % 4) + 1; i++) {                                                                                    // add random rotation
      if (TetrisHelper::canTetrisRotate(upcoming)) {
         upcoming.rotate();
      } else {
         break;
      }
   }
   DrawHelper::drawUpcoming(&upcoming);
}

int Game::getDroppingFrames() const {
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
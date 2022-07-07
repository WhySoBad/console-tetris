#include "Game.h"
#include "ncurses.h"
#include "algorithm"
#include "chrono"
#include "set"
#include "vector"
#include "helper/TetrisHelper.h"
#include "helper/DrawHelper.h"
#include "helper/ConfigHelper.h"

using namespace std;

bool gameover = false;
int step = 0;  // step indicating the current cycle tick; the object should only move every fourth tick

Game::Game() {
    ConfigHelper::initialize();
    DrawHelper::initialize();
    TetrisHelper::initialize();

    framerate = getDroppingFrames();

    auto seed = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now()).time_since_epoch().count();
    srand(seed);  // add seed to rand();

    DrawHelper::drawBorder();
    active = TetrisHelper::generateTetromino();
    upcoming = TetrisHelper::generateTetromino();

    DrawHelper::drawUpcoming(upcoming);
    refresh();
}

void Game::draw() {
    timeout(0);  // don't wait for user input
    int key = getch(); // get current key
    flushinp();  // clear input buffer

    if (key == 'p' && !paused && !gameover) {
        paused = true;
        key = -1;  // make sure the key information isn't used twice
    } else if(key == 'r') return reset();
    else if(key == 'e') {
        stop();
        exit(0);
    }

    if (!gameover && !paused) {
        if (step >= getDroppingFrames()) {
            if (TetrisHelper::canTetrisMove(active, Direction::vertical, 1)) {
                DrawHelper::clearTetris(active);
                active->moveY(1);
                DrawHelper::drawPreviewLine(active);
                DrawHelper::drawTetris(active);
            }
        }

        switch (key) {
            case KEY_UP:
                if (TetrisHelper::canTetrisRotate(active)) {
                    DrawHelper::clearTetris(active);
                    DrawHelper::drawPreviewLine(active, EMPTY_SPACE);
                    active->rotate();
                    DrawHelper::drawPreviewLine(active);
                    DrawHelper::drawTetris(active);
                }
                break;
            case KEY_DOWN: {
                int shortest = FIELD_HEIGHT - active->getY() - active->getHeight();  // default shortest distance
                for (int i = 0; i < active->getPoints().size(); i++) {
                    Point point = active->getPoints()[i];
                    int highestFixed = TetrisHelper::getHighestFixed(point.getX(),
                                                                     active->getHeight() + active->getY());
                    int _shortest = highestFixed - point.getY() - 1;
                    if (_shortest < shortest) shortest = _shortest;
                }
                if (TetrisHelper::canTetrisMove(active, Direction::vertical, shortest)) {
                    DrawHelper::clearTetris(active);
                    DrawHelper::drawPreviewLine(active, EMPTY_SPACE);
                    active->moveY(shortest);
                    score += shortest;
                    if(ConfigHelper::getHighscore() < score) ConfigHelper::setHighscore(score);
                    DrawHelper::drawPreviewLine(active);
                    DrawHelper::drawTetris(active);
                }
                break;
            }
            case KEY_RIGHT:
                if (TetrisHelper::canTetrisMove(active, Direction::horizontal, 1)) {
                    DrawHelper::clearTetris(active);
                    DrawHelper::drawPreviewLine(active, EMPTY_SPACE);
                    active->moveX(1);
                    DrawHelper::drawPreviewLine(active);
                    DrawHelper::drawTetris(active);
                }
                break;
            case KEY_LEFT:
                if (TetrisHelper::canTetrisMove(active, Direction::horizontal, -1)) {
                    DrawHelper::clearTetris(active);
                    DrawHelper::drawPreviewLine(active, EMPTY_SPACE);
                    active->moveX(-1);
                    DrawHelper::drawPreviewLine(active);
                    DrawHelper::drawTetris(active);
                }
                break;
        }

        if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_LEFT || step >= getDroppingFrames()) {
            if (!TetrisHelper::canTetrisMove(active, Direction::vertical, 1)) fixActive();
        }
    } else if (paused) {
        if (key == 'p') {
            DrawHelper::clearUpcoming();
            DrawHelper::drawUpcoming(upcoming);
            paused = false;
        } else DrawHelper::drawPaused();
    }

    DrawHelper::drawStatistics(score, level, cleared);
    refresh();
    if (step >= getDroppingFrames()) step = 0;
    else step += 1;
}

void Game::start() {
    step = 0;
    running = true;
    const float tick = 1000.0f / (float) framerate;
    do {
        chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
        draw();
        chrono::high_resolution_clock::time_point after = chrono::high_resolution_clock::now();
        float elapsed = static_cast<float>(chrono::duration_cast<chrono::milliseconds>(after - now).count());
        napms(static_cast<int>(tick - elapsed));  // await remaining milliseconds of the tick
    } while (running);
}

void Game::stop() {
    running = false; // stop the game cycle
    endwin(); // stop the ncurses mode and reenter the default terminal
}


void Game::fixActive() {
    DrawHelper::drawPreviewLine(active, EMPTY_SPACE);  // remove preview line
    bool outOfScreen = false;

    vector<int> modifiedRows = vector<int>();
    for (Point point: active->getPoints()) { // update fixed points in the array
        if (point.getY() >= 0)
            TetrisHelper::setFixedPoint(point.getX(), point.getY(), static_cast<short>(active->getType() + 1)); // store color for the point
        else outOfScreen = true;

        bool existing = false;
        for (int row: modifiedRows) {
            if (row == point.getY()) {
                existing = true;
                break;
            }
        }
        if (!existing) modifiedRows.push_back(point.getY());
    }

    if (outOfScreen || active->getY() <= 0) {
        gameOver();
        return;
    }

    // clear full rows
    vector<int> fullRows = vector<int>();
    for (int row: modifiedRows) {
        array<short, FIELD_WIDTH> line = TetrisHelper::getFixedLine(row);
        bool isFull = true;
        for (short entry: line) {
            if (!entry) {
                isFull = false;
                break;
            }
        }
        if (isFull) fullRows.push_back(row);
    }

    for (int row: fullRows) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
            TetrisHelper::setFixedPoint(j, row, 0); // not occupied
            auto *point = new Point(j, row);
            DrawHelper::drawPoint(point, EMPTY_SPACE);
        }
    }

    sort(fullRows.begin(), fullRows.end()); // sort full rows in ascending order

    for (int row: fullRows) {
        int highestY = FIELD_HEIGHT;
        for (int i = 0; i < FIELD_WIDTH; i++) {  // determine the highest occupied row
            int highest = TetrisHelper::getHighestFixed(i);
            if (highest < highestY) highestY = highest;
        }

        if (highestY > row) continue;  // if no higher occupied row go to next one
        for (int j = 0; j <= row - highestY; j++) {
            int _y = row - j;
            if (j == row - highestY) {  // last occupied row should be cleared
                for (int k = 0; k < FIELD_WIDTH; k++) {
                    TetrisHelper::setFixedPoint(k, _y, 0); // not occupied
                    auto *point = new Point(k, _y);
                    DrawHelper::drawPoint(point, EMPTY_SPACE);
                }
            } else {  // every not-last occupied row gets moved one index lower
                TetrisHelper::setFixedLine(_y, TetrisHelper::getFixedLine(_y - 1));
                for (int k = 0; k < FIELD_WIDTH; k++) {
                    auto *point = new Point(k, _y);
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

    cleared += static_cast<int>(fullRows.size());
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

    if(ConfigHelper::getHighestLevel() < level) ConfigHelper::setHighestLevel(level);
    if(ConfigHelper::getHighscore() < score) ConfigHelper::setHighscore(score);
    ConfigHelper::addDestroyedLines(fullRows.size());

    // update active object

    active = upcoming;
    upcoming = TetrisHelper::generateTetromino();
    DrawHelper::drawUpcoming(upcoming);
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
    ConfigHelper::addRound();
    DrawHelper::clearUpcoming();
    int paddingX = DrawHelper::getPaddingX() + FIELD_WIDTH * POINT_WIDTH * DrawHelper::getUiSize() + POINT_WIDTH;
    int paddingY = DrawHelper::getPaddingY() + (UPCOMING_HEIGHT * DrawHelper::getUiSize()) / 2;
    DrawHelper::printAt(paddingX, paddingY - 2, "Gameover!");
    DrawHelper::printAt(paddingX, paddingY, "Press 'r' to");
    DrawHelper::printAt(paddingX, paddingY + 1, "replay!");
    refresh();
    gameover = true;
}

void Game::reset() {
    DrawHelper::clearStatistics();
    DrawHelper::clearField();
    refresh();
    gameover = false;
    paused = false;
    level = 0;
    score = 0;
    cleared = 0;
    TetrisHelper::initialize(); // reset the tetris helper
    active = TetrisHelper::generateTetromino();
    upcoming = TetrisHelper::generateTetromino();
    DrawHelper::drawUpcoming(upcoming);
}

#include "TetrisHelper.h"

bool TetrisHelper::isPointInScreen(Point *point) {
    return point->getX() >= 0 && point->getX() < FIELD_WIDTH && point->getY() >= 0 && point->getY() < FIELD_HEIGHT;
}

bool TetrisHelper::isPointInField(Point *point) {
    return point->getX() >= 0 && point->getX() < FIELD_WIDTH && point->getY() < FIELD_HEIGHT;
}

bool TetrisHelper::canTetrisRotate(Tetris tetris) {
    tetris.rotate();
    for (Point point : tetris.getPoints()) {
        if(!isPointInField(&point)) return false;
    }
    return !doesTetrisIntersect(&tetris);
}

bool TetrisHelper::canTetrisMove(Tetris tetris, Direction direction, int delta) {
    if(direction == Direction::vertical) tetris.moveY(delta);
    else tetris.moveX(delta);
    for (Point point : tetris.getPoints()) {
        if(!isPointInField(&point)) return false;
    }
    return !doesTetrisIntersect(&tetris);
}

bool TetrisHelper::doesTetrisIntersect(Tetris *tetris) {
    for (Point point : tetris->getPoints()) {
        if(isPointInScreen(&point)) {
            if(getHighestFixed(point.getX()) == 0 && point.getY() < 0) return true;
            if(isPointInField(&point) && fixed.at(point.getY()).at(point.getX())) return true;
        }
    }
    return false;
}

int TetrisHelper::getHighestFixed(int x, int higher) {
    if(x < 0 || x >= FIELD_WIDTH) return FIELD_HEIGHT;
    for (int k = 0; k < FIELD_HEIGHT; k++) {
        if(fixed.at(k).at(x) && k >= higher) return k;
    }
    return FIELD_HEIGHT;
}

short TetrisHelper::getFixedPoint(int x, int y) {
    if(isPointInScreen(new Point(x, y))) return fixed.at(y).at(x);
    else return 0;
}

void TetrisHelper::setFixedPoint(int x, int y, short value) {
    if(isPointInScreen(new Point(x, y))) fixed.at(y).at(x) = value;
}

array<short, FIELD_WIDTH> TetrisHelper::getFixedLine(int y) {
    return fixed.at(y);
}

void TetrisHelper::setFixedLine(int y, array<short, FIELD_WIDTH> row) {
    fixed.at(y) = row;
}


array<array<short, FIELD_WIDTH>, FIELD_HEIGHT> TetrisHelper::getFixed() {
    return fixed;
}

void TetrisHelper::initialize() {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        array<short, FIELD_WIDTH> row{};
        for (int j = 0; j < FIELD_WIDTH; j++) row[j] = 0;  // not occupied
        fixed[i] = row;
    }
}

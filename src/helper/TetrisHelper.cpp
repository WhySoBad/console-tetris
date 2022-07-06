#include "ConsoleHelper.h"
#include "../game.h"

bool ConsoleHelper::isPointInScreen(Point *point) {
    return point->getX() >= 0 && point->getX() < FIELD_WIDTH && point->getY() >= 0 && point->getY() < FIELD_HEIGHT;
}

bool ConsoleHelper::isPointInField(Point *point) {
    return point->getX() >= 0 && point->getX() < FIELD_WIDTH && point->getY() < FIELD_HEIGHT;
}

bool ConsoleHelper::canTetrisRotate(Tetris *tetris) {
    Tetris copy = *tetris;
    copy.rotate();
    for (Point point : copy.getPoints()) {
        if(!isPointInField(&point)) return false;
    }
    return !doesTetrisIntersect(tetris);
}

bool ConsoleHelper::canTetrisMove(Tetris *tetris, Direction direction, int delta) {
    Tetris copy = *tetris;
    if(direction == Direction::vertical) copy.moveY(delta);
    else copy.moveX(delta);
    for (Point point : copy.getPoints()) {
        if(!isPointInField(&point)) return false;
    }
    return !doesTetrisIntersect(tetris);
}

bool ConsoleHelper::doesTetrisIntersect(Tetris *tetris) {
    for (Point point : tetris->getPoints()) {
        if(isPointInScreen(&point)) {
            if(getHighestFixed(point.getX()) == 0 && point.getY() < 0 || fixed[point.getY()][point.getX()]) return true;
        }
    }
    return false;
}

int ConsoleHelper::getHighestFixed(int x, int higher) {
    for (int k = 0; k < fixed.size(); ++k) {
        if(fixed[i][x] && i >= higher) return i;
    }
    return static_cast<int>(fixed.size());
}

void ConsoleHelper::initialize(vector<vector<short>> &_fixed) {
    fixed = _fixed;
}


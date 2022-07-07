#include "random"
#include "algorithm"
#include "TetrisHelper.h"
#include "../tetromino/TetrominoI.h"
#include "../tetromino/TetrominoJ.h"
#include "../tetromino/TetrominoL.h"
#include "../tetromino/TetrominoO.h"
#include "../tetromino/TetrominoS.h"
#include "../tetromino/TetrominoT.h"
#include "../tetromino/TetrominoZ.h"

inline std::mt19937 &generator() {
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

bool TetrisHelper::isPointInScreen(Point *point) {
    return point->getX() >= 0 && point->getX() < FIELD_WIDTH && point->getY() >= 0 && point->getY() < FIELD_HEIGHT;
}

bool TetrisHelper::isPointInField(Point *point) {
    return point->getX() >= 0 && point->getX() < FIELD_WIDTH && point->getY() < FIELD_HEIGHT;
}

bool TetrisHelper::canTetrisRotate(Tetris *tetris) {
    Tetris *copy = TetrisHelper::copyTetromino(tetris);
    copy->rotate();
    for (Point point : copy->getPoints()) {
        if(!isPointInField(&point)) return false;
    }
    return !doesTetrisIntersect(copy);
}

bool TetrisHelper::canTetrisMove(Tetris *tetris, Direction direction, int delta) {
    Tetris *copy = TetrisHelper::copyTetromino(tetris);
    if(direction == Direction::vertical) copy->moveY(delta);
    else copy->moveX(delta);
    for (Point point : copy->getPoints()) {
        if(!isPointInField(&point)) return false;
    }
    return !doesTetrisIntersect(copy);
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

    for (short k = 0; k < static_cast<short>(tetrominoBlock.size()); ++k) tetrominoBlock[k] = k;
    std::shuffle(tetrominoBlock.begin(), tetrominoBlock.end(), generator());
}

Tetris *TetrisHelper::generateTetromino() {
    Tetris* tetris;

    auto startXDistribution = std::uniform_int_distribution<int>(0, FIELD_WIDTH);
    int x = startXDistribution(generator());

    if(currentBlock == tetrominoBlock.size() - 1) {
        std::shuffle(begin(tetrominoBlock), end(tetrominoBlock), generator());
        currentBlock = 0;
    }

    switch (tetrominoBlock[currentBlock]) {
        case i:
            tetris = new TetrominoI(x);
            break;
        case j:
            tetris = new TetrominoJ(x);
            break;
        case l:
            tetris = new TetrominoL(x);
            break;
        case o:
            tetris = new TetrominoO(x);
            break;
        case s:
            tetris = new TetrominoS(x);
            break;
        case t:
            tetris = new TetrominoT(x);
            break;
        case z:
            tetris = new TetrominoZ(x);
            break;
    }

    currentBlock++;
    if(tetris->getX() + tetris->getWidth() >= FIELD_WIDTH) tetris->moveX(FIELD_WIDTH - tetris->getX() - tetris->getWidth());
    auto rotationDistribution = std::uniform_int_distribution<int>(0, 4);
    for (int k = 0; k < rotationDistribution(generator()); ++k) {
        if(TetrisHelper::canTetrisRotate(tetris)) tetris->rotate();
        else break;
    }
    return tetris;
}

Tetris *TetrisHelper::copyTetromino(Tetris *tetris) {
    switch (tetris->getType()) {
        case i: {
            auto *original = dynamic_cast<TetrominoI*>(tetris);
            auto *copy = new TetrominoI(*original);
            return copy;
        }
        case j: {
            auto *original = dynamic_cast<TetrominoJ*>(tetris);
            auto *copy = new TetrominoJ(*original);
            return copy;
        }
        case l: {
            auto *original = dynamic_cast<TetrominoL*>(tetris);
            auto *copy = new TetrominoL(*original);
            return copy;
        }
        case o: {
            auto *original = dynamic_cast<TetrominoO*>(tetris);
            auto *copy = new TetrominoO(*original);
            return copy;
        }
        case s: {
            auto *original = dynamic_cast<TetrominoS*>(tetris);
            auto *copy = new TetrominoS(*original);
            return copy;
        }
        case t: {
            auto *original = dynamic_cast<TetrominoT*>(tetris);
            auto *copy = new TetrominoT(*original);
            return copy;
        }
        case z: {
            auto *original = dynamic_cast<TetrominoZ*>(tetris);
            auto *copy = new TetrominoZ(*original);
            return copy;
        }
    }
    return tetris;
}

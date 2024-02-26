#include "algorithm"
#include "sstream"
#include "csignal"
#include "DrawHelper.h"
#include "TetrisHelper.h"
#include "cmath"
#include "sys/ioctl.h"
#include "ncurses.h"
#include "ConfigHelper.h"
#include "unistd.h"

void DrawHelper::drawTetris(Tetris *tetris, const char *character, int color) {
    for (Point point : tetris->getPoints()) drawPoint(&point, character, color);
}

void DrawHelper::drawTetris(Tetris *tetris) {
    DrawHelper::drawTetris(tetris, OCCUPIED_SPACE, tetris->getType() + 1);
}

void DrawHelper::drawPaused() {
    DrawHelper::clearUpcoming();
    int startX = FIELD_WIDTH + 2;
    int endX = FIELD_WIDTH + MENU_WIDTH - 3;
    int startY = 1;
    int endY = UPCOMING_HEIGHT - 1;

    for (int k = 0; k < endY - startY; ++k) {
        auto *p1 = new Point(startX, startY + k);
        auto *p2 = new Point(endX, startY + k);
        DrawHelper::drawPoint(p1, OCCUPIED_SPACE, 9, true);
        DrawHelper::drawPoint(p2, OCCUPIED_SPACE, 9, true);
    }
}

void DrawHelper::clearTetris(Tetris *tetris) {
    DrawHelper::drawTetris(tetris, EMPTY_SPACE);
}

void DrawHelper::clearField() {
    for (int k = 0; k < FIELD_HEIGHT * DrawHelper::getUiSize(); ++k) {
        for (int m = 0; m < FIELD_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH; ++m) {
            int x = DrawHelper::getPaddingX() + m;
            int y = DrawHelper::getPaddingY() + k;
            DrawHelper::printAt(x, y, EMPTY_SPACE);
        }
    }
}

void DrawHelper::drawPoint(Point *point, const char *character, int color, bool ignore, bool letter) {
    int scale = letter ? LETTER_SIZE : DrawHelper::getUiSize();
    if(ignore || TetrisHelper::isPointInScreen(point)) {
        DrawHelper::useColor(color);
        for (int k = 0; k < pow(scale, 2) * POINT_WIDTH; ++k) {
            int row = k / POINT_WIDTH / DrawHelper::getUiSize();
            int x = POINT_WIDTH * scale * point->getX() + DrawHelper::getPaddingX() + k - row * POINT_WIDTH * scale;
            if(point->getX() > FIELD_WIDTH && point->getX() < FIELD_WIDTH + MENU_WIDTH) x++;
            int y = scale * point->getY() + DrawHelper::getPaddingY() + row;
            DrawHelper::printAt(x, y, character);
        }
        DrawHelper::useColor(color, true);
    }
}

void DrawHelper::drawBorder() {
    struct winsize size{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int height = size.ws_row;
    int width = size.ws_col;
    int totalFieldHeight = FIELD_HEIGHT * DrawHelper::getUiSize() + 2;
    int totalFieldWidth = (FIELD_WIDTH + MENU_WIDTH) * DrawHelper::getUiSize() * POINT_WIDTH + 3;

    // calculate field padding on both axes
    if(width > totalFieldWidth) paddingX = (int) (width - totalFieldWidth) / 2;
    else if(width < totalFieldWidth) {} // handle small terminal error
    if(height > totalFieldHeight) paddingY = (int) (height - totalFieldHeight) / 2;
    else if(height < totalFieldHeight) {} // handle small terminal error

    // draw the border of the field
    for (int k = 0; k < totalFieldHeight; ++k) {
        for (int m = 0; m < totalFieldWidth; ++m) {
            int x = DrawHelper::getPaddingX() + m - 1;
            int y = DrawHelper::getPaddingY() + k - 1;
            if(k == 0 && m == 0) DrawHelper::printAt(x, y, TOP_LEFT_CORNER);
            else if(k == 0 && m == totalFieldWidth - 1) DrawHelper::printAt(x, y, TOP_RIGHT_CORNER);
            else if(k == totalFieldHeight - 1 && m == 0) DrawHelper::printAt(x, y, BOTTOM_LEFT_CORNER);
            else if(k == totalFieldHeight - 1 && m == totalFieldWidth - 1) DrawHelper::printAt(x, y, BOTTOM_RIGHT_CORNER);
            else if(k == 0 || k == totalFieldHeight - 1) DrawHelper::printAt(x, y, HORIZONTAL_BORDER);
            else if(m == 0 || m == totalFieldWidth - 1) DrawHelper::printAt(x, y, VERTICAL_BORDER);
        }
    }

    // draw menu separator line
    for (int k = 0; k < totalFieldHeight; ++k) {
        int x = DrawHelper::getPaddingX() + FIELD_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH;
        int y = DrawHelper::getPaddingY() + k - 1;
        if(k == 0) DrawHelper::printAt(x, y, TOP_CROSSING_BORDER);
        else if(k == totalFieldHeight - 1) DrawHelper::printAt(x, y, BOTTOM_CROSSING_BORDER);
        else DrawHelper::printAt(x, y, VERTICAL_BORDER);
    }

    // draw upcoming separator line
    for (int k = 0; k < MENU_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH + 2; ++k) {
        int x = DrawHelper::getPaddingX() + FIELD_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH + k;
        int y = DrawHelper::getPaddingY() + UPCOMING_HEIGHT * DrawHelper::getUiSize();
        if(k == 0) DrawHelper::printAt(x, y, LEFT_CROSSING_BORDER);
        else if(k == MENU_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH + 1) DrawHelper::printAt(x, y, RIGHT_CROSSING_BORDER);
        else DrawHelper::printAt(x, y, HORIZONTAL_BORDER);
    }
}

void DrawHelper::drawPreviewLine(Tetris *tetris, const char *character) {
    for (Point point : tetris->getPoints()) {
        if(point.getY() != tetris->getY() + tetris->getHeight() - 1) continue; // only let the lowest points pass
        for (int k = 0; k < TetrisHelper::getHighestFixed(point.getX(), tetris->getHeight() + tetris->getY()) - point.getY(); ++k) {
            auto * dot = new Point(point.getX(), point.getY() + 1 + k);
            if(TetrisHelper::isPointInScreen(dot) && !TetrisHelper::getFixedPoint(dot->getX(), dot->getY())) DrawHelper::drawPoint(dot, character, 8);
        }
    }
}

void DrawHelper::drawUpcoming(Tetris *tetris) {
    DrawHelper::clearUpcoming();

    int offsetX = (MENU_WIDTH - tetris->getHeight()) / 2;
    int offsetY = (UPCOMING_HEIGHT - tetris->getWidth()) / 2;

    for (Point point : tetris->getPoints()) {
        auto * preview = new Point(FIELD_WIDTH + 1 + offsetY + point.getX() - tetris->getX(), offsetX + point.getY() - tetris->getY());
        DrawHelper::drawPoint(preview, OCCUPIED_SPACE, tetris->getType() + 1, true);
    }
}

void DrawHelper::clearUpcoming() {
    for (int k = 0; k < UPCOMING_HEIGHT * DrawHelper::getUiSize(); ++k) {
        for (int m = 0; m < MENU_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH; ++m) {
            int x = DrawHelper::getPaddingX() + FIELD_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH + 1 + m;
            int y = DrawHelper::getPaddingY() + k;
            DrawHelper::printAt(x, y, EMPTY_SPACE);
        }
    }
}

void DrawHelper::drawStatistics(int score, int level, int lines) {
    auto * point = new Point(DrawHelper::getPaddingX() + FIELD_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH + 2, DrawHelper::getPaddingY() + UPCOMING_HEIGHT * DrawHelper::getUiSize() + 1 * DrawHelper::getUiSize());
    auto * scPoint = new Point(DrawHelper::getPaddingX() + FIELD_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH + 2, DrawHelper::getPaddingY() + FIELD_HEIGHT * DrawHelper::getUiSize() - 1 * DrawHelper::getUiSize());
    auto drawStatistic = [&point](const char * name, int value) {
        attron(A_BOLD);
        DrawHelper::printAt(point->getX(), point->getY(), name);
        attroff(A_BOLD);
        point->moveY(1);
        std::stringstream stream;
        stream.imbue(std::locale(""));
        stream << value;
        std::string str = stream.str();
        std::replace(str.begin(), str.end(), ',', '\'');
        DrawHelper::printAt(point->getX(), point->getY(), str.c_str());
        point->moveY(2);
    };

    drawStatistic("Score", score);
    drawStatistic("Highscore", ConfigHelper::getHighscore());
    drawStatistic("Level", level);
    drawStatistic("Lines", lines);
}

void DrawHelper::clearStatistics() {
    for (int k = 0; k < (FIELD_HEIGHT - UPCOMING_HEIGHT) * DrawHelper::getUiSize() - 1; ++k) {
        for (int m = 0; m < MENU_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH; ++m) {
            int x = DrawHelper::getPaddingX() + FIELD_WIDTH * DrawHelper::getUiSize() * POINT_WIDTH + 1 + m;
            int y = DrawHelper::getPaddingY() + UPCOMING_HEIGHT * DrawHelper::getUiSize() + 1 + k;
            DrawHelper::printAt(x, y, EMPTY_SPACE);
        }
    }
}

void DrawHelper::useColor(int color, bool disable) {
    if(color >= 0) {
        if(!disable) attron(COLOR_PAIR(color));
        else attroff(COLOR_PAIR(color));
    }
}

void DrawHelper::printAt(int x, int y, const char *characters) {
    mvprintw(y, x, "%s", characters);
}


void DrawHelper::initialize() {
    struct winsize size{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int height = size.ws_row, width = size.ws_col;
    paddingX = 0;
    paddingY = 0;

    auto fitsScreen = [&height, &width](int &uiSize) {
        return height >= FIELD_HEIGHT * uiSize + 2 && width >= (FIELD_WIDTH + MENU_WIDTH) * uiSize * POINT_WIDTH + 3;
    };

    while(!fitsScreen(uiSize) && uiSize > 0) uiSize--;
    if(uiSize == 0) {
        printf("Your terminal doesn't fit the required dimensions! [height: %i -> required: %i; width: %i -> required: %i]\n", height, FIELD_HEIGHT + 2, width, FIELD_WIDTH + MENU_WIDTH + 3);
        fflush(stdout);
        exit(0);
    }

    setlocale(LC_ALL, ""); // enable unicodes

    // setup ncurses
    WINDOW *win = newwin(height, width, 0, 0); // create a new ncurses window
    initscr(); // init the screen
    cbreak(); // enable cbreak mode to get inputs without pressing enter and interruption signals
    noecho(); // disable character echoing
    keypad(stdscr, TRUE); // enable arrow-keys
    use_default_colors(); // enable default terminal colors for a transparent background
    start_color(); // enable colors
    curs_set(0); // disable cursor
    nodelay(win, TRUE); // disable key wait delay

    // add color pairs
    init_color(24, 1000, 533, 0);    // orange color -> used in L-Tetromino
    init_color(25, 309, 309, 309);   // light gray color -> used in preview dot

    init_pair(1, -1, COLOR_CYAN);     // I-Tetromino
    init_pair(2, -1, COLOR_BLUE);     // J-Tetromino
    init_pair(3, -1, 24);             // L-Tetromino
    init_pair(4, -1, COLOR_YELLOW);   // O-Tetromino
    init_pair(5, -1, COLOR_GREEN);    // S-Tetromino
    init_pair(6, -1, COLOR_MAGENTA);  // T-Tetromino
    init_pair(7, -1, COLOR_RED);      // Z-Tetromino
    init_pair(8, 25, -1);             // preview dot
    init_pair(9, -1, COLOR_WHITE);    // menu stuff
}

int DrawHelper::getPaddingX() {
    return paddingX;
}

int DrawHelper::getPaddingY() {
    return paddingY;
}

int DrawHelper::getUiSize() {
    return uiSize;
}




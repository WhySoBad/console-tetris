#include "FontHelper.h"
#include "DrawHelper.h"
#include "TetrisHelper.h"

void FontHelper::drawNumber(Point *point, float number, bool clear) {
    int power = 0;
    while (number > 1000){ number /= 1000; power++; }
    string chars = to_string(number);

    for (int k = 0; k < chars.length(); ++k) {
        int offsetX = LETTER_WIDTH * k + k * LETTER_SIZE;
        auto * position = new Point(point->getX() + offsetX, point->getY());
        if(chars[i] == '.') drawCharacter(position, '.', clear);
        else {
            short digit = static_cast<short>(stoi(chars.substr(i, 1)));
            FontHelper::drawDigit(position, digit, clear);
        }
    }

    if(power > 0) {
        int offsetX = (LETTER_WIDTH + LETTER_SIZE) * chars.length();
        auto * position = new Point(point->getX() + offsetX, point->getY());
        FontHelper::drawCharacter(position, power == 1 ? 'K' : 'M', clear);
    }
}

void FontHelper::drawDigit(Point *point, short digit, bool clear) {
    array<bool, 7> segments = array<bool, 7>({false, false, false, false, false, false, false});  // 7 segment
    switch (digit) {
        case 0: for (bool & segment : segments) segment = true; segments[6] = false; break;
        case 1: segments[1] = true; segments[2] = true; break;
        case 2: for (bool & segment : segments) segment = true; segments[2] = false; segments[5] = false; break;
        case 3: for (bool & segment : segments) segment = true;segments[4] = false; segments[5] = false; break;
        case 4: for (bool & segment : segments) segment = true; segments[0] = false; segments[3] = false; segments[4] = false; break;
        case 5: for (bool & segment : segments) segment = true; segments[1] = false; segments[4] = false; break;
        case 6: for (bool & segment : segments) segment = true; segments[1] = false; break;
        case 7: segments[0] = true; segments[1] = true; segments[2] = true; break;
        case 8: for (bool & segment : segments) segment = true; break;
        case 9: for (bool & segment : segments) segment = true; segments[4] = false; break;
    }

    int padding = LETTER_WIDTH + 1;

    for (int k = 0; k < segments.size(); ++k) {
        if(!segments[i]) continue;
        switch (i) {
            case 0:
            case 3:
            case 6: {
                int x = point->getX();
                int y = point->getY() + i == 3 ? padding * 2 : i == 6 ? padding : 0;
                for (int m = 0; m < LETTER_WIDTH; ++m) {
                    auto * position = new Point(x + m, y);
                    DrawHelper::drawPoint(position, clear ? EMPTY_SPACE : OCCUPIED_SPACE, clear ? -1 : 9, true, true);
                }
                break;
            }
            case 1:
            case 2:
            case 4:
            case 5: {
                int x = point->getX() + (i == 1 || i == 2 ? padding : 0);
                int y = point->getY() + (i == 2 || i == 4 ? padding : 0);
                for (int j = 0; j < LETTER_WIDTH; j++) {
                    auto * position = new Point(x, y + j);
                    DrawHelper::drawPoint(position, clear ? EMPTY_SPACE : OCCUPIED_SPACE, clear ? -1 : 9, true, true);
                }
                break;
            }

        }
    }
}

void FontHelper::drawCharacter(Point *point, char character, bool clear) {
    array<bool, LETTER_HEIGHT * LETTER_WIDTH> segments = array<bool, LETTER_HEIGHT * LETTER_WIDTH>();
    for (bool & segment : segments) segment = false;

    switch (character) {
        case 'K': {
            for (int i = 0; i < segments.size(); i++) {
                bool atBeginning = i % LETTER_WIDTH == 0;
                bool atEnd = i % LETTER_WIDTH == LETTER_WIDTH - 1;
                bool middleY = i / LETTER_WIDTH == (int)(LETTER_HEIGHT / 2);
                if (atBeginning || atEnd && !middleY || !atBeginning && !atEnd && middleY) segments[i] = true;
            }
            break;
        }
        case 'M': {
            for (int i = 0; i < segments.size(); i++) {
                bool atBeginning = i % LETTER_WIDTH == 0;
                bool atEnd = i % LETTER_WIDTH == LETTER_WIDTH - 1;
                if (atBeginning || atEnd || i / LETTER_WIDTH == 1 && !atBeginning && !atEnd) segments[i] = true;
            }
            break;
        }
        case '.': {
            segments[segments.size() - (int)(LETTER_WIDTH / 2) - 1] = true;
            break;
        }
    }

    for (int i = 0; i < segments.size(); i++) {
        if (!segments[i]) continue;
        int x = i % LETTER_WIDTH + point->getX();
        int y = i / LETTER_WIDTH + point->getY();
        auto * position = new Point(x, y);
        DrawHelper::drawPoint(position, clear ? EMPTY_SPACE : OCCUPIED_SPACE, clear ? -1 : 9, true, true);
    }

}

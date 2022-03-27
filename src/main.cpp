#include <iostream>
#include <unistd.h>
#include "game.h"
#include <termios.h>
#include <string.h>
#include <chrono>
#include <csignal>
using namespace std;

#define CONSOLE_CLEAR "\x1B[2J\x1B[H";

struct termios orig_term_attr;
struct termios new_term_attr;
Game game = Game(3);

void handleSignal(int signum)
{
  game.stop();
  tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
  exit(signum);
}

int main(int argc, char *args[])
{
  signal(SIGINT, handleSignal); // add sigint exit
  auto seed = chrono::time_point_cast<chrono::microseconds>(chrono::system_clock::now()).time_since_epoch().count();
  cout << seed << "\n";
  srand(seed); // add seed to rand();
  cout << rand() % 7 << " - " << rand() % 7 << " - " << rand() % 7 << "\n";

  // disable default terminal behaviour

  tcgetattr(fileno(stdin), &orig_term_attr);
  memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
  new_term_attr.c_lflag &= ~(ECHO | ICANON);
  new_term_attr.c_cc[VTIME] = 0;
  new_term_attr.c_cc[VMIN] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);
  game.start();

  sleep(1000); // prevent process from stopping [TODO]

  //┏━┓
  //┃ ┃
  //┗━┛
}

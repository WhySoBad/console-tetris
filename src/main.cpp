#include <csignal>

#include "game.h"
using namespace std;

Game game = Game(60);

void handleSignal(int signum) {
   game.stop();
   exit(signum);
}

int main(int argc, char *args[]) {
   signal(SIGINT, handleSignal);   // add sigint exit
   signal(SIGTERM, handleSignal);  // add sigterm exit
   game.start();
   return 0;
}

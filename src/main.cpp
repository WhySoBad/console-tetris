#include "csignal"
#include "Game.h"

Game game = Game();

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

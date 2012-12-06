#include "game.h"

int main(int argc, char *argv[])
{
    Game* game;
    game = (Game*) malloc (sizeof(Game));
    game_init(game);
    game_welcome(game);
    return 0;
}

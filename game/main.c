#include "game.h"
int main(int argc, char *argv[])
{
    Game* game;
    game = (Game*) malloc (sizeof(Game));
    game_init(game);
    game->start_round(game);
    game->discard_round(game);
    game->show_results(game);
    return 0;
}

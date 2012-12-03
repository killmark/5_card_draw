#include "game.h"

int main(int argc, char *argv[])
{
    Game* game;
    game = (Game*) malloc (sizeof(Game));
    game_init(game);
    
    game->blind_betting(game);
    game->start_round(game);

    printf ("First betting round: \n");
    game->reg_betting(game);

    printf ("Discard round: \n");
    game->discard_round(game);

    printf ("Second betting round: \n");
    game->reg_betting(game);
    game->show_results(game);
    return 0;
}

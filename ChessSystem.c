#include "chessSystem.h"
#include "map.h"
#include <stdio.h>
#include <assert.h>

#define NUM_IDS 2


struct chess_system_t
{
    Map tournaments; //Keys will be tournament id, data will be the struct tournament that contains more information
};

typedef struct tournament_t
{
    char* tournament_location;
    int max_games_per_player;
    int games_played;
    Map games; //Keys will arrays of player ids, data will be the struct game
} *Tournament;

typedef struct game_t
{
    Winner winner;
    int play_time;
    int winner_id;
} *Game;


static Tournament tournamentCopyData(Tournament tournament);


static void tournamentsFreeData(Tournament tournament);


static int* tournamentsCopyKey(int* tournament_id)
{
    int* new_id = malloc(sizeof(*new_id));
    if (new_id == NULL)
    {
        return NULL;
    }
    *new_id = *tournament_id;
    return new_id;
}


static void tournamentsFreeKey(int* tournament_id)
{
    free(key);
}


static int tournamentsKeyCompare(int* id_1, int* id_2)
{
    return *id_1 - *id_2;
}




static Game gamesCopyData(Game game);


static void gamesFreeData(Game game);


static int* gamesCopyKey(int* player_ids)
{
    int new_ids[NUM_IDS] = {player_ids[0], player_ids[1]};
    return new_ids;
}


static void gamesFreeKey(int* player_ids); //צריך בכלל? לא הקצינו דינמית


static int gamesKeyCompare();
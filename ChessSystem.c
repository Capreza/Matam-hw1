#include "chessSystem.h"
#include "map.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define NUM_IDS 2
#define EQUAL 0
#define NOT_EQUAL 1


struct chess_system_t
{
    Map tournaments; //Keys will be tournament id, data will be the struct tournament that contains more information
};

typedef struct tournament_t
{
    char* tournament_location;
    int max_games_per_player;
    int games_played;
    int winner_id;
    Map games; //Keys will arrays of player ids, data will be the struct game
} *Tournament;

typedef struct game_t
{
    Winner winner;
    int play_time;
} *Game;


static Tournament tournamentCopyData(Tournament tournament)
{
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    if (new_tournament == NULL || tournament == NULL)
    {
        return NULL;
    }
    new_tournament->tournament_location = tournament->tournament_location; //להעביר לפונקציית עזר של גל?
    new_tournament->max_games_per_player = tournament->max_games_per_player;
    new_tournament->games_played = tournament->games_played;
    new_tournament->winner_id = tournament->winner_id;
    new_tournament->games = mapCopy(tournament->games);
    assert(new_tournament->games != NULL);
    return new_tournament;
}


static void tournamentsFreeData(Tournament tournament)
{
    mapDestroy(tournament->games);
    free(tournament);
}


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
    free(tournament_id);
}


static int tournamentsKeyCompare(int* id_1, int* id_2)
{
    return *id_1 - *id_2;
}



static Game gameCreate(Winner winner, int play_time)
{
    Game new_game = malloc(sizeof(*new_game));
    if (new_game == NULL)
    {
        return NULL;
    }
    new_game->play_time = play_time;
    new_game->winner = winner;
    return new_game;
}


static Game gamesCopyData(Game game)
{
    Game new_game = gameCreate(game->winner, game->play_time);
    if (new_game == NULL)
    {
        return NULL;
    }
    return new_game;
}


static void gamesFreeData(Game game)
{
    free(game);
}


static int* gamesCopyKey(int* player_ids)
{
    int new_ids = malloc(NUM_IDS * sizeof(int));
    if (new_ids == NULL)
    {
        return NULL;
    }
    new_ids[0] = player_ids[0];
    new_ids[1] = player_ids[1];
    return new_ids;
}


static void gamesFreeKey(int* player_ids)
{
    free(player_ids);
}


static int gamesKeyCompare(int* player_ids1, int* player_ids2)
{
    if (player_ids1[0] == player_ids2[0])
    {
        if (player_ids1[1] == player_ids2[1])
        {
            return EQUAL;
        }
        return NOT_EQUAL;
    }
    if (player_ids1[0] == player_ids2[1])
    {
        if (player_ids1[1] == player_ids2[0])
        {
            return EQUAL;
        }
        return NOT_EQUAL;
    }
    return NOT_EQUAL;
}




static ChessResult gameCreateAndInsert(ChessSystem chess, Tournament tournament, int* new_ids, \
Winner winner, int play_time)
{
    Game new_game = gameCreate(winner, play_time);
    int* new_ids_copied = gamesCopyKey(new_ids);
    ChessResult result = mapPut(tournament->games, new_ids_copied, new_game);
    assert (result != NULL);
    if (result == MAP_OUT_OF_MEMORY)
    {
        free(new_game);
        return CHESS_OUT_OF_MEMORY;
    }
    ++(tournament->games_played);
    assert (result == MAP_SUCCESS);
    return CHESS_SUCCESS;
}


void chessDestroy(ChessSystem chess)
{
    if (chess == NULL)
    {
        return;
    }
    mapDestroy(chess->tournaments);
    free(chess);
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, \
int second_player, Winner winner, int play_time)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if ((tournament_id < 0) || (first_player <= 0) || (second_player <= 0) || (second_player == first_player))
    {
        return CHESS_INVALID_ID;
    } 
    Tournament curr_tournament = mapGet(chess->tournaments, &tournament_id);
    if (curr_tournament == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if (curr_tournament->winner_id != 0)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    int new_ids[NUM_IDS] = {first_player, second_player}; // לתקן????
    if (mapContains(curr_tournament->games, new_ids))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if (play_time < 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    int max_games = curr_tournament->max_games_per_player;
    if (chessGamesPerGivenPlayer(first_player) >= max_games || \
    chessGamesPerGivenPlayer(second_player) >= max_games)
    {
        return CHESS_EXCEEDED_GAMES;
    }
    return chessCreateAndInsert(chess, curr_tournament, new_ids, winner, play_time);
}


ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    bool player_exists_in_tournament = false;
    MAP_FOREACH(int*, tournament_iterator, chess->tournaments)
    {
        Tournament curr_tournament = mapGet(tournament_iterator);

        if (curr_tournament->winner_id != 0)
        {
            MAP_FOREACH(int*, game_iterator, curr_tournament->games)
            {
                int result0 = gamesKeyCompare(&game_iterator[0], &player_id);
                int result1 = gamesKeyCompare(&game_iterator[1], &player_id);
                if ( result0 == 0 || result1 == 0)
                {
                    int* new_ids = malloc(sizeof(NUM_IDS * int));
                    if (new_ids == NULL)
                    {
                        //do something....
                    }
                    new_ids[0] = (result0 == 0) ? 0 : game_iterator[0];
                    new_ids[1] = (result1 == 0) ? 0 : game_iterator[1];
                    int other_player = (result0 == 0) ? game_iterator[1] : game_iterator[0];
                    assert(other_player != player_id);
                    int play_time = ((Game) (mapGet(curr_tournament->games, game_iterator)))->play_time;
                    if (gameCreateAndInsert(chess, curr_tournament, new_ids, other_player, play_time) \
                    == CHESS_OUT_OF_MEMORY)
                    {
                        return CHESS_OUT_OF_MEMORY;
                    }
                    assert(mapRemove(curr_tournament->games, game_iterator) == MAP_SUCCESS);
                    player_exists_in_tournament = true;
                }
            }
        }
    }
    if (player_exists_in_tournament == false)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    return CHESS_SUCCESS;
}


double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{

}
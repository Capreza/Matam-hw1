#include "chessSystem.h"
#include "map.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define NUM_IDS 2
#define EQUAL 0
#define NOT_EQUAL 1
#define FIRST 1
#define SECOND 2


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
    Map players; //keys are ids and data is struct
} *Tournament;

typedef struct game_t
{
    Winner winner;
    int play_time;
} *Game;

typedef struct players_t
{
    int num_wins;
    int num_draws;
    int num_loses;
    int current_tournament_score;
} *Player;


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



int* playersCopyKey(int* id)
{
    int* id_copy = malloc(sizeof(*id_copy));
    if (id_copy == NULL)
    {
        return NULL;
    }
    return id_copy;
}


void playersFreeKey(int* id)
{
    free(id);
}


int playersKeyCompare(int* id_1, int* id_2)
{
    return *id_1 - *id_2
}


Player playersCopyData(Player player)
{
    Player new_player = malloc(sizeof(*new_player));
    if (new_player == NULL)
    {
        return NULL;
    }
    new_player->current_tournament_score = player->current_tournament_score;
    new_player->num_draws = player->num_draws;
    new_player->num_loses = player->num_loses;
    new_player->num_wins = player->num_wins;
    return new_player;
}


void playersFreeData(Player player)
{
    free(player);
}


Player playerCreate()
{
    Player new_player = malloc(sizeof(*new_player));
    if (new_player == NULL)
    {
        return NULL;
    }
    new_player->current_tournament_score = 0;
    new_player->num_draws = 0;
    new_player->num_loses = 0;
    new_player->num_wins = 0;
    return new_player;
}


void playerIncreaseCount(Tournament tournament,int player_id, Winner winner, int index)
{
    assert(tournament != NULL);
    Player curr_player = mapGet(tournament->players, player_id);
    if (curr_player == NULL)
    {
        curr_player = playerCreate();
        if (curr_player == NULL)
        {
            return CHESS_OUT_OF_MEMORY;
        }
    }
    if (winner == DRAW)
    {
        ++(curr_player->num_draws);
        ++(curr_player->curr_tournament_score);
    }
    else if ((winner == FIRST_PLAYER && index == FIRST) || (winner == SECOND_PLAYER && index === SECOND))
    {
        ++(curr_player->num_wins);
        curr_player->curr_tournament_score += 2;
    }
    else
    {
        ++(curr_player->num_loses);
    }
    MapResult result = mapPut(tournament->players, player_id, curr_player);
    if (result == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    assert(result != MAP_NULL_ARGUMENT);
    return CHESS_SUCCESS;
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
    playerIncreaseCount(tournament, new_ids[0], winner, FIRST);
    playerIncreaseCount(tournament, new_ids[1], winner, SECOND);
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


ChessResult chessRemovePlayer(ChessSystem chess, int player_id) //make sure i changed winner
{
    if (chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    bool player_exists_in_system = false, player_exists_in_tournament = false;
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
                        return CHESS_OUT_OF_MEMORY;
                    }
                    new_ids[0] = (result0 == 0) ? 0 : game_iterator[0];
                    new_ids[1] = (result1 == 0) ? 0 : game_iterator[1];
                    Winner new_winner = (result0 == 0) ? SECOND_PLAYER : FIRST_PLAYER;
                    int play_time = ((Game) (mapGet(curr_tournament->games, game_iterator)))->play_time;
                    if (gameCreateAndInsert(chess, curr_tournament, new_ids, new_winner, play_time) \
                    == CHESS_OUT_OF_MEMORY)
                    {
                        free(new_ids);
                        return CHESS_OUT_OF_MEMORY;
                    }
                    assert(mapRemove(curr_tournament->games, game_iterator) == MAP_SUCCESS);
                    player_exists_in_system = true;
                    player_exists_in_tournament = true;
                }
            }
            if (player_exists_in_tournament == true)
            {
                MapResult result = mapRemove(curr_tournament->players, player_id);
                assert(result != MAP_ITEM_DOES_NOT_EXIST && result == MAP_SUCCESS);
            }
        }
        player_exists_in_tournament = false;
    }
    if (player_exists_in_tournament == false)
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    return CHESS_SUCCESS;
}


double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if (chess == NULL || chess_result == NULL) // chess_result cant be NULL?
    {
        return CHESS_NULL_ARGUMENT;
    }
    if (player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    MAP_FOREACH(int*, tournament_iterator, chess->tournaments)
    {
        MAP_FOREACH(int*, game_iterator, curr_tournament->games)
        {
            if (game_iterator[0] == player_id || game_iterator[1] == player_id)
            {
            }
        }
    }
}


//GAL WORKING DOWN HERE 


ChessSystem chessCreate() 
{
    Map tournament_map = mapCreate(tournamentCopyData, tournamentsCopyKey, 
                tournamentsFreeData, tournamentsFreeKey, tournamentsKeyCompare);
    if (tournament_map == NULL)
    {
        return NULL;
    }
    ChessSystem chess = malloc(sizeof(*chess));
    if(chess==NULL)
    {
        mapDestroy(tournament_map);
        return NULL;
    }
    return chess;
}


ChessResult chessAddTournament (ChessSystem chess, int
tournament_id, int max_games_per_player, const char*
tournament_location)
{
    if(tournament_id <=0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournaments, tournament_id)==1)
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if(chessTournamentLocationValid(tournament_location) ==false)
    {
        return CHESS_INVALID_LOCATION;
    }
    if(max_games_per_player<=0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }
    Tournament tournament =chessTournamentCreate(max_games_per_player, tournament_location);
    if (tournament==NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    assert(chess->tournaments != NULL);
    if (mapPut(chess->tournaments, &tournament_id, tournament) == MAP_OUT_OF_MEMORY)
    {
        free(tournament);
        return CHESS_OUT_OF_MEMORY;
    }
    else
    {
        free(tournament);
        return CHESS_SUCCESS;
    }
    
}


bool chessTournamentLocationValid(const char* tournament_location)
{
    if(tournament_location[0]<='Z' && tournament_location[0] >='A')
    {
        for(int i=1;tournament_location[i];i++)
        {
            if((tournament_location[i] <'a' || tournament_location[i]>'z')&&(tournament_location[i]!=' '))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

Tournament chessTournamentCreate(int max_games_per_player, const char* tournament_location)
{
    assert(max_games_per_player! == 0 && strlen(tournament_location) == 0); //CHANGED
    Tournament tournament = malloc(sizeof(*tournament));
    if (tournament == NULL)
    {
        return NULL;
    }
    tournament->max_games_per_player = max_games_per_player;
    strcpy(tournament->tournament_location, tournament_location);
    assert(tournament->tournament_location != NULL);
    tournament->games_played = 0;
    tournament->winner_id = 0;
    tournament->games = mapCreate(gamesCopyData, gamesCopyKey, gamesFreeData, gamesFreeKey, \
    gamesKeyCompare);

    return tournament;
}


ChessResult chessRemoveTournament (ChessSystem chess, int
tournament_id)
{
    if(tournament_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    if (mapRemove(chess->tournaments, tournament_id) == MAP_SUCCESS)
    {
    return CHESS_SUCCESS;
    }
    return CHESS_TOURNAMENT_NOT_EXIST;
}


ChessResult chessEndTournament (ChessSystem chess, int  
tournament_id)                                                //THIS NEEDS MORE WORK
{ 
    if(tournament_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    Tournament tournament = mapGet(chess->tournaments, tournament_id);
    if (tournament == NULL)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if(tournament->winner_id !=0)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    //can you remove a player from tournament after it ended? no.
  
    //we built a player stat struct;
    int tournament_winner_id=NULL;
    MAP_FOREACH(int*, game_iterator, tournament->games)
    {
        //need to decide exactly how to do this
    }
    tournament->winner_id = tournament_winner_id;
    return CHESS_SUCCESS;
}


ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)  //i want to decide how we save player stats before i write this
{
    if(chess==NULL || file==NULL)
    {
        return CHESS_SAVE_FAILURE;
    }

    int number_of_players = 0; //need to think about this line
    for(int i =0 ; i<number_of_players;i++)
    {
        if(fprintf(file, "%d %d\n",PLAYER_ID, PLAYER_LEVEL)<=0)   //this line is wrong on purpose
        {
            return CHESS_SAVE_FAILURE;
        }
    }
    fclose(file);
}

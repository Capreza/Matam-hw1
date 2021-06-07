#include "chessSystem.h"
#include "map.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define NUM_IDS 2
#define EQUAL 0
#define NOT_EQUAL 1
#define FIRST 1
#define SECOND 2
#define WIN_SCORE 2



struct chess_system_t
{
    Map tournaments; //Keys - tournament id, data - struct tournament
};

typedef struct tournament_t
{
    char* tournament_location;
    int max_games_per_player;
    int games_played;
    int winner_id;
    Map games; //Keys - arrays of player ids, data - struct game
    Map players; //keys - ids, data - struct player
    int num_of_players;
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
    bool removed;
} *Player;



static MapDataElement tournamentCopyData(MapDataElement tournament);
static void tournamentsFreeData(MapDataElement tournament);
static MapKeyElement tournamentsCopyKey(MapKeyElement tournament_id);
static void tournamentsFreeKey(MapKeyElement tournament_id);
static int tournamentsKeyCompare(MapKeyElement id_1, MapKeyElement id_2);

static Game gameCreate(Winner winner, int play_time);
static MapDataElement gamesCopyData(MapDataElement game);
static void gamesFreeData(MapDataElement game);
static MapKeyElement gamesCopyKey(MapKeyElement player_ids);
static void gamesFreeKey(MapKeyElement player_ids);
static int gamesKeyCompare(MapKeyElement player_ids1, MapKeyElement player_ids2);

static Player playerCreate();
static MapDataElement playersCopyData(MapDataElement player);
static void playersFreeData(MapDataElement player);
static MapKeyElement playersCopyKey(MapKeyElement id);
static void playersFreeKey(MapKeyElement id);
static int playersKeyCompare(MapKeyElement id_1, MapKeyElement id_2);
static MapResult addPlayer(Map players, int player_id);
static void playerChangeCount(Tournament tournament, int* new_ids, int* old_ids, Winner winner);

static int chessGamesPerGivenPlayer(Map players, int player_id);
static ChessResult playerIncreaseCount(Tournament tournament,int player_id, Winner winner, int index);
static ChessResult gameCreateAndInsert(ChessSystem chess, Tournament tournament, int* new_ids, \
    Winner winner, int play_time, int* old_ids);
static int longestGameTime(Tournament tournament);
static double tournamentAverageGameTime(Tournament tournament);
static bool chessTournamentLocationValid(const char* tournament_location);
static Tournament chessTournamentCreate(int max_games_per_player, const char* tournament_location);
static int chessFindTournamentWinner(ChessSystem chess, int tournament_id);
static void getSortedLevelArray(Map global_player_stats, float *sorted_level_array);
static void placeInGlobalStatArray(float* sorted_level_array, int index, int id, float level, int num_of_players);
static void getGlobalStatistics(ChessSystem chess, Map global_player_stats);


static MapDataElement tournamentCopyData(MapDataElement tournament)
{
    if (tournament == NULL)
    {
        return NULL;
    }
    Tournament copy = malloc(sizeof(*copy));
    
    if (copy == NULL)
    {
        return NULL;
    }
    char* new_location = malloc(strlen(((Tournament)tournament)->tournament_location)+1);
    if (new_location == NULL)
    {
        free(copy);
        return NULL;
    }
    assert(((Tournament) tournament->games != NULL) && ((Tournament) tournament->players != NULL) && \
    (chessTournamentLocationValid((Tournament) tournament->tournament_location)) && \
    ((Tournament) tournament->max_games_per_player > 0) && ((Tournament) tournament->winner_id >= 0));
    strcpy(new_location, ((Tournament)tournament)->tournament_location);
    copy->tournament_location = new_location; 
    copy->max_games_per_player = ((Tournament) tournament)->max_games_per_player;
    copy->games_played = ((Tournament) tournament)->games_played;
    copy->winner_id = ((Tournament) tournament)->winner_id;
    copy->num_of_players = ((Tournament) tournament)->num_of_players;
    copy->games = mapCopy(((Tournament) tournament)->games);
    if (copy->games == NULL)
    {
        free(copy);
        return NULL;
    }
    copy->players = mapCopy(((Tournament) tournament)->players);
    if (copy->players == NULL)
    {
        free(copy->games);
        free(copy);
        return NULL;
    }
    return copy;
}


static void tournamentsFreeData(MapDataElement tournament)
{
    if(tournament!=NULL)
    {
      mapDestroy(((Tournament) tournament)->games);
      mapDestroy(((Tournament) tournament)->players);
      if(((Tournament)tournament)->tournament_location!=NULL)
      {
           free((((Tournament)tournament)->tournament_location));
      }
       free(tournament);
    }
}


static MapKeyElement tournamentsCopyKey(MapKeyElement tournament_id)
{
    if (tournament_id == NULL)
    {
        return NULL;
    }
    int* copy = malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    *copy = *(int*)tournament_id;
    return copy;
}


static void tournamentsFreeKey(MapKeyElement tournament_id)
{
    free(tournament_id);
}


static int tournamentsKeyCompare(MapKeyElement id_1, MapKeyElement id_2)
{
    return *(int*)id_1 - *(int*)id_2;
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


static MapDataElement gamesCopyData(MapDataElement game)
{
    if (game == NULL)
    {
        return NULL;
    }
    Game copy = gameCreate(((Game) game)->winner, ((Game) game)->play_time);
    if (copy == NULL)
    {
        return NULL;
    }
    return copy;
}


static void gamesFreeData(MapDataElement game)
{
    free(game);
}


static MapKeyElement gamesCopyKey(MapKeyElement player_ids)
{
    if (player_ids == NULL)
    {
        return NULL;
    }
    int* copy = malloc(NUM_IDS * sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    copy[0] = ((int*) player_ids)[0];
    copy[1] = ((int*) player_ids)[1];
    return copy;
}


static void gamesFreeKey(MapKeyElement player_ids)
{
    free(player_ids);
}


static int gamesKeyCompare(MapKeyElement player_ids1, MapKeyElement player_ids2)
{
    if (((int*) player_ids1)[0] == ((int*) player_ids2)[0])
    {
        if (((int*) player_ids1)[1] == ((int*) player_ids2)[1])
        {
            return EQUAL;
        }
        return NOT_EQUAL;
    }
    if (((int*) player_ids1)[0] == ((int*) player_ids2)[1])
    {
        if (((int*) player_ids1)[1] == ((int*) player_ids2)[0])
        {
            return EQUAL;
        }
        return NOT_EQUAL;
    }
    return NOT_EQUAL;
}



static MapKeyElement playersCopyKey(MapKeyElement id)
{
    if (id == NULL)
    {
        return NULL;
    }
    int* copy = malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    *copy = *(int*)id;
    return copy;
}


static void playersFreeKey(MapKeyElement id)
{
    free(id);
}


static int playersKeyCompare(MapKeyElement id_1, MapKeyElement id_2)
{
    return *(int*)id_1 - *(int*)id_2;
}


static MapDataElement playersCopyData(MapDataElement player)
{
    if (player == NULL)
    {
        return NULL;
    }
    Player copy = malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    copy->current_tournament_score = ((Player) player)->current_tournament_score;
    copy->num_draws = ((Player) player)->num_draws;
    copy->num_loses = ((Player) player)->num_loses;
    copy->num_wins = ((Player) player)->num_wins;
    copy->removed = ((Player) player)->removed;
    return copy;
}


static void playersFreeData(MapDataElement player)
{
    free(player);
}


static Player playerCreate()
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
    new_player->removed = false;
    return new_player;
}


static MapResult addPlayer(Map players, int player_id) //changed this
{
    if (players == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    if (mapContains(players, &player_id))
    {
        return MAP_SUCCESS;
    }
    Player new_player = playerCreate();
    if (new_player == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(players, &player_id, new_player);
    free(new_player);
    return result;
}


static int chessGamesPerGivenPlayer(Map players, int player_id)
{
    if(players ==NULL )
    {
        return 0;
    }
    Player current_player = mapGet(players, &player_id);
    assert (current_player != NULL);
    if(current_player == NULL)
    {
        return 0;
    }
    int games_per_given_player =current_player->num_draws + current_player->num_loses + current_player->num_wins;
    return games_per_given_player;
}


static ChessResult playerIncreaseCount(Tournament tournament,int player_id, Winner winner, int index)
{
    assert(tournament != NULL);
    Player curr_player = mapGet(tournament->players, &player_id);
    assert(curr_player!=NULL);
    if(curr_player->num_wins == 0 && curr_player->num_loses == 0 && curr_player->num_draws == 0)
    {
        (tournament->num_of_players)++;
    }
    if (winner == DRAW)
    {
        ++(curr_player->num_draws);
        ++(curr_player->current_tournament_score);
    }
    else if ((winner == FIRST_PLAYER && index == FIRST) || (winner == SECOND_PLAYER && index == SECOND))
    {
        ++(curr_player->num_wins);
        curr_player->current_tournament_score += WIN_SCORE;
    }
    else
    {
        ++(curr_player->num_loses);
    }
    curr_player->removed=false;
    return CHESS_SUCCESS;
}


static ChessResult gameCreateAndInsert(ChessSystem chess, Tournament tournament, int* new_ids,  //check if new ids are null
Winner winner, int play_time, int *old_ids)
{
    Game new_game = gameCreate(winner, play_time);
    if (new_game == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(tournament->games, new_ids, new_game);
    free(new_game);
    if (result == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    assert (result == MAP_SUCCESS);
    if(old_ids!=NULL)
    {
        playerChangeCount(tournament,new_ids,old_ids,winner);
    }
    else 
    {
        playerIncreaseCount(tournament, new_ids[0], winner, FIRST);
        playerIncreaseCount(tournament, new_ids[1], winner, SECOND);
        ++(tournament->games_played);
    }
    return CHESS_SUCCESS;
}

static void playerChangeCount(Tournament tournament, int* new_ids, int* old_ids, Winner winner)
{
    Game old_game = mapGet(tournament->games, old_ids);
    Player first_player = mapGet(tournament->players, old_ids);
    Player second_player = mapGet(tournament->players, (old_ids+1));
    if(new_ids[0] == 0 && new_ids[1]==0)
    {
        return;
    }
    if(old_game->winner == DRAW)
    {
        if(new_ids[0] != 0)
        {
            first_player->num_draws--;
            first_player->num_wins++;
            first_player->current_tournament_score++;
        }
        if(new_ids[1] != 0)
        {
            second_player->num_draws--;
            second_player->num_wins++;
            second_player->current_tournament_score++;
        }
        return; 
    }
    if(old_game->winner == FIRST_PLAYER && new_ids[0] == 0)
    {
        second_player->num_loses--;
        second_player->num_wins++;
        second_player->current_tournament_score+=2;

        return;
    }
    if(old_game->winner == SECOND_PLAYER && new_ids[1] == 0)
    {
        first_player->num_loses--;
        first_player->num_wins++;
        first_player->current_tournament_score+=2;
        
        return;
    }
    return;


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
    int new_ids[NUM_IDS] = {first_player, second_player};
    if (mapContains(curr_tournament->games, new_ids))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }

    if (play_time < 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    if(addPlayer(curr_tournament->players, first_player)!=MAP_SUCCESS) //NEED TO CHANGE THIS SEGMENT TO HANDLE THE MEMORY ERROR->think i did
    {
        return CHESS_OUT_OF_MEMORY;
    }
    if(addPlayer(curr_tournament->players, second_player)!=MAP_SUCCESS)
    {
        Player first_player_stats = mapGet(curr_tournament->players, &first_player);
        if(first_player_stats->num_wins==0 && first_player_stats->num_draws == 0 && first_player_stats->num_loses==0)
        {
            mapRemove(curr_tournament->players, &first_player);
        }
        return CHESS_OUT_OF_MEMORY;
    }
    int max_games = curr_tournament->max_games_per_player;
    if (chessGamesPerGivenPlayer(curr_tournament->players, first_player) >= max_games || 
    chessGamesPerGivenPlayer(curr_tournament->players, second_player) >= max_games)
    {
        return CHESS_EXCEEDED_GAMES;
    }

    return gameCreateAndInsert(chess, curr_tournament, new_ids, winner, play_time, NULL);
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
    bool player_exists_in_system = false, player_exists_in_tournament = false;
    bool changed = false;
    do
    {
        changed = false;
        MAP_FOREACH(int*, tournament_iterator, chess->tournaments)
        {
            Tournament curr_tournament = mapGet(chess->tournaments, tournament_iterator);
            Player player_to_remove = mapGet(curr_tournament->players, &player_id);
            if (player_to_remove!= NULL)
            {
                player_exists_in_system+= !(player_to_remove->removed);
            }
            if (curr_tournament->winner_id == 0)
            {
                MAP_FOREACH(int*, game_iterator, curr_tournament->games)
                {
                    bool result0 = (game_iterator[0] == player_id);
                    bool result1 = (game_iterator[1] == player_id);
                    if ( result0 == true || result1 == true)
                    {
                        int new_ids[NUM_IDS]= {0};
                        new_ids[0] = (result0 == true) ? 0 : game_iterator[0];
                        new_ids[1] = (result1 == true) ? 0 : game_iterator[1];
                        Winner new_winner = (result0 == true) ? SECOND_PLAYER : FIRST_PLAYER;
                        int play_time = ((Game) (mapGet(curr_tournament->games, game_iterator)))->play_time;
                        if (gameCreateAndInsert(chess, curr_tournament, new_ids, new_winner, play_time, game_iterator) \
                         == CHESS_OUT_OF_MEMORY)
                        {
                            return CHESS_OUT_OF_MEMORY;
                        }
                        mapRemove(curr_tournament->games, game_iterator);
                        player_exists_in_tournament = true;
                    }
                    gamesFreeKey(game_iterator);
                }
                if (player_exists_in_tournament == true)
                {
                    Player current_player = mapGet(curr_tournament->players, &player_id);
                    if (current_player != NULL)
                    {
                        changed = true;
                        current_player->removed = true;
                        current_player->num_draws = 0;
                        current_player->num_wins = 0;
                        current_player->num_loses = 0;
                        current_player->current_tournament_score = 0;

                    }
                }
            }
            player_exists_in_tournament = false;
            tournamentsFreeKey(tournament_iterator);
        }
    } while (changed == true);
    if (player_exists_in_system == false)
        {
            return CHESS_PLAYER_NOT_EXIST;
        }

    return CHESS_SUCCESS;
}


double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if (chess == NULL || chess_result == NULL) // chess_result cant be NULL?
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return CHESS_NULL_ARGUMENT;
    }
    if (player_id <= 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return 0;
    }
    Game current_game = NULL;
    int total_time = 0, num_games_played = 0;
    MAP_FOREACH(int*, tournament_iterator, chess->tournaments)
    {
        Tournament curr_tournament = mapGet(chess->tournaments, tournament_iterator);
        MAP_FOREACH(int*, game_iterator, curr_tournament->games)
        {
            if (game_iterator[0] == player_id || game_iterator[1] == player_id)
            {
                current_game = (Game) mapGet(curr_tournament->games, game_iterator);
                total_time += current_game->play_time;
                ++num_games_played;
            }
            gamesFreeKey(game_iterator);
        }
        tournamentsFreeKey(tournament_iterator);
    }
    if (num_games_played == 0)
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;
    }
    *chess_result = CHESS_SUCCESS;
    return ((double) total_time) / num_games_played;
}


static int longestGameTime(Tournament tournament)
{
    assert(tournament != NULL);
    int current_play_time = 0, max_time = 0;
    MAP_FOREACH(int*, game_iterator, tournament->games)
    {
        current_play_time = ((Game) mapGet(tournament->games, game_iterator))->play_time;
        max_time = (current_play_time > max_time) ? current_play_time : max_time;
        gamesFreeKey(game_iterator);
    }
    return max_time;
}


static double tournamentAverageGameTime(Tournament tournament)
{
    assert(tournament != NULL);
    if (tournament->games_played == 0)
    {
        return 0;
    }

    int total_time=0, current_play_time=0;
    MAP_FOREACH(int*, game_iterator, tournament->games)
    {
        current_play_time = ((Game) mapGet(tournament->games, game_iterator))->play_time;
        total_time += current_play_time;
        gamesFreeKey(game_iterator);
    }
    return ((double) total_time)/(tournament->games_played);
}
    

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    
    if (chess == NULL || strlen(path_file) == 0)
    {
        return CHESS_NULL_ARGUMENT;
    }
    FILE* file = fopen(path_file, "w");
    if (file == NULL)
    {
        return CHESS_SAVE_FAILURE;
    }
    bool tournaments_ended = false;
    Tournament current_tournament = NULL;
    MAP_FOREACH(int*, tournament_iterator, chess->tournaments)
    {
        current_tournament = mapGet(chess->tournaments, tournament_iterator);
        if (current_tournament->winner_id != 0)
        {
            tournaments_ended = true;            
            if (fprintf(file, "%d\n", current_tournament->winner_id) < 0 ||
            fprintf(file, "%d\n", longestGameTime(current_tournament)) < 0 ||
            fprintf(file, "%.2f\n", tournamentAverageGameTime(current_tournament)) < 0 ||
            fprintf(file, "%s\n", current_tournament->tournament_location) < 0 ||
            fprintf(file, "%d\n", current_tournament->games_played) < 0 ||
            fprintf(file, "%d\n", current_tournament->num_of_players) < 0)
            {
                free(tournament_iterator);
                return CHESS_SAVE_FAILURE;
            }     
        }   
        free(tournament_iterator);
    }
    fclose(file);
    if (tournaments_ended == false)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    return CHESS_SUCCESS;
}


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
    chess->tournaments = tournament_map;
    return chess;
}


ChessResult chessAddTournament (ChessSystem chess, int
tournament_id, int max_games_per_player, const char*
tournament_location) 
{

    if(chess == NULL || tournament_location == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id <=0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournaments, &tournament_id)==true)
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
    assert(tournament_id_copy!=NULL);
    MapResult res = mapPut(chess->tournaments, &tournament_id, tournament);
    tournamentsFreeData(tournament);
    if ( res == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    else if (res == MAP_NULL_ARGUMENT)
    {
        return CHESS_NULL_ARGUMENT;
    }
    else
    {
        return CHESS_SUCCESS;
    }
    
}


static bool chessTournamentLocationValid(const char* tournament_location)
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


static Tournament chessTournamentCreate(int max_games_per_player, const char* tournament_location)
{
    assert(max_games_per_player >0 && chessTournamentLocationValid (tournament_location )==true);
    Tournament tournament = malloc(sizeof(*tournament));
    if (tournament==NULL)
    {
        return NULL;
    }

    tournament->max_games_per_player = max_games_per_player;
    tournament->winner_id=0;
    tournament->num_of_players = 0;
    tournament->tournament_location = malloc(sizeof(char)*(strlen(tournament_location)+1));
    if(tournament->tournament_location == NULL)
    {
        tournamentsFreeData(tournament);
        return CHESS_OUT_OF_MEMORY;
    }
    strcpy(tournament->tournament_location, tournament_location);
    tournament->players = mapCreate(playersCopyData,playersCopyKey,
    playersFreeData,playersFreeKey,playersKeyCompare);
    if (tournament->players == NULL)
    {
        tournamentsFreeData(tournament);
        return NULL;
    }
    tournament->games = mapCreate(gamesCopyData,gamesCopyKey,
    gamesFreeData,gamesFreeKey,gamesKeyCompare);
    if (tournament->games == NULL)
    {
        tournamentsFreeData(tournament);
        return NULL;
    }
    tournament->games_played = 0;
    return tournament;
}


ChessResult chessRemoveTournament (ChessSystem chess, int
tournament_id)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    if (mapRemove(chess->tournaments, &tournament_id) == MAP_SUCCESS)
    {
    return CHESS_SUCCESS;
    }
    return CHESS_TOURNAMENT_NOT_EXIST;
}


ChessResult chessEndTournament (ChessSystem chess, int  
tournament_id)                                                
{ 
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id<=0)
    {
        return CHESS_INVALID_ID;
    }
    Tournament tournament = mapGet(chess->tournaments, &tournament_id);
    if (tournament == NULL)
    {        
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    if(tournament->winner_id !=0)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    tournament->winner_id = chessFindTournamentWinner(chess,tournament_id);
    if(tournament->winner_id==0)
    {
        return CHESS_NO_GAMES; 
    }
    return CHESS_SUCCESS;
}


static int chessFindTournamentWinner(ChessSystem chess, int tournament_id) 
{
    Tournament current_tournament = mapGet(chess->tournaments, &tournament_id);
    int current_winner_id = 0, current_top_score=0;
    Player current_winner = NULL;

    MAP_FOREACH(int*, player_iterator, current_tournament->players)
    {
        
        Player current_contender = mapGet(current_tournament->players, player_iterator);
        if(current_contender->removed == true)
        {
            free(player_iterator);
            continue;
        }
        int current_contender_score = current_contender->current_tournament_score;

        if(current_contender_score > current_top_score)
        {
            current_top_score = current_contender_score;
            current_winner = current_contender;
            current_winner_id = *player_iterator;
        }
        else if (current_contender_score == current_top_score && current_winner!=NULL)
        {
            if(current_contender->num_loses < current_winner->num_loses)
            {
                current_winner = current_contender;
                current_winner_id = *player_iterator;
            }
            else if (current_contender->num_loses > current_winner->num_loses)
            {
                continue;
            }
            else 
            {
                if   (current_contender->num_wins > current_winner->num_wins)
                {
                    current_winner = current_contender;
                    current_winner_id = *player_iterator;
                }
                else if (current_contender->num_wins < current_winner->num_wins)
                {
                    continue;
                }
                else
                {
                    if(*player_iterator<current_winner_id)
                    {
                        current_winner = current_contender;
                        current_winner_id = *player_iterator;
                    }
                }
            }
        }
        free(player_iterator);
    }
    return current_winner_id;
}


ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)  
{
    if(chess==NULL || file==NULL) 
    {
        return CHESS_NULL_ARGUMENT;
    }

    Map global_player_stats = mapCreate(playersCopyData, playersCopyKey,
    playersFreeData,playersFreeKey,playersKeyCompare);
    if(global_player_stats == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    getGlobalStatistics(chess, global_player_stats); 
    float *sorted_level_array =malloc(sizeof(float)*2*mapGetSize(global_player_stats));  //maybe i should define 2?
    if(sorted_level_array == NULL)
    {
        mapDestroy(global_player_stats);
        return CHESS_OUT_OF_MEMORY;
    }
    getSortedLevelArray(global_player_stats, sorted_level_array);
    int number_of_players = mapGetSize(global_player_stats);  
    for(int i =0 ; i<number_of_players;i++)
    {
        if(fprintf(file, "%d %.2f\n",(int)*(sorted_level_array+i),*(sorted_level_array+i+number_of_players))<=0)   
        {
            free(sorted_level_array);
            mapDestroy(global_player_stats);
            return CHESS_SAVE_FAILURE;
        }
    }
    mapDestroy(global_player_stats);
    free(sorted_level_array);
    return CHESS_SUCCESS;
}


static void getSortedLevelArray(Map global_player_stats, float *sorted_level_array)
{
    assert(global_player_stats != NULL && sorted_level_array != NULL);
    int num_of_players = mapGetSize(global_player_stats);
    for(int i = 0; i<num_of_players*2;i++)     //is 2 a magic number?
    {
        *(sorted_level_array+i) = 0;
    }
    MAP_FOREACH(int*, player_iterator, global_player_stats)
    {
        Player current_player_stats = mapGet(global_player_stats,player_iterator);
        int current_player_wins = current_player_stats->num_wins;
        int current_player_loses = current_player_stats->num_loses;
        int current_player_draws = current_player_stats->num_draws;
        float current_player_level = ((float) (current_player_wins*6-current_player_loses*10+current_player_draws*2)) 
        / (current_player_wins+current_player_loses+current_player_draws);  //need to define numbers 

        for(int i=0; i<num_of_players;i++)
        {
            int index_to_place = i; 
            float level_of_current_player = *(sorted_level_array+i+num_of_players);
            bool is_there_a_current_player = *(sorted_level_array+i);
            if(level_of_current_player > current_player_level && is_there_a_current_player == true)
            {
                continue;
            }
           if( level_of_current_player == current_player_level && *player_iterator>(int)*(sorted_level_array+i) && is_there_a_current_player == true)
           {
               index_to_place++;
           }
            placeInGlobalStatArray(sorted_level_array, index_to_place, *player_iterator,
            current_player_level, num_of_players);
            break;

        }
        free(player_iterator);

    }



}


static void placeInGlobalStatArray(float* sorted_level_array, int index, int id, float level, int num_of_players)  
{
    assert(index < num_of_players);
    if(*(sorted_level_array+index) == 0)
    {
        *(sorted_level_array+index) = id;
        *(sorted_level_array+index+num_of_players) = level;    }
    else
    {
        placeInGlobalStatArray(sorted_level_array, index+1, *(sorted_level_array+index),
         *(sorted_level_array+index+num_of_players), num_of_players);
        *(sorted_level_array+index) = id;
        *(sorted_level_array+index+num_of_players) = level;
    }

}


static void getGlobalStatistics(ChessSystem chess, Map global_player_stats)   
{
    assert(chess != NULL && global_player_stats != NULL);
    MAP_FOREACH(int*, tournament_iterator, chess->tournaments)
    {
        Tournament current_tournament = mapGet(chess->tournaments,tournament_iterator);
        MAP_FOREACH(int*, player_iterator, current_tournament->players)
        {
            Player current_player_local_stats = mapGet(current_tournament->players,player_iterator);
            Player current_player_global_stats = mapGet(global_player_stats,player_iterator);
            if(current_player_global_stats!=NULL)
            {
                
                Player current_player_tournament_stats = mapGet(current_tournament->players,player_iterator);
                current_player_global_stats->num_wins += current_player_tournament_stats->num_wins;
                current_player_global_stats->num_loses += current_player_tournament_stats->num_loses;
                current_player_global_stats->num_draws += current_player_tournament_stats->num_draws;
                
            }
            else
            {
                mapPut(global_player_stats,player_iterator,mapGet(current_tournament->players, player_iterator)); 
                current_player_global_stats = mapGet(global_player_stats,player_iterator); 
            }
            if(current_player_local_stats->removed==true)
            {
                current_player_global_stats->removed=true;
            }
            
            free(player_iterator);
        }
        free(tournament_iterator);
    }
    bool changed = false;
    do
    {
        changed = false;
        MAP_FOREACH(int*, player_iterator, global_player_stats)
        {
            Player current_player_global_stats = mapGet(global_player_stats,player_iterator);
            if(current_player_global_stats->removed==true)
            {
                mapRemove(global_player_stats,player_iterator);
                changed = true;
                free(player_iterator);
                break;
            }
            free(player_iterator);
        } 
    }
    while(changed == true);
}
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



static Tournament tournamentCopyData(Tournament tournament);
static void tournamentsFreeData(Tournament tournament);
static int* tournamentsCopyKey(int* tournament_id);
static void tournamentsFreeKey(int* tournament_id);
static int tournamentsKeyCompare(int* id_1, int* id_2);

static Game gameCreate(Winner winner, int play_time);
static Game gamesCopyData(Game game);
static void gamesFreeData(Game game);
static int* gamesCopyKey(int* player_ids);
static void gamesFreeKey(int* player_ids);
static int gamesKeyCompare(int* player_ids1, int* player_ids2);

static Player playerCreate();
static Player playersCopyData(Player player);
static void playersFreeData(Player player);
static int* playersCopyKey(int* id);
static void playersFreeKey(int* id);
static int playersKeyCompare(int* id_1, int* id_2);

static int chessGamesPerGivenPlayer(Map players, Player player);
static void playerIncreaseCount(Tournament tournament,int player_id, Winner winner, int index);
static ChessResult gameCreateAndInsert(ChessSystem chess, Tournament tournament, int* new_ids, \
    Winner winner, int play_time);
static int longestGameTime(Tournament tournament);
static double tournamentAverageGameTime(Tournament tournament);
static bool chessTournamentLocationValid(const char* tournament_location);
static Tournament chessTournamentCreate(int max_games_per_player, const char* tournament_location);
static int chessFindTournamentWinner(ChessSystem chess, int tournament_id);
static void getSortedLevelArray(Map global_player_stats, float *sorted_level_array);
static void placeInGlobalStatArray(float* sorted_level_array, int index, int id, int level, int num_of_players);
static void getGlobalStatistics(ChessSystem chess, Map global_player_stats);


static Tournament tournamentCopyData(Tournament tournament)
{
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    if (new_tournament == NULL || tournament == NULL)
    {
        return NULL;
    }
    assert((tournament->games != NULL) && (tournament->players != NULL) && \
    (chessTournamentLocationValid(tournament->tournament_location)) && (tournament->max_games_per_player > 0) \
    && (tournament->winner_id >= 0));
    new_tournament->tournament_location = tournament->tournament_location; 
    new_tournament->max_games_per_player = tournament->max_games_per_player;
    new_tournament->games_played = tournament->games_played;
    new_tournament->winner_id = tournament->winner_id;
    new_tournament->games = mapCopy(tournament->games);
    if (new_tournament->games == NULL)
    {
        free(new_tournament);
        return NULL;
    }
    new_tournament->players = mapCopy(tournament->players);
    if (new_tournament->players == NULL)
    {
        free(new_tournament->games);
        free(new_tournament);
        return NULL;
    }
    return new_tournament;
}


static void tournamentsFreeData(Tournament tournament)
{
    mapDestroy(tournament->games);
    mapDestroy(tournament->players);
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
    assert(player_ids != NULL);
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



static int* playersCopyKey(int* id)
{
    assert(id != NULL);
    int* id_copy = malloc(sizeof(*id_copy));
    if (id_copy == NULL)
    {
        return NULL;
    }
    return id_copy;
}


static void playersFreeKey(int* id)
{
    free(id);
}


static int playersKeyCompare(int* id_1, int* id_2)
{
    return *id_1 - *id_2
}


static Player playersCopyData(Player player)
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
    new_player->removed = player->removed;
    return new_player;
}


static void playersFreeData(Player player)
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



static int chessGamesPerGivenPlayer(Map players, Player player)
{
    assert (player != NULL);
    return player->num_draws + player->num_loses + player->num_wins;
}


static void playerIncreaseCount(Tournament tournament,int player_id, Winner winner, int index)
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
        curr_player->curr_tournament_score += WIN_SCORE;
    }
    else
    {
        ++(curr_player->num_loses);
    }
    curr_player->removed = false;
    MapResult result = mapPut(tournament->players, &player_id, curr_player);
    free(curr_player);
    if (result == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    assert(result == MAP_SUCCESS);
    return CHESS_SUCCESS;
}


static ChessResult gameCreateAndInsert(ChessSystem chess, Tournament tournament, int* new_ids, \
Winner winner, int play_time)
{
    Game new_game = gameCreate(winner, play_time);
    if (new_game == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    int* new_ids_copied = gamesCopyKey(new_ids);
    if (new_ids_copied == NULL)
    {
        free(new_game);
        return CHESS_OUT_OF_MEMORY;
    }
    ChessResult result = mapPut(tournament->games, new_ids_copied, new_game);
    assert (result != NULL);
    free(new_game);
    free(new_ids_copied);
    if (result == MAP_OUT_OF_MEMORY)
    {
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
    int new_ids[NUM_IDS] = {first_player, second_player};
    if (mapContains(curr_tournament->games, new_ids))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if (play_time < 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    int max_games = curr_tournament->max_games_per_player;
    if (chessGamesPerGivenPlayer(curr_tournament->players, first_player) >= max_games || \
    chessGamesPerGivenPlayer(curr_tournament->players, second_player) >= max_games)
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
    bool player_exists_in_system = false, player_exists_in_tournament = false;
    MAP_FOREACH(int*, tournament_iterator, chess->tournaments)
    {
        Tournament curr_tournament = mapGet(tournament_iterator);

        if (curr_tournament->winner_id == 0)
        {
            MAP_FOREACH(int*, game_iterator, curr_tournament->games)
            {
                bool result0 = (game_iterator[0] == player_id);
                bool result1 = (game_iterator[1] == player_id);
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
                    mapRemove(curr_tournament->games, game_iterator);
                    player_exists_in_system = true;
                    player_exists_in_tournament = true;
                }
                gamesFreeKey(game_iterator);
            }
            if (player_exists_in_tournament == true)
            {
                Player current_player = mapGet(curr_tournament->players, &player_id);
                current_player->removed = true;
            }
        }
        player_exists_in_tournament = false;
        tournamentsFreeKey(tournament_iterator);
    }
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
    int current_play_time, max_time = 0;
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

    int total_time, current_play_time;
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
        if (current_tournament->winner_id != 0)
        {
            tournaments_ended = true;
            if (fprintf(file, ("%d\n", current_tournament->winner_id)) < 0 ||
            fprintf(file, ("%d\n", longestPlayTime(current_tournament))) < 0 ||
            fprintf(file, ("%.2f\n", tournamentAverageGameTime(current_tournament))) < 0 ||
            fprintf(file, ("%s\n", current_tournament->tournament_location)) < 0 ||
            fprintf(file, ("%d\n", current_tournament->games_played)) < 0 ||
            fprintf(file, ("%d\n", mapGetSize(current_tournament->players))) < 0)
            {
                return CHESS_SAVE_FAILURE;
            } 
        }   
    }
    fclose(file);
    if (tournaments_ended == false)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    return CHESS_SUCCESS
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
    return chess;
}


ChessResult chessAddTournament (ChessSystem chess, int
tournament_id, int max_games_per_player, const char*
tournament_location) 
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id <=0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournaments, tournament_id)==true)
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
    strcpy(tournament->tournament_location, tournament_location);
    if(tournament->players = mapCreate(playersCopyData,playersCopyKey,
    playersFreeData,playersFreeKey,playersKeyCompare)== NULL)
    {
        free(tournament);
        return NULL;
    }
    if (tournament->games = mapCreate(gamesCopyData,gamesCopyKey,
    gamesFreeData,gamesFreeKey,gamesKeyCompare) == NULL)
    {
        mapDestroy(tournament->players);
        free(tournament);
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
    if (mapRemove(chess->tournaments, tournament_id) == MAP_SUCCESS)
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
    Tournament tournament = mapGet(chess->tournaments, tournament_id);
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
        return CHESS_NO_GAMES; //ADD TO CHESS.H
    }
    return CHESS_SUCCESS;
}


static int chessFindTournamentWinner(ChessSystem chess, int tournament_id) 
{
    Tournament current_tournament = mapGet(chess->tournaments, tournament_id);
    int current_winner_id = 0, current_top_score=0;
    Player current_winner = NULL;
    while(true)
    {
        int *current_winner_id_ptr = *((int*) mapGetFirst(current_tournament->players)); //IS THIS BLOCK OKAY?
        if(current_winner_id_ptr == NULL)
        {
            return 0;
        }
        current_winner_id = *current_winner_id_ptr;
        free(current_winner_id_ptr);
        current_winner =mapGet(current_tournament->players, current_winner_id); // need to see if we need to implicit cast
        current_top_score =current_winner->current_tournament_score;
        if(current_winner->removed == false)
        {
            break;
        }
    }
    MAP_FOREACH(int*, player_iterator, current_tournament->players)
    {
        
        Player current_contender = mapGet(current_tournament->players, player_iterator);
        if(current_contender->removed == true)
        {
            continue;
        }
        int current_contender_score = current_contender->current_tournament_score;
        if(current_contender_score > current_top_score)
        {
            current_top_score = current_contender_score;
            current_winner = current_contender;
            current_winner_id = *player_iterator;
        }
        else if (current_contender_score == current_top_score)
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
    if(chess==NULL || file==NULL) //is null file save error?
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
        if(fprintf(file, "%d %d\n",*(sorted_level_array+i),*(sorted_level_array+i+number_of_players))<=0)   
        {
            return CHESS_SAVE_FAILURE;
        }
    }
    
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
            if(level_of_current_player > current_player_level)
            {
                continue;
            }
           if( level_of_current_player == current_player_level && player_iterator>(int)*(sorted_level_array+i))   
           {
               index_to_place++;
           }
           placeInGlobalStatArray(sorted_level_array, index_to_place, player_iterator,
            current_player_level, num_of_players);

        }
        free(player_iterator);

    }



}


static void placeInGlobalStatArray(float* sorted_level_array, int index, int id, int level, int num_of_players)  
{
    if(*(sorted_level_array+index) == 0)
    {
        *(sorted_level_array+index) = id;
        *(sorted_level_array+index+num_of_players) = level;
    }
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
            Player current_player_global_stats = mapGet(global_player_stats,player_iterator);
            if(current_player_global_stats->removed == false)
                {
                if(current_player_global_stats)
                {
                    Player current_player_tournament_stats = mapGet(current_tournament->players,player_iterator);
                    current_player_global_stats->num_wins += current_player_tournament_stats->num_wins;
                    current_player_global_stats->num_loses += current_player_tournament_stats->num_loses;
                    current_player_global_stats->num_draws += current_player_tournament_stats->num_draws;
                }
                else
                {
                    mapPut(global_player_stats,player_iterator,mapGet(current_tournament->players, player_iterator)); //maybe we need to implicit cast
                    free(player_iterator);
                }
            }
        }
        
        free(tournament_iterator);
    }
}
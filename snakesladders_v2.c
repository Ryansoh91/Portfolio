#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Number of players
int n = 0;

// Players' figures in unicode
const char * const p_figures[] = {0, "\U0001F534", "\U0001F7E2", "\U0001F535", "\U0001F7E1"};

// Snake and ladder figures in unicode
const char * const sl_figures[] = {0, "\U0001F40D", "\U0001FA9C"};

// Snake and ladder indices
const int snake_index = 1, ladder_index = 2;

// Set board rows and cols
const int rows = 10, cols = 10;

// Set snakes and ladders numbers
int snakes_num = 8, ladders_num = 8;

// Define a new structure
typedef struct
{
    int encounter;
    int with_player;
    int move;
    int space;
}
BOARD;

void generate_board(BOARD board[rows][cols]);
void generate_snakes_ladders(BOARD board[rows][cols]);
void show_players_figures();
void get_players_number(BOARD board[rows][cols]);
void show_players_spaces(int players[]);
void show_scoreboard(BOARD board[rows][cols]);
void scoreboard_cols(BOARD board[rows][cols], int row, int col);
int generate_dice_random();
void game_rounds(int turn, int players[], BOARD board[rows][cols]);
void print_encounter(int index, int move);
bool isWinner(int players[]);
int get_row(int space);
int get_col(int space);

int main(void)
{
    // Seed rand()
    srand(time(NULL));

    // Set numbers of rows and cols for board
    BOARD board[rows][cols] = {0};

    // Generate board
    generate_board(board);

    // to be removed: show_players_figures();
    // to be removed: show_scoreboard(board);

    // Get input for number of players, validate 2 to 4 - get_players_number() save return value in n
    get_players_number(board);
    int players[n];

    for (int i = 1; i <= n; i++)
    {
        players[i] = 0;
    }

    // Keep track of players' turns
    int turn = 1;

    // Keep the game running until a player wins
    while (!isWinner(players))
    {
        // Show all players' current spaces
        // to be removed: show_players_spaces(players);

        // Run one round for every player
        game_rounds(turn, players, board);

        turn++;

        // Check if all players have taken their turns
        if (turn > n)
            turn = 1;
    }
}

// Generate board and include snakes and ladders locations
void generate_board(BOARD board[rows][cols])
{
    int spaces = 1;

    // Call function to randomize snakes and ladders
    generate_snakes_ladders(board);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Set numbers (spaces) onto board
            board[i][j].space = spaces;
            spaces++;

            //printf("%i  ", board[i][j].space);

            /*if (board[i][j].encounter == 0)
                printf("%i  ", board[i][j].space);
            else
                printf("%c  ", board[i][j].encounter);
            if (i == 0)
                printf(" ");*/
        }
        //printf("\n");
    }
}

// Randomize snakes' and ladders' locations
void generate_snakes_ladders(BOARD board[rows][cols])
{
    // Keep track of how many snakes and ladders located
    int snakes = 0, ladders = 0;
    int random = 0, row = 0, col = 0;

    // Set min how many spaces away from located snake/ladder, either backward or forward
    int min = 5;

    for (int num = snakes_num + ladders_num; snakes + ladders < num; )
    {
        // Set max to 95 to allow few more spaces before reaching winning point
        random = rand() % (95 - min + 1) + min;

        // To determine which space to locate snake
        // e.g. space 35: 35 / 10 = 3, 35 % 10 - 1 = 4, row 3 col 4 which is where space 35 is in 2-dimensional board
        row = get_row(random);
        col = get_col(random);

        if (board[row][col].encounter == 0)
        {
            // To place a snake
            if (snakes < snakes_num)
            {
                // Allows to directly index into the array without having to search through array for matching numbers (spaces)
                board[row][col].encounter = snake_index;
                board[row][col].move = rand() % (random - min + 1) + min;
                snakes++;
            }
            // To place a ladder
            else
            {
                board[row][col].encounter = ladder_index;
                board[row][col].move = rand() % ((100 - random) - min + 1) + min;
                ladders++;
            }

        }
    }
}

// Display players' figures
void show_players_figures()
{
    for (int i = 1; i <= n; i++)
    {
        printf("%s P%i\n", p_figures[i], i);
    }
    printf("\n");
}

// Get number of players
void get_players_number(BOARD board[rows][cols])
{
    do
    {
        printf("Number of players (2-4): ");
        scanf("%i", &n);
    }
    while (n < 2 || n > 4);

    // Clear input stream after scanf()
    // As combining getchar() for pressing just Enter to roll dice caused slight bug without clearing input stream
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        continue;

    // Display players' figures and board
    printf("\n");
    show_players_figures();
    show_scoreboard(board);
}

// Show players' current spaces
void show_players_spaces(int players[])
{
    printf(" --------------------- \n");
    for (int i = 1; i <= n; i++)
    {
        printf("|    Player %i at %i    |\n", i, players[i]);
    }
    printf(" --------------------- \n\n");
}

// Display all players' current spaces
void show_scoreboard(BOARD board[rows][cols])
{
    for (int i = rows - 1; i >= 0; i--)
    {
        // To determine whether to display current row in ascending or descending order
        if (i % 2 == 0)
        {
            // Ascending order
            for (int j = 0; j < cols; j++)
            {
                scoreboard_cols(board, i, j);
            }
        }
        else
        {
            // Descending order
            for (int j = cols - 1; j >= 0; j--)
            {
                scoreboard_cols(board, i, j);
            }
        }
        printf("\n");
    }
}

// Display board numbers (spaces) in descending or ascending order
void scoreboard_cols(BOARD board[rows][cols], int row, int col)
{
    if (board[row][col].with_player != 0)
        printf("%7s", p_figures[board[row][col].with_player]);
    else if (board[row][col].encounter != 0)
    {
        if (board[row][col].encounter == snake_index)
            printf("%7s", sl_figures[snake_index]);
        else
            printf("%7s", sl_figures[ladder_index]);
    }
    else
        printf("%5i", board[row][col].space);
}

// Generate a random number between 1 - 6 to simulate a dice
int generate_dice_random()
{
    // Set dice min - max
    int min = 1, max = 6;

    return rand() % max + min;
}

// Run rounds for the game
void game_rounds(int turn, int players[], BOARD board[rows][cols])
{
    int row = 0, col = 0;
    int dice = 0;
    int index = 0, move = 0;
    bool is_encounter = false;

    // Delete current player's figure from current space only if there is current player
    row = get_row(players[turn]);
    col = get_col(players[turn]);

    if (board[row][col].with_player == turn)
        board[row][col].with_player = 0;

    // Generate random number between 1 to 6 - generate_dice_random()
    // Roll dice for current player
    printf("\n%s Player %i's turn. Press Enter key to roll the dice.", p_figures[turn], turn);
    getchar();
    dice = generate_dice_random();

    // Clear previous board
    system("clear");

    // Update player i's space
    players[turn] += dice;

    // Bounce back if player reaches over 100th space
    if (players[turn] > 100)
        players[turn] = 100 - (dice - (100 - (players[turn] - dice)));

    row = get_row(players[turn]);
    col = get_col(players[turn]);

    // Check for snake/ladder
    if (board[row][col].encounter != 0)
    {
        is_encounter = true;
        move = board[row][col].move;

        if (board[row][col].encounter == snake_index)
        {
            index = snake_index;
            players[turn] -= board[row][col].move;
        }
        else
        {
            index = ladder_index;
            players[turn] += board[row][col].move;
        }
    }

    // Place current player's figure onto current space
    row = get_row(players[turn]);
    col = get_col(players[turn]);

    board[row][col].with_player = turn;

    // Display players' figures
    show_players_figures();

    // Display board with players' current spaces
    show_scoreboard(board);

    // Print currently player i
    printf("\nPlayer %i rolled %i\n", turn, dice);

    // Display message if player encounters snake or ladder
    if (is_encounter)
        print_encounter(index, move);
}

// Display message if player encounters snake or ladder
void print_encounter(int index, int move)
{
    if (index == snake_index)
        printf("Encounter a snake! Move backward %i spaces!\n", move);
    else
        printf("Encounter a ladder! Move forward %i spaces!\n", move);
}

// Check if any player reaches 100th space
bool isWinner(int players[])
{
    for (int i = 1; i <= n; i++)
    {
        if (players[i] == 100)
        {
            printf("Player %i won!\n", i);
            return true;
        }
    }

    return false;
}

// To directly index into the space
int get_row(int space)
{
    return space / 10;
}

int get_col(int space)
{
    return space % 10 - 1;
}

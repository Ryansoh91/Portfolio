#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Number of players
int n = 0;

// Set board rows and cols
const int rows = 10, cols = 10;

// Set snakes and ladders numbers
int snakes_num = 8, ladders_num = 8;

typedef struct
{
    char encounter;
    int move;
    int space;
}
BOARD;

void generate_board(BOARD board[rows][cols]);
void generate_snakes_ladders(BOARD board[rows][cols]);
void get_players_number();
void show_players_spaces(int players[]);
int generate_dice_random();
void game_rounds(int dice, int current_player, int players[], BOARD board[rows][cols]);
bool isWinner(int players[]);

int main(void)
{
    // Seed rand()
    srand(time(NULL));

    // Set numbers of rows and cols for board
    BOARD board[rows][cols] = {0};

    // Generate board
    generate_board(board);

    // Get input for number of players, validate 2 to 4 - get_players_number() save return value in n
    get_players_number();
    int players[n];

    for (int i = 0; i < n; i++)
    {
        players[i] = 0;
    }

    // Keep track of players' turns
    int current_player = 0, dice = 0;

    // Keep the game running until a player wins
    while (!isWinner(players))
    {
        // Show all players' current spaces
        show_players_spaces(players);

        // Run one round for every player
        game_rounds(dice, current_player, players, board);
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
        }
    }
}

// Randomize snakes' and ladders' locations
void generate_snakes_ladders(BOARD board[rows][cols])
{
    // Keep track of how many snakes and ladders located
    int snakes = 0, ladders = 0;
    int random = 0, random_row = 0, random_col = 0;

    // Set min how many spaces away from located snake/ladder, either backward or forward
    int min = 5;

    for (int num = snakes_num + ladders_num; snakes + ladders < num; )
    {
        // Set max to 95 to allow few more spaces before reaching winning point
        random = rand() % (95 - min + 1) + min;

        // To determine which space to locate snake
        // e.g. space 35: 35 / 10 = 3, 35 % 10 - 1 = 4, row 3 col 4 which is where space 35 is in 2-dimensional board
        random_row = random / 10;
        random_col = random % 10 - 1;

        if (board[random_row][random_col].encounter == 0)
        {
            if (snakes < snakes_num)
            {
                // Allows to directly index into the array without having to search through array for matching numbers (spaces)
                board[random_row][random_col].encounter = 'S';
                board[random_row][random_col].move = rand() % (random - min + 1) + min;
                snakes++;
            }
            else
            {
                board[random_row][random_col].encounter = 'L';
                board[random_row][random_col].move = rand() % ((100 - random) - min + 1) + min;
                ladders++;
            }

        }
    }
}

// Get number of players
void get_players_number()
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
}

// Show players' current spaces
void show_players_spaces(int players[])
{
    printf(" --------------------- \n");
    for (int i = 0; i < n; i++)
    {
        printf("|    Player %i at %i    |\n", i + 1, players[i]);
    }
    printf(" --------------------- \n\n");
}

// Generate a random number between 1 - 6 to simulate a dice
int generate_dice_random()
{
    // Set dice min - max
    int min = 1, max = 6;

    return rand() % max + min;
}

// Run rounds for the game
void game_rounds(int dice, int current_player, int players[], BOARD board[rows][cols])
{
    int space_row = 0, space_col = 0;

    for (; current_player < n; current_player++)
    {
        // Generate random number between 1 to 6 - generate_dice_random()
        // Roll dice for current player
        printf("Player %i's turn. Press Enter key to roll the dice.", current_player + 1);
        getchar();
        dice = generate_dice_random();

        // Print currently player i
        printf("Player %i rolled %i\n\n", current_player + 1, dice);

        // Update player i's space
        players[current_player] += dice;

        // Bounce back if player reaches over 100th space
        if (players[current_player] > 100)
            players[current_player] = 100 - (dice - (100 - (players[current_player] - dice)));

        space_row = players[current_player] / 10;
        space_col = players[current_player] % 10 - 1;

        // Check for snake/ladder
        if (board[space_row][space_col].encounter != 0)
        {
            if (board[space_row][space_col].encounter == 'S')
            {
                printf("Encounter a snake! Move backward %i spaces!\n\n", board[space_row][space_col].move);
                players[current_player] -= board[space_row][space_col].move;
            }
            else
            {
                printf("Encounter a ladder! Move forward %i spaces!\n\n", board[space_row][space_col].move);
                players[current_player] += board[space_row][space_col].move;
            }
        }
    }

    printf("Round completed. Press Enter key for next round.");
    getchar();
    system("clear");
}

// Check if any player reaches 100th space
bool isWinner(int players[])
{
    for (int i = 0; i < n; i++)
    {
        if (players[i] == 100)
        {
            printf("Player %i won!\n", i + 1);
            return true;
        }
    }

    return false;
}

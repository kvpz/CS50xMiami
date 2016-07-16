/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void find_empty_block(int* empty_i, int* empty_j);
void find_tile_location(int tile, int* tile_i, int* tile_j);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int block_value = d*d - 1; // starting at largest block value on board
    for (int i = 0; i < d; ++i)
    {
        for (int j = 0; j < d; ++j)
        {
            board[i][j] = block_value;
            --block_value;
        }
    }
    board[d-1][d-1] = 0; // bottom right block as "empty space"
    if(!(d%2)) // if there is an odd number of tiles
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}   

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int i = 0; i < d; ++i)
    {
        for (int j = 0; j < d; ++j)
        {
            if(board[i][j] == 0)
            { 
                printf(" _"); 
            }
            else
            {
                if(board[i][j] < 10) printf(" ");
                printf("%d ", board[i][j]);
            }
        }
        printf("\n");
    }
}

void find_empty_block(int* index_i, int* index_j)
{
    for(int i = 0; i < d; ++i)
    {
        for(int j = 0; j < d; ++j)
        {
            if(board[i][j] == 0) // empty block
            {
                *index_i = i;
                *index_j = j;
                return;
            }
        }
    }
}

void find_tile_location(int tile, int* tile_i, int* tile_j)
{
    for(int i = 0; i < d; ++i)
    {
        for(int j = 0; j < d; ++j)
        {
            if(board[i][j] == tile)
            {
                *tile_i = i;
                *tile_j = j;
                return;
            }
        }
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 * Tiles may not be moved diagonally or removed from the board.
 * 
 * Future implementation: It would be more efficient to search for the empty
 * block & tile to move in one iteration of the board. Can be implemented
 * into one function.
 */
bool move(int tile)
{
    int empty_i = 0,
        empty_j = 0,
        tile_i = 0,
        tile_j = 0;
    find_empty_block(&empty_i, &empty_j);
    find_tile_location(tile, &tile_i, &tile_j);
    /* 
        Since tiles can't be moved diagonally, either their row or column
        location must be be the same.
    */
    if(tile_i == empty_i)  // 0 & tile are in the same row
    {
        if(tile_j-1 == empty_j || tile_j+1 == empty_j)
        {
            board[tile_i][empty_j] = tile;
            board[tile_i][tile_j] = 0;
            return true;
        }
    }
    else if(tile_j == empty_j) // 0 & tile are in the same column
    {
        if(tile_i-1 == empty_i || tile_i+1 == empty_i)
        {
            board[empty_i][tile_j] = tile;
            board[tile_i][tile_j] = 0;
            return true;
        }
    }
    // else
        return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int proper_block = 1; // the block value supposed to be in block[i][j]
    for (int i = 0; i < d; ++i)
    {
        for (int j = 0; j < d; ++j)
        {
            if(i == d-1 &&  j == d-1 && board[i][j] == 0)
                return true;
            else if(board[i][j] == proper_block)
                ++proper_block;
            else
                return false;
        }
    }
    return false;
}

/*
    :( 3x3 board, from start try to move tile 2, 4, 5, 6, 7, 8
   \ expected "2" character 1 of line 7, not "0":) 3x3 board, move blank left (tile 1) 
    then up (tile 4), then try to move tiles 1, 2, 6, 8
*/
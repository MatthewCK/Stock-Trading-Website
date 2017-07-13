/**
 * fifteen.c
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

// space location
int space_i;
int space_j;

// prototypes
void clear(void);
void greet(void);
void init(int array[DIM_MAX][DIM_MAX], int d);
void draw(void);
bool move(int array[DIM_MAX][DIM_MAX], int tile, int d);
bool won(int array[DIM_MAX][DIM_MAX], int d);

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
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init(board, d);

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
        if (won(board, d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(board, tile, d))
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
void init(int array[DIM_MAX][DIM_MAX], int d)
{
    // TODO
    space_i = d - 1;
    space_j = d - 1;
    
    int t = d * d - 1; 
    for (int a = 0; a < d; a++)
    {
        for (int b = 0; b < d; b++)
        {
            
            if (a == (d - 1) && b == (d - 3) && d%2 == 0)
            {
                array[a][b] = 1;
                array[a][b + 1] = 2;
                
                break;
            }
            
            
            
            array[a][b] = t;
            t--;
        
            
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    
     for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                if (board[i][j] < 10)
                    printf (" ");
                
                if (board[i][j] != 0)
                    printf("%i", board[i][j]);
                else
                    printf("_");
                    
                if (j < d - 1)
                {
                    printf("|");
                }
            }
            printf("\n");
        }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int array[DIM_MAX][DIM_MAX], int tile, int d)
{
    // is tile on board?
    if (tile > d * d - 1 || tile < 1)
        return false;
    
    // find location of tile
    int tile_i, tile_j;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (array[i][j] == tile)
            {
                tile_i = i;
                tile_j = j;
            }
        }
    }

    // check if space is adjacent to tile
    // is tile below space?
    if (tile_i == space_i - 1 && tile_j == space_j)
        {
            int temp = array[tile_i][tile_j];
            array[tile_i][tile_j] = array[space_i][space_j];
            array[space_i][space_j] = temp;
            
            space_i = tile_i;
            space_j = tile_j;
            
            return true;
        }
        
    // is tile above space?
     else if (tile_i == space_i + 1 && tile_j == space_j)
        {
            int temp = array[tile_i][tile_j];
            array[tile_i][tile_j] = array[space_i][space_j];
            array[space_i][space_j] = temp;
            
            space_i = tile_i;
            space_j = tile_j;
            
            return true;
        }
        
        
    // is tile to left of space?
     else if (tile_i == space_i && tile_j == space_j - 1)
        {
            int temp = array[tile_i][tile_j];
            array[tile_i][tile_j] = array[space_i][space_j];
            array[space_i][space_j] = temp;
            
            space_i = tile_i;
            space_j = tile_j;
            
            return true;
        }
    
    // is tile to right of space?
     else if (tile_i == space_i && tile_j == space_j + 1)
        {
            int temp = array[tile_i][tile_j];
            array[tile_i][tile_j] = array[space_i][space_j];
            array[space_i][space_j] = temp;
            
            space_i = tile_i;
            space_j = tile_j;
            
            return true;
        }
    else
        return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int array[DIM_MAX][DIM_MAX], int d)
{
    int t = 1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (array[i][j] != t)
                return false;
                
            if (t == d * d - 1)
                return true;
            
            t++;
        }
    }
    return false;
}

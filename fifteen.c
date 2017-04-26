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

// blank space coord
int blank_coord[2];

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

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
    //TODO get full count of board
    int size = ( d * d ) - 1;
    
    bool is_odd = size % 2 != 0;
    
    //TODO loop through and assign values
    for( int x = 0; x < d; x++ )
    {
        for( int y = 0; y < d; y++ )
        {
            board[x][y] = size;
            size--;
        }
    }
    
    
    //TODO check if full count is odd, if so swap 1 and 2 [d - 2][d] and [d - 1][d]  
    if( is_odd )
    {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Get the size of the board
    int size = ( d * d ) - 1;
    
    // TODO loop through board and print each value
    for( int x = 0; x < d; x++ )
    {
        for( int y = 0; y < d; y++ )
        {
            // print every number onto the board but leave zero blank
            if( board[x][y] != 0 )
            {
                printf( " %2i", board[x][y] );
            }
            
            else
            {
                blank_coord[0] = x;
                blank_coord[1] = y;
                
                printf( "   " );
            }
            size--;
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // check if tile is in range
    bool in_range = ( tile >= 1 ) && ( tile <= ( d * d ) - 1 ); 
    
    // tile coordinates
    int tile_coord[2];
    
    // get the coordinates of the tile
    if( in_range )
    {
        for( int y = 0; y < d; y++ )
        {
            bool found = false;
            
            for( int x = 0; x < d; x++ )
            {
                if( board[x][y] == tile )
                {
                    tile_coord[0] = x;
                    tile_coord[1] = y;
                    
                    found = true;
                    
                    break;
                }
            }
            
            if( found )
            {
                break;
            }
        }
    }
    
    // check if coords are in the same row
    if( tile_coord[0] == blank_coord[0] )
    {
        
        int right = tile_coord[1] + 1;
        int left = tile_coord[1] - 1;
        
        // check if tile is to the right or left of the blank space
        if( right == blank_coord[1] || left == blank_coord[1] )
        {
            // swap tiles
            board[blank_coord[0]][blank_coord[1]] = tile;
            board[tile_coord[0]][tile_coord[1]] = 0;
            
            // update blank coordinates
            blank_coord[0] = tile_coord[0];
            blank_coord[1] = tile_coord[1];
            
            return true;
        }
    }
    
    // check if the coord are in the same column
    else if( tile_coord[1] == blank_coord[1] )
    {
        
        int above = tile_coord[0] + 1;
        int below = tile_coord[0] - 1;
        
        // check if tile is above or below the blank space
        if( above == blank_coord[0] || below == blank_coord[0] )
        {
            
            // swap tiles
            board[blank_coord[0]][blank_coord[1]] = tile;
            board[tile_coord[0]][tile_coord[1]] = 0;
            
            // update blank coordinates
            blank_coord[0] = tile_coord[0];
            blank_coord[1] = tile_coord[1];
            
            return true;
        }
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int count = 1;
    
    for( int x = 0; x < d; x++ )
    {
        for( int y = 0; y < d; y++ )
        {
            // If the last value on the board is 0 the game has been won
            if( x == d - 1 && y == d - 1 && board[x][y] == 0)
            {
                return true;
            }
            
            // If the coord does not match the count the game has not been won
            else if( board[x][y] != count )
            {
                return false;
            }
            
            count++;
        }
    }
    return false;
}

//------------------------------------------------
// 
// 
// REMARKS   : Implement a module which finds the best move in the game Reversi
//             The best move refers to placing next stone for the most number of gains
//             https://en.wikipedia.org/wiki/Reversi
//------------------------------------------------

//------------------------------------------------------------------------------
// INCLUDE HEADERS
//------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


//------------------------------------------------------------------------------
// CONSTANTS AND TYPES
//------------------------------------------------------------------------------
#define LINE_MAX            512
#define MAX_BOARD_COLUMNS   26
#define MAX_BOARD_ROWS      26
#define MAX_BOARD_TITLE     LINE_MAX

typedef enum
{
    false,
    true
} boolean;

typedef enum
{
    NONE,
    BLACK,
    WHITE
}GameBoardCell;

typedef struct
{
    int nColumns;
    int nRows;
    GameBoardCell player; // who can move next
    GameBoardCell state[MAX_BOARD_ROWS][MAX_BOARD_COLUMNS];
    char title[MAX_BOARD_TITLE];
}GameBoard;


//------------------------------------------------------------------------------
// PROTOTYPES
//------------------------------------------------------------------------------
//--------------------------------------------------
// checkstate
// PURPOSE: Check if the given board is valid
// INPUT PARAMETERS:
//   [board]<IN> Game board to check
// OUTPUT PARAMETERS:
//   [boolean]<OUT> True if valid; otherwise, false
//--------------------------------------------------
boolean checkstate( const GameBoard * board );

//--------------------------------------------------
// computeBestMove
// PURPOSE: Read one board from standard input and compute a player's best move
// INPUT PARAMETERS:
// OUTPUT PARAMETERS:
//   [boolean]<OUT> True if succeeded; otherwise, false
//--------------------------------------------------
boolean computeBestMove( );

//--------------------------------------------------
// canPlayAt
// PURPOSE: Check if current player perhaps be able to play at the given row and column
// INPUT PARAMETERS:
//   [board]<IN> Game board to check
//   [row]<IN> Cell row position to check playability
//   [col]<IN> Cell column position to check playability
// OUTPUT PARAMETERS:
//   [boolean]<OUT> True if possibly playable; otherwise, false
// REMARKS: The function may return true if the cell is possibly playable.
//   To check if cell is indeed playable, after this, the caller must make sure that 
//   there will be at least one piece which will be reversed by playing at this cell.
//--------------------------------------------------
boolean canPlayAt( const GameBoard * board, int row, int col );

//--------------------------------------------------
// numAllReverse
// PURPOSE: Find out all number of reverses by playing at the given position
// INPUT PARAMETERS:
//   [board]<IN> Game board to check
//   [row]<IN> Cell row position
//   [col]<IN> Cell column position
// OUTPUT PARAMETERS:
//   [int]<OUT> Total number of reverses caused by playing at the given position
//--------------------------------------------------
int numAllReverse( const GameBoard * board, int row, int col );

//--------------------------------------------------
// numReverseDirection
// PURPOSE: Find out all number of reverses from the given position toward the given direction
// INPUT PARAMETERS:
//   [board]<IN> Game board to check
//   [row]<IN> Cell row position
//   [col]<IN> Cell column position
//   [dirRow]<IN> Row direction to check reverses [-1, 1]
//                -1 for moving above, 0 for no move, 1 for moving below
//   [dirCol]<IN> Column direction to check reverses [-1, 1]
//                -1 for moving left, 0 for no move, 1 for moving right
// OUTPUT PARAMETERS:
//   [int]<OUT> Total number of reverses caused by playing at the given location toward the direction
//--------------------------------------------------
int numReverseDirection( const GameBoard * board, int row, int col, int dirRow, int dirCol );

//--------------------------------------------------
// readGameBoard
// PURPOSE: Read a game board from standard input
// INPUT PARAMETERS:
//   [board]<OUT> Board to store the information
// OUTPUT PARAMETERS:
//   [boolean]<OUT> True if no error occurred; otherwise, false
// REMARKS: Input must have the board's title on the first line,
//   number of columns, number of rows, and current player ('B' or 'W') on the second (space as a delimiter),
//   the board's row on each line (' ' for NONE, 'B' for BLACK, and 'W' for WHITE)
//   and an empty line at the end.
//--------------------------------------------------
boolean readGameBoard( GameBoard * board );

//--------------------------------------------------
// printBoard
// PURPOSE: Print board in pretty format
// INPUT PARAMETERS:
//   [board]<IN> Game board to print
//--------------------------------------------------
void printBoard( const GameBoard * board );

//--------------------------------------------------
// printBoardColumnName
// PURPOSE: Print board's column head. Support function for printBoard()
// INPUT PARAMETERS:
//   [nColumns]<IN> The number of columns for the currently printing board
//--------------------------------------------------
void printBoardColumnName( int nColumns );

//--------------------------------------------------
// printBoardRowSeparator
// PURPOSE: Print board's row separator. Support function for printBoard()
// INPUT PARAMETERS:
//   [nColumns]<IN> The number of columns for the currently printing board
//--------------------------------------------------
void printBoardRowSeparator( int nColumns );


//------------------------------------------------------------------------------
// VARIABLES
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------
//------------------------------------------------------
// main
// PURPOSE: Application entry point.
// INPUT PARAMETERS:
// OUTPUT PARAMETERS:
//   [int]<OUT> Application exit code.
//------------------------------------------------------
int main( void )
{
    while( computeBestMove( ) )
    {
        printf( "================================================================================\n" );
        printf( "\n" );
    }
    printf( "\n*** END OF PROCESSING ***\n\n" );
    return EXIT_SUCCESS;
}


boolean checkstate( const GameBoard * board )
{
	assert(0 < board->nColumns && board->nColumns < MAX_BOARD_COLUMNS);
	assert(0 < board->nRows && board->nColumns < MAX_BOARD_ROWS);
	assert(board->player == WHITE || board->player == BLACK);

	boolean bState = false;

	if (0 < board->nColumns && board->nColumns < MAX_BOARD_COLUMNS
		&& 0 < board->nRows && board->nColumns < MAX_BOARD_ROWS
		&&board->player == WHITE || board->player == BLACK)
	{
		bState = true;
	}

	return bState;
}


boolean computeBestMove( )
{
    GameBoard board;
    int col, row;
    int bestCol = -1;
    int bestRow = -1;
    int bestReverse = 0;
    int currReverse = 0;
    boolean success = false;

    if( readGameBoard( &board ) )
    {
        printBoard( &board );
        for( row = 0; row < board.nRows; row++ )
        {
            for( col = 0; col < board.nColumns; col++ )
            {
                if( canPlayAt( &board, row, col ) )
                {
                    board.state[row][col] = board.player; // play the piece
                    currReverse = numAllReverse( &board, row, col );
                    board.state[row][col] = NONE; // revert our last change
                    if( currReverse > bestReverse )
                    {
                        bestCol = col;
                        bestRow = row;
                        bestReverse = currReverse;
                    }
                }
            }
        }
        printf( "\n" );
        printf( "The best move for %s is (%c, %d), which will reverse %d opponent piece(s)\n",
            WHITE == board.player ? "WHITE" : "BLACK",
            bestCol + 'a',
            bestRow + 1,
            bestReverse );
        printf( "\n" );
        success = true;
    }
    return success;
}


boolean canPlayAt( const GameBoard * board, int row, int col )
{
    boolean result = false;

	if (checkstate(board))
	{
		assert(0 <= row && row < board->nRows);
		assert(0 <= col && col < board->nColumns);

		//only can play at the spot that demonstrates NONE state
		if (0 <= row && row < board->nRows && 0 <= col && col < board->nColumns && board->state[row][col] == NONE)
		{
			//check if opponent's piece is around given position. 8 directions 
			if ((row - 1 >= 0 && row -1 < board->nRows && col -1 >= 0 && col - 1 < board->nRows && board->state[row - 1][col - 1] != board->player && board->state[row - 1][col - 1] != NONE)
				|| (row - 1 > 0 && row - 1 < board->nRows && board->state[row - 1][col] != board->player && board->state[row - 1][col] != NONE)
				|| (row - 1 >= 0 && row - 1 < board->nRows && col + 1 >= 0 && col + 1 < board->nRows && board->state[row - 1][col + 1] != board->player && board->state[row - 1][col + 1] != NONE)
				|| (col - 1 >= 0 && col - 1 < board->nRows && board->state[row][col - 1] != board->player && board->state[row][col - 1] != NONE)
				|| (col + 1 >= 0 && col + 1 < board->nRows && board->state[row][col + 1] != board->player && board->state[row][col + 1] != NONE)
				|| (row + 1 >= 0 && row + 1 < board->nRows && col - 1 >= 0 && col - 1 < board->nRows && board->state[row + 1][col - 1] != board->player && board->state[row + 1][col - 1] != NONE)
				|| (row + 1 >= 0 && row + 1 < board->nRows && board->state[row + 1][col] != board->player && board->state[row + 1][col] != NONE)
				|| (row + 1 >= 0 && row + 1 < board->nRows && col + 1 >= 0 && col + 1 < board->nRows && board->state[row + 1][col + 1] != board->player && board->state[row + 1][col + 1] != NONE))
			{
				result = true;
			}
		}
	}

	return result;
}


int numAllReverse( const GameBoard * board, int row, int col )
{
    int count = 0;

    if( checkstate( board ) )
    {
        assert( 0 <= row && row < board->nRows );
        assert( 0 <= col && col < board->nColumns );
        assert( board->state[row][col] == board->player );

        if( 0 <= row && row < board->nRows && 0 <= col && col < board->nColumns && board->state[row][col] == board->player )
        {   // we could make following statements to one; however, this is easier to step-in in debugger
            count += numReverseDirection( board, row, col, -1, -1 );
            count += numReverseDirection( board, row, col, -1, 0 );
            count += numReverseDirection( board, row, col, -1, 1 );
            count += numReverseDirection( board, row, col, 0, -1 );
            count += numReverseDirection( board, row, col, 0, 1 );
            count += numReverseDirection( board, row, col, 1, -1 );
            count += numReverseDirection( board, row, col, 1, 0 );
            count += numReverseDirection( board, row, col, 1, 1 );
        }
    }
    return count;
}


int numReverseDirection( const GameBoard * board, int row, int col, int dirRow, int dirCol )
{
	int count = 0;

	if (checkstate(board))
	{
		assert(0 <= row && row < board->nRows);
		assert(0 <= col && col < board->nColumns);
		assert(board->state[row][col] == board->player);
		
		if (0 <= row && row < board->nRows && 0 <= col && col < board->nColumns && board->state[row][col] == board->player)
		{
			//the cell which we are going to check must be inside board.
			while (0 <= row + dirRow && row + dirRow < board->nRows && 0 <= col + dirCol && col + dirCol < board->nColumns)
			{
				if (board->state[row + dirRow][col + dirCol] == board->player)
					return count;
				else if (board->state[row + dirRow][col + dirCol] == NONE)
					break;
				else
					count++;

				row = row + dirRow;
				col = col + dirCol;
			}
		}	
	}
	//outside board or found NONE state position.
	count = 0;
	return count;
}


boolean readGameBoard( GameBoard * board )
{
    char line[LINE_MAX];
    char player = 0;
    int length = 0;
    int col, row;
    boolean success = false;

    assert( NULL != board );
    if( NULL != board )
    {
        memset( board, 0, sizeof( GameBoard ) ); // initialization

        fgets( board->title, MAX_BOARD_TITLE, stdin );
        length = strlen( board->title );
        if( length > 0 && board->title[length - 1] == '\n' )
        {   // remove \n
            board->title[length - 1] = '\0';
        }

        fgets( line, MAX_BOARD_TITLE, stdin );
        sscanf( line, "%d %d %c", &board->nColumns, &board->nRows, &player );
        board->player = 'W' == player ? WHITE : BLACK; // who will play next?

        for( row = 0; NULL != fgets( line, LINE_MAX, stdin ) && row < board->nRows; row++ )
        {   // by putting read line first, we discard the last empty line
            for( col = 0; '\0' != line[col] && col < board->nColumns; col++ )
            {
                switch( line[col] )
                {
                case 'B':
                    board->state[row][col] = BLACK;
                    break;
                case 'W':
                    board->state[row][col] = WHITE;
                    break;
                case ' ':
                default:
                    board->state[row][col] = NONE;
                    break;
                }
            }
        }
        success = row > 0 && row >= board->nRows && checkstate( board );
    }
    return success;
}


void printBoard( const GameBoard * board )
{
    int col, row;

    if( checkstate( board ) )
    {
        printf( "%s\n\n", board->title );
        printBoardColumnName( board->nColumns );
        printBoardRowSeparator( board->nColumns );
        for( row = 0; row < board->nRows; row++ )
        {
            printf( "%2d|", row + 1 );
            for( col = 0; col < board->nColumns; col++ )
            {
                switch( board->state[row][col] )
                {
                case BLACK:
                    printf( "B|" );
                    break;
                case WHITE:
                    printf( "W|" );
                    break;
                default:
                    printf( " |" );
                    break;
                }
            }
            printf( "%-2d\n", row + 1 );
            printBoardRowSeparator( board->nColumns );
        }
        printBoardColumnName( board->nColumns );
    }
}


void printBoardColumnName( int nColumns )
{
    int col;

    assert( nColumns > 0 );
    assert( nColumns < MAX_BOARD_COLUMNS );
    printf( "   " );
    for( col = 0; col < nColumns; col++ )
    {
        printf( "%c ", 'a' + col );
    }
    printf( "  \n" );
}


void printBoardRowSeparator( int nColumns )
{
    int col;

    assert( nColumns > 0 );
    assert( nColumns < MAX_BOARD_COLUMNS );
    printf( "  +" );
    for( col = 0; col < nColumns; col++ )
    {
        printf( "-+" );
    }
    printf( "\n" );
}


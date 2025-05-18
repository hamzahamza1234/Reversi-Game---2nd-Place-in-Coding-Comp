// Program that prints a reversi board of given dimensions,
// Then sets up an intermediate board,prints it, asks user for a move and checks its legality,
// and then prints the final board.
// Author: Hamza Mohammed
// student number: 1007639520

// Uncomment to disable assertions
#define NDEBUG
#include "limits.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "assert.h" // Asserting certain conditions throughout the program makes for easier debugging
#include "string.h"
// Defining certain constants like these makes for easy modification of the program when necessary
#define MAX_DIMENSIONS 26
#define MOVE_SIZE 2
#define TERMINATING_INDEX -1

#define BLACK_COLOR 'B'
#define WHITE_COLOR 'W'
#define EMPTY 'U'
// This function converts a character to an index in the reversi board (For example a is 0, b is 1, etc)
int charToInt(char c)
{
    return c - 'a';
}

int get1DIndex(int row, int col, int n)
{
    return (row * n) + col;
}

int getRowValFromIndex(int idx, int n)
{
    return idx / n;
}

int getColValFromIndex(int idx, int n)
{
    return idx % n;
}

// This function makes sure the move is not out of the boards bounds (avoiding segmentation errors)
bool positionInBounds(int n, int row, int col)
{
    return row >= 0 && row < n && col >= 0 && col < n;
}
// This function checks is the given parameters are of opposing colors
bool isContrastingColor(char c1, char c2)
{
    return c1 != c2 && c1 != EMPTY && c2 != EMPTY;
}

char getContrastingColor(char col)
{
    assert(col == BLACK_COLOR || col == WHITE_COLOR);
    return col == BLACK_COLOR ? WHITE_COLOR : BLACK_COLOR;
}

// This function checks if the move entered by the user is legal (but only checks the given direction)
int calculateContrastInDirection(const char board[][MAX_DIMENSIONS], int n, int row, int col,
                                 char color, int deltarow, int deltacol)
{
    assert(deltarow || deltacol); // since both shoul not be 0
    row += deltarow;
    col += deltacol;
    int numContrastingColorsInBetween = 0;
    while (positionInBounds(n, row, col) && isContrastingColor(board[row][col], color))
    {
        ++numContrastingColorsInBetween;
        row += deltarow;
        col += deltacol;
    }
    return (numContrastingColorsInBetween) * (positionInBounds(n, row, col) && board[row][col] == color);
}

bool checkLegalInDirectionConst(const char board[][MAX_DIMENSIONS], int n, int row, int col,
                                char color, int deltarow, int deltacol)
{
    return calculateContrastInDirection(board, n, row, col, color, deltarow, deltacol) >= 1;
}

// This function calls positionInBounds multiple times to check in all directions if the move is legal
bool checkMoveLegal(const char board[][MAX_DIMENSIONS], int n, int row, int col,
                    char color)
{
    if (!positionInBounds(n, row, col) || board[row][col] != EMPTY)
        return false;
    for (int r = -1; r <= 1; ++r)
    {
        for (int c = -1; c <= 1; ++c)
        {
            if (!r && !c)
                continue;
            if (checkLegalInDirectionConst(board, n, row, col, color, r, c))
                return true;
        }
    }
    return false;
}
// This function flips the pieces in between to the color in the ends of the pieces (in 1 direction)
void flipColorsInDirection(char board[][MAX_DIMENSIONS], int row, int col,
                           char color, int deltaR, int deltaC)
{
    do
    {
        board[row][col] = color;
        row += deltaR;
        col += deltaC;
    } while (board[row][col] != color);
}

// This function calls flipColorsInDirection multiple times to make sure all the captured pieces are flipped
bool flipColors(char board[][MAX_DIMENSIONS], int n, int row, int col,
                char color)
{
    if (!positionInBounds(n, row, col) || board[row][col] != EMPTY)
        return false;
    bool flippedColors = false;
    for (int r = -1; r <= 1; ++r)
    {
        for (int c = -1; c <= 1; ++c)
        {
            if (!r && !c)
                continue;
            if (checkLegalInDirectionConst(board, n, row, col, color, r, c))
            {
                flippedColors = true;
                flipColorsInDirection(board, row, col, color, r, c);
            }
        }
    }
    return flippedColors;
}

int calculateGreedyScore(const char board[][MAX_DIMENSIONS], int n, int row, int col,
                         char color)
{
    int numColorsFlipped = 0;
    for (int r = -1; r <= 1; ++r)
    {
        for (int c = -1; c <= 1; ++c)
        {
            if (!r && !c)
                continue;
            numColorsFlipped += calculateContrastInDirection(board, n, row, col, color, r, c);
        }
    }
    return numColorsFlipped;
}

// This function reads the 3 letter string from the user (contatining the color, row and coloumn)
void readMove(char output[MOVE_SIZE + 1])
{
    scanf("%2s", output);
}
// This function checks if the input enetered by the user is in correct format (color, then row , then coloumn) (its optional)
void checkInputMoveLegality(char output[MOVE_SIZE + 1])
{
    assert(charToInt(output[1]) < MAX_DIMENSIONS);
    assert(charToInt(output[2]) < MAX_DIMENSIONS);
}
// This function calls readMove and checkInputMoveLegality to safetly read the input from the user
void readMoveSafely(char output[MOVE_SIZE + 1])
{
    readMove(output);
    checkInputMoveLegality(output);
}
// This function prints the header of the board (depending on the dimensions)
void printHeader(int n)
{
    printf("  ");
    char c = 'a';
    for (int i = 0; i < n; ++i)
        printf("%c", c + i);
    printf("\n");
}
// This function prints the body of the board (depending on the dimensions)
void printBody(const char board[][MAX_DIMENSIONS], int n)
{
    char c = 'a';
    for (int i = 0; i < n; ++i)
    {
        printf("%c ", c + i);
        for (int j = 0; j < n; ++j)
        {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}
// This function calls printHeader and printBody to print the entire board
void printBoardConst(const char board[][MAX_DIMENSIONS], int n)
{
    printHeader(n);
    printBody(board, n);
}
// This function asks the user and scans the board dimensions from the user
int fetchBoardDimensions()
{
    int n = MAX_DIMENSIONS;
    printf("Enter the board dimension: ");
    scanf("%d", &n);
    assert(n <= MAX_DIMENSIONS && !(n % 2));
    return n;
}

char fetchComputerColor()
{
    char c;
    printf("Computer plays (B/W) : ");
    scanf(" %c", &c);
    assert(c == BLACK_COLOR || c == WHITE_COLOR);
    return c;
}

// This function sets up the initial board with all U's and the WBBW configuration in the centre
void initializeBoard(char board[][MAX_DIMENSIONS], int n)
{
    memset(board, EMPTY, MAX_DIMENSIONS * MAX_DIMENSIONS);
    int center = (n / 2) - 1;
    board[center][center] = WHITE_COLOR;
    board[center][center + 1] = BLACK_COLOR;
    board[center + 1][center] = BLACK_COLOR;
    board[center + 1][center + 1] = WHITE_COLOR;
}

// This function prints all the avalible moves for the specified color (using checkMoveLegal)
void fetchAvailableMoves(int moves[MAX_DIMENSIONS * MAX_DIMENSIONS], const char board[][MAX_DIMENSIONS], int n, char color)
{
    int moveIdx = 0;
    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            if (checkMoveLegal(board, n, row, col, color))
                moves[moveIdx++] = get1DIndex(row, col, n);
        }
    }
    moves[moveIdx] = TERMINATING_INDEX;
}

int getNumAvailableMoves(const char board[][MAX_DIMENSIONS], int n, char color)
{
    int moves[MAX_DIMENSIONS * MAX_DIMENSIONS];
    fetchAvailableMoves(moves, board, n, color);
    int numMoves = 0;
    for (int i = 0; i < MAX_DIMENSIONS * MAX_DIMENSIONS; ++i)
    {
        if (moves[i] == TERMINATING_INDEX)
            break;
        ++numMoves;
    }
    return numMoves;
}

bool checkMovesExists(const char board[][MAX_DIMENSIONS], int n, char color)
{
    return getNumAvailableMoves(board, n, color) > 0;
}

void printNoMoveAvailableFor(char color)
{
    printf("%c player has no valid move.\n", color);
}
void printWinningMsg(char color)
{
    printf("%c player wins.", color);
}

// This function reads the users move and checks if its legal or illegl (if it is legal, it implements it into the board flipping the necessary pieces)
bool humanTurn(char board[][MAX_DIMENSIONS], int n, char humanColor)
{
    if (!checkMovesExists(board, n, humanColor))
    {
        printNoMoveAvailableFor(humanColor);
        return false;
    }
    printf("Enter move for colour %c (RowCol): ", humanColor);
    char move[MOVE_SIZE + 1];
    readMoveSafely(move);
    int row = charToInt(move[0]);
    int col = charToInt(move[1]);
    bool flippedColors = flipColors(board, n, row, col, humanColor);
    if (!flippedColors)
    {
        printf("Invalid move.\n");
        printWinningMsg(getContrastingColor(humanColor));
        exit(0);
    }
    printBoardConst(board, n);
    return flippedColors;
}

void handleComputerLogs(char board[][MAX_DIMENSIONS], char color, int row, int col, int n, bool hasAvailableMove)
{
    if (!hasAvailableMove)
    {
        printNoMoveAvailableFor(color);
        return;
    }
    printf("Computer places %c at %c%c.\n", color, 'a' + row, 'a' + col);
    flipColors(board, n, row, col, color);
    printBoardConst(board, n);
}

bool computerGreedyAlgorithmTurn(char board[][MAX_DIMENSIONS], int n, char color)
{
    int moves[MAX_DIMENSIONS * MAX_DIMENSIONS];
    fetchAvailableMoves(moves, board, n, color);
    int maxScoreSoFar = 0;
    int bestRowSoFar = TERMINATING_INDEX;
    int bestColSoFar = TERMINATING_INDEX;
    for (int i = 0; i < MAX_DIMENSIONS * MAX_DIMENSIONS; ++i)
    {
        int idx = moves[i];
        if (idx == TERMINATING_INDEX)
            break;
        int row = getRowValFromIndex(idx, n);
        int col = getColValFromIndex(idx, n);
        assert(get1DIndex(row, col, n) == idx);
        int greedyScore = calculateGreedyScore(board, n, row, col, color);
        assert(greedyScore > 0);
        if (greedyScore > maxScoreSoFar)
        {
            maxScoreSoFar = greedyScore;
            bestRowSoFar = row;
            bestColSoFar = col;
        }
    }
    bool hasAvailableMove = maxScoreSoFar;
    handleComputerLogs(board, color, bestRowSoFar, bestColSoFar, n, hasAvailableMove);
    return hasAvailableMove;
}

int makeMove(const char board[][MAX_DIMENSIONS], int n, char turn, int *row, int *col);
bool computerCustomAlgorithmTurn(char board[][MAX_DIMENSIONS], int n, char color)
{
    int row = TERMINATING_INDEX;
    int col = TERMINATING_INDEX;
    int moveMade = makeMove(board, n, color, &row, &col);
    handleComputerLogs(board, color, row, col, n, moveMade);
    return true;
}

void printWinner(char board[][MAX_DIMENSIONS], int n)
{
    int blackCol = 0;
    int whiteCol = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            blackCol += board[i][j] == BLACK_COLOR;
            whiteCol += board[i][j] == WHITE_COLOR;
        }
    }
    if (blackCol == whiteCol)
    {
        printf("Draw!\n");
        return;
    }
    char winningCol = blackCol > whiteCol ? BLACK_COLOR : WHITE_COLOR;
    printf("%c player wins.", winningCol);
}
// The main function that simply calls all the other functions in a step by step manner

void playGame()
{
    int n = fetchBoardDimensions();
    char compColor = fetchComputerColor();
    char board[MAX_DIMENSIONS][MAX_DIMENSIONS];
    initializeBoard(board, n);
    printBoardConst(board, n);
    typedef bool (*Game_Ptr)(char board[][MAX_DIMENSIONS], int, char);
    Game_Ptr blackPlayer = compColor == BLACK_COLOR ? &computerGreedyAlgorithmTurn : &humanTurn;
    Game_Ptr whitePlayer = compColor == WHITE_COLOR ? &computerGreedyAlgorithmTurn : &humanTurn;
    bool blackPlays = true;
    bool whitePlays = true;
    while (blackPlays && whitePlays)
    {
        blackPlays = blackPlayer(board, n, BLACK_COLOR);
        whitePlays = whitePlayer(board, n, WHITE_COLOR);
    }
    if (blackPlays)
        blackPlayer(board, n, BLACK_COLOR);
    if (whitePlays)
        whitePlayer(board, n, WHITE_COLOR);
    printWinner(board, n);
}

///////////////////////////////////////////////////

int max(int n1, int n2)
{
    return n1 > n2 ? n1 : n2;
}
int min(int n1, int n2)
{
    return n1 < n2 ? n1 : n2;
}

int positionScore(const char board[][MAX_DIMENSIONS], int n, char color)
{
    if (n != 8)
        return 0;
    static const char boardEncoding[8][8] = {{99, -8, 8, 6, 6, 8, -8, 99},
                                             {-8, -24, -4, -3, -3, -4, -24, -8},
                                             {8, -4, 7, 4, 4, 7, -4, 8},
                                             {6, -3, 4, 0, 0, 4, -3, 6},
                                             {6, -3, 4, 0, 0, 4, -3, 6},
                                             {8, -4, 7, 4, 4, 7, -4, 8},
                                             {-8, -24, -4, -3, -3, -4, -24, -8},
                                             {99, -8, 8, 6, 6, 8, -8, 99}};
    int positionScore = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            positionScore += ((board[i][j] == color) * (boardEncoding[i][j]));
        }
    }
    return positionScore;
}

int playerDiffScore(const char board[][MAX_DIMENSIONS], int n, char color, int winningFactor, int evaporationFactor)
{
    char contrastingCol = getContrastingColor(color);
    int numCol = 0;
    int numContrasting = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            numCol += (board[i][j] == color);
            numContrasting += (board[i][j] == contrastingCol);
        }
    }

    if ((numContrasting + numCol) < ((n * n) / 5))
    {
        return evaporationFactor * (numContrasting - numCol);
    }
    return winningFactor * (numCol - contrastingCol);
}

int mobilityScore(const char board[][MAX_DIMENSIONS], int n, char color)
{
    return getNumAvailableMoves(board, n, color);
}

int calculateScore(const char board[][MAX_DIMENSIONS], int n, char color)
{
    static const int evaporationFactor = 6;
    static const int winningFactor = 10;
    static const int positionFactor = 8;
    static const int mobilityFactor = 10;

    return mobilityFactor * mobilityScore(board, n, color) + playerDiffScore(board, n, color, winningFactor, evaporationFactor) + positionFactor * positionScore(board, n, color);
}

void createBoardCopy(char output[][MAX_DIMENSIONS], const char input[][MAX_DIMENSIONS])
{
    memcpy(output, input, MAX_DIMENSIONS * MAX_DIMENSIONS * sizeof(char));
}

int maxAlgorithm(const char board[][MAX_DIMENSIONS], int n, char color, int depth, int alpha, int bet);

int minAlgorithm(const char board[][MAX_DIMENSIONS], int n, char color, int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return calculateScore(board, n, getContrastingColor(color));
    }
    int moves[MAX_DIMENSIONS * MAX_DIMENSIONS];
    fetchAvailableMoves(moves, board, n, color);
    char contrastCol = getContrastingColor(color);
    int minScoreSoFar = INT_MAX;
    for (int i = 0; i < MAX_DIMENSIONS * MAX_DIMENSIONS; ++i)
    {
        if (moves[i] == TERMINATING_INDEX)
            break;
        char tempBoard[MAX_DIMENSIONS][MAX_DIMENSIONS];
        createBoardCopy(tempBoard, board);
        int row = getRowValFromIndex(moves[i], n);
        int col = getColValFromIndex(moves[i], n);
        flipColors(tempBoard, n, row, col, color);
        minScoreSoFar = min(minScoreSoFar, maxAlgorithm(tempBoard, n, contrastCol, depth - 1, alpha, beta));
        if (minScoreSoFar <= alpha)
            return minScoreSoFar;
        beta = min(minScoreSoFar, beta);
    }
    return minScoreSoFar;
}

int maxAlgorithm(const char board[][MAX_DIMENSIONS], int n, char color, int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return calculateScore(board, n, color);
    }
    int moves[MAX_DIMENSIONS * MAX_DIMENSIONS];
    fetchAvailableMoves(moves, board, n, color);
    int maxScoreSoFar = INT_MIN;
    char contrastCol = getContrastingColor(color);
    for (int i = 0; i < MAX_DIMENSIONS * MAX_DIMENSIONS; ++i)
    {
        if (moves[i] == TERMINATING_INDEX)
            break;
        char tempBoard[MAX_DIMENSIONS][MAX_DIMENSIONS];
        createBoardCopy(tempBoard, board);
        int row = getRowValFromIndex(moves[i], n);
        int col = getColValFromIndex(moves[i], n);
        flipColors(tempBoard, n, row, col, color);
        maxScoreSoFar = max(maxScoreSoFar, minAlgorithm(tempBoard, n, contrastCol, depth - 1, alpha, beta));
        if (maxScoreSoFar >= beta)
            return maxScoreSoFar;
        alpha = max(alpha, maxScoreSoFar);
    }
    return maxScoreSoFar;
}

int makeMove(const char board[][MAX_DIMENSIONS], int n, char turn, int *row, int *col)
{
    assert(turn == BLACK_COLOR || turn == WHITE_COLOR);
    int depth = 5;
    int moves[MAX_DIMENSIONS * MAX_DIMENSIONS];
    fetchAvailableMoves(moves, board, n, turn);
    int maxScoreSoFar = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;
    char contrastCol = getContrastingColor(turn);
    *row = TERMINATING_INDEX;
    *col = TERMINATING_INDEX;
    for (int i = 0; i < MAX_DIMENSIONS * MAX_DIMENSIONS; ++i)
    {
        if (moves[i] == TERMINATING_INDEX)
            break;
        char tempBoard[MAX_DIMENSIONS][MAX_DIMENSIONS];
        createBoardCopy(tempBoard, board);
        int rowFromIdx = getRowValFromIndex(moves[i], n);
        int colFromIdx = getColValFromIndex(moves[i], n);
        flipColors(tempBoard, n, rowFromIdx, colFromIdx, turn);
        int minPlayerScore = minAlgorithm(tempBoard, n, contrastCol, depth, alpha, beta);
        if (minPlayerScore > maxScoreSoFar)
        {
            maxScoreSoFar = minPlayerScore;
            alpha = maxScoreSoFar;
            *row = rowFromIdx;
            *col = colFromIdx;
        }
    }
    if (*row == TERMINATING_INDEX)
    {
        printf("row %d  col %d \n", *row, *col);
        printBoardConst(board, n);
    }
    return maxScoreSoFar;
}

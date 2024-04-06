#include <iostream>
#include <cstdlib> // For  malloc and free
#include <stdio.h> // For printf

#define ROWS 6
#define COLUMNS 7
#define connect 4
#define player1 'X'
#define player2 'O'
#define emptySpace ' '
#define rowNotFound -1
#define invalidRow 4
#define fullColumn 2
#define noError 3
#define twoPlayers 1
#define noConnect 0

// Function prototypes
void createBoard(char ***boardPtr);
void displayHeader(int columns);
void displayBoard(char **board);
void clearBoard(char **board);
int findFreeRow(int column, char **board);
int placeAToken(char player, int column, char **board);


// BOARD FUNCTIONALITIES (CREATION-DISPLAY)
void createBoard(char ***boardPtr)
{
    char **board = (char **)malloc(ROWS * sizeof(char *)); // 6 rows
    for (int i = 0; i < ROWS; ++i)
    {
        board[i] = (char *)malloc(COLUMNS * sizeof(char)); // 7 columns for each row
        for (int j = 0; j < COLUMNS; ++j)
        {
            board[i][j] = emptySpace;
        }
    }

    *boardPtr = board;
}

void displayHeader(int columns)
{
    printf("\n");
    for (int i = 1; i <= columns; ++i)
    {
        printf("|%i", i);
        if (i == columns)
        {
            printf("|");
        }
    }
}

void displayBoard(char **board)
{
    displayHeader(COLUMNS);
    printf("\n");
    for (int i = 0; i < ROWS; i++)
    {
        printf("|");
        for (int j = 0; j < COLUMNS; j++)
        {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
}

void clearBoard(char **board)
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            board[i][j] = emptySpace;
        }
    }
}

int findFreeRow(int column, char **board)
{
    for (int i = ROWS - 1; i >= 0; i--)
    {
        if (board[i][column] == emptySpace)
        {
            return i;
        }
    }
    return rowNotFound;
}

int placeAToken(char player, int column, char **board)
{
    if (column < 0 || column >= COLUMNS)
    {
        return invalidRow;
    }
    int row = findFreeRow(column, board);
    if (row == rowNotFound)
    {
        return fullColumn;
    }
    board[row][column] = player;
    return noError;
}


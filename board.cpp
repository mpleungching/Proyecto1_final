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
int requestAPlayersColumn();
char getOpponent(char player);
void game(int gameMode);
bool checkWin(char player, char **board);
bool checkHorizontal(char player, char **board);
bool checkVertical(char player, char **board);
bool checkDiagonal(char player, char **board);


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

int requestAPlayersColumn()
{
    int column;
    printf("Escribe la columna en donde colocar la pieza (1-7): ");
    scanf("%d", &column);
    column--;
    return column;
}

char getOpponent(char player)
{
    if (player == player1)
    {
        return player2;
    }
    else
    {
        return player1;
    }
}


void game(int gameMode)
{
    char **board;
    createBoard(&board);
    displayBoard(board);          // No es necesario pasar la dirección de board porque estas funciones no necesitan modificar board en sí;
    clearBoard(board);            // solo necesitan acceder a la memoria a la que apunta.
    char currentPlayer = player1; // inician las x
    int moves = 0;

    while (moves < ROWS * COLUMNS)
    {
        int column = 0;
        displayBoard(board);

        if (gameMode == twoPlayers)
        {
            column = requestAPlayersColumn();
            int result = placeAToken(currentPlayer, column, board);
        
            if (result == invalidRow)
            {
                printf("Número de columna inválido. Por favor, elige un número entre 1 y 7.\n");
                continue; // Continuar con el siguiente turno sin cambiar de jugador
            }
            else if (result == fullColumn)
            {
                printf("¡La columna está llena! Por favor, elige otra columna.\n");
                continue; // Continuar con el siguiente turno sin cambiar de jugador
            }
             // Mostrar el tablero actualizado después de colocar la marca
            displayBoard(board);

            // check if the current player has won
            bool win = checkWin(currentPlayer, board);
            if (win)
            {
                printf("Player %c wins!\n", currentPlayer);
                break;
            }
        }
        currentPlayer = getOpponent(currentPlayer);
        moves++; 
    }

    // Free memory allocated for the board
    for (int i = 0; i < ROWS; ++i)
    {
        free(board[i]);
    }
    free(board);
}

// CHECK WIN - TOKEN COUNT
bool checkHorizontal(char player, char **board)
{
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col <= COLUMNS - connect; ++col)
        {
            int counter = 0;
            for (int i = 0; i < connect; ++i)
            {
                if (board[row][col + i] == player)
                {
                    counter++;
                    if (counter == connect)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool checkVertical(char player, char **board)
{
    for (int col = 0; col < COLUMNS; ++col)
    {
        for (int row = 0; row <= ROWS - connect; ++row)
        {
            int counter = 0;
            for (int i = 0; i < connect; ++i)
            {
                if (board[row + i][col] == player)
                {
                    counter++;
                    if (counter == connect)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool checkDiagonal(char player, char **board)
{
    // Check diagonals (top-left to bottom-right)
    for (int row = 0; row <= ROWS - connect; ++row)
    {
        for (int col = 0; col <= COLUMNS - connect; ++col)
        {
            int counter = 0;
            for (int i = 0; i < connect; ++i)
            {
                if (board[row + i][col + i] == player)
                {
                    counter++;
                    if (counter == connect)
                    {
                        return true;
                    }
                }
            }
        }
    }

    // Check diagonals (top-right to bottom-left)
    for (int row = 0; row <= ROWS - connect; ++row)
    {
        for (int col = COLUMNS - 1; col >= connect - 1; --col)
        {
            int counter = 0;
            for (int i = 0; i < connect; ++i)
            {
                if (board[row + i][col - i] == player)
                {
                    counter++;
                    if (counter == connect)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool checkWin(char player, char **board)
{
    return checkHorizontal(player, board) || checkVertical(player, board) || checkDiagonal(player, board);
}

int main()
{
    game(twoPlayers);
    return 0;
}


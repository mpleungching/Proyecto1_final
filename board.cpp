/*  Universidad de Costa Rica
    Programación II 
    Tarea programada - 4 en línea con matrices infinitas
    Integrantes: 
    Jimena Rivera Álvarez C36561
    María Paula Leung Ching C34258
    Nazareth
 */

#include <iostream>
#include <cstdlib> // malloc and free
#include <stdio.h> // printf

#define connect 4
#define player1 'X'
#define player2 'O'
#define emptySpace ' '
#define rowNotFound -1
#define invalidRow -2
#define fullColumn -3
#define noError 1
#define noConnect 0

// function prototypes
void createBoard(char ***boardPtr, int ROWS, int COLUMNS);
void displayBoard(char **board, int ROWS, int COLUMNS);
int findFreeRow(int column, char **board, int ROWS);
int placeAToken(char player, int column, char **board, int ROWS);
int requestAPlayersColumn(int COLUMNS);
char getOpponent(char player);
void game(int gameMode);
void multiplayer();
void singlePlayer();
bool checkWin(char player, char **board);
int countVerticalTokens(char player, int startRow, int column, char **board);
int countHorizontalTokens(char player, int row, int startColumn, char **board);
int countDiagonalTokens1(char player, int startRow, int column, char **board);
int countDiagonalTokens2(char player, int startRow, int column, char **board);
bool checkHorizontal(char player, char **board);
bool checkVertical(char player, char **board);
bool checkDiagonal(char player, char **board);
int chooseRandomColumn(char **board, char player, int COLUMNS);
int checkWinAI(char **board, char player);
int blockOpponent(char **board, char player);
void expandBoard(char ***board, int *ROWS, int *COLUMNS);
int ROWS, COLUMNS;

// board funcionalities, create and display
void createBoard(char ***boardPtr, int ROWS, int COLUMNS)
{
    char **board = (char **)malloc(ROWS * sizeof(char *));
    for (int i = 0; i < ROWS; ++i)
    {
        board[i] = (char *)malloc(COLUMNS * sizeof(char));
        for (int j = 0; j < COLUMNS; ++j)
        {
            board[i][j] = emptySpace;
        }
    }
    *boardPtr = board;
}

void displayBoard(char **board, int ROWS, int COLUMNS)
{
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

// this method searches for the first available row in a specific column of the board
int findFreeRow(int column, char **board, int ROWS)
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

// this method attempts to place a token in a specific column of the board,
// checking if the column is valid and if there is available space in that column
int placeAToken(char player, int column, char **board, int ROWS)
{
    if (column < 0 || column >= COLUMNS)
    {
        return invalidRow;
    }
    int row = findFreeRow(column, board, ROWS);
    if (row == rowNotFound)
    {
        return fullColumn;
    }
    board[row][column] = player;
    return noError;
}

int requestAPlayersColumn(int COLUMNS)
{
    int column = 0;
    printf("Enter the column to place the token (1-%d) or 'h' to expand height, 'r' to add columns to the right or 'l' to add columns to the left: ", COLUMNS);
    scanf("%i", &column);
    return column - 1;
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
    if (gameMode == 1) // game mode 1: multiplayer
    {
        multiplayer();
    }
    else if (gameMode == 2) // game mode 2: singleplayer
    {
        singlePlayer();
    }
}

void multiplayer()
{
    char **board;
    createBoard(&board, ROWS, COLUMNS);
    char currentPlayer = player1;

    while (true)
    {
        int column;
        displayBoard(board, ROWS, COLUMNS);
        printf("Player %c's turn.\n", currentPlayer);
        column = requestAPlayersColumn(COLUMNS);

        if (column == -1) // player chose to add columns
        {
            expandBoard(&board, &ROWS, &COLUMNS);
            continue;
        }

        else if (column < 0 || column >= COLUMNS || findFreeRow(column, board, ROWS) == rowNotFound)
        {
            printf("Invalid or full column. Please choose another column, or expand the board.");
            continue;
        }

        placeAToken(currentPlayer, column, board, ROWS);

        bool win = checkWin(currentPlayer, board);
        if (win)
        {
            displayBoard(board, ROWS, COLUMNS);
            printf("Player %c wins!\n", currentPlayer);
            break;
        }
        currentPlayer = getOpponent(currentPlayer);
    }
    for (int i = 0; i < ROWS; ++i) // free memory
    {
        free(board[i]);
    }
    free(board);
}

void singlePlayer()
{
    char **board;
    createBoard(&board, ROWS, COLUMNS);
    char currentPlayer = player1;

    while (true)
    {
        int column;
        if (currentPlayer == player1) // player's turn
        {
            displayBoard(board, ROWS, COLUMNS);
            printf("Player %c's turn.\n", currentPlayer);
            column = requestAPlayersColumn(COLUMNS);

            if (column == -1) // player chose to add columns
            {
                expandBoard(&board, &ROWS, &COLUMNS);
                continue;
            }

            else if (column < 0 || column >= COLUMNS || findFreeRow(column, board, ROWS) == rowNotFound)
            {
                printf("Invalid or full column. Please choose a number between 1 and %d or enter 'l' or 'r' to expand the board: \n", COLUMNS);
                continue;
            }

            placeAToken(currentPlayer, column, board, ROWS);
            currentPlayer = getOpponent(currentPlayer);
        }
        else // AI's turn
        {
            displayBoard(board, ROWS, COLUMNS);
            printf("AI's turn: ");

            int winningCol = checkWinAI(board, currentPlayer);
            int blockCol = blockOpponent(board, currentPlayer);
            int randomCol = chooseRandomColumn(board, currentPlayer, COLUMNS);

            if (winningCol != -1) // 1: try to win
            {
                placeAToken(currentPlayer, winningCol, board, ROWS);
            }
            else if (blockCol != -1) // 2: block opponents win
            {
                placeAToken(currentPlayer, blockCol, board, ROWS);
            }
            else // choose random column
            {
                placeAToken(currentPlayer, randomCol, board, ROWS);
            }

            bool win = checkWin(currentPlayer, board);
            if (win)
            {
                displayBoard(board, ROWS, COLUMNS);
                printf("AI wins!\n");
                break;
            }
            currentPlayer = getOpponent(currentPlayer);
        }
    }

    for (int i = 0; i < ROWS; ++i) // free memory
    {
        free(board[i]);
    }
    free(board);
}

// check win - token count
int countHorizontalTokens(char player, int row, int startColumn, char **board)
{
    int count = 0;
    for (int col = startColumn; col < COLUMNS; ++col)
    {
        if (board[row][col] == player)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    return count;
}

int countVerticalTokens(char player, int startRow, int column, char **board)
{
    int count = 0;
    for (int row = startRow; row < ROWS; ++row)
    {
        if (board[row][column] == player)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    return count;
}

int countDiagonalTokens1(char player, int startRow, int column, char **board)
{
    int count = 0;
    for (int row = startRow, col = column; row < ROWS && col < COLUMNS; ++row, ++col)
    {
        if (board[row][col] == player)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    return count;
}

int countDiagonalTokens2(char player, int startRow, int column, char **board)
{
    int count = 0;
    for (int row = startRow, col = column; row < ROWS && col >= 0; ++row, --col)
    {
        if (board[row][col] == player)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    return count;
}

// check connect 4
bool checkHorizontal(char player, char **board)
{
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col <= COLUMNS - connect; ++col)
        {
            int count = countHorizontalTokens(player, row, col, board);
            if (count == connect)
            {
                return true;
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
            int count = countVerticalTokens(player, row, col, board);
            if (count == connect)
            {
                return true;
            }
        }
    }
    return false;
}

bool checkDiagonal(char player, char **board)
{ // check diagonals (top-left to bottom-right)
    for (int row = 0; row <= ROWS - connect; ++row)
    {
        for (int col = 0; col <= COLUMNS - connect; ++col)
        {
            int count = countDiagonalTokens1(player, row, col, board);
            if (count == connect)
            {
                return true;
            }
        }
    }

    // check diagonals (top-right to bottom-left)
    for (int row = 0; row <= ROWS - connect; ++row)
    {
        for (int col = COLUMNS - 1; col >= connect - 1; --col)
        {
            int count = countDiagonalTokens2(player, row, col, board);
            if (count == connect)
            {
                return true;
            }
        }
    }
    return false;
}

bool checkWin(char player, char **board)
{
    return checkHorizontal(player, board) || checkVertical(player, board) || checkDiagonal(player, board);
}

// AI vs player (logic)
int checkWinAI(char **board, char player)
{
    for (int col = 0; col < COLUMNS; ++col)
    {
        int freeRow = findFreeRow(col, board, ROWS);
        if (freeRow != rowNotFound)
        {
            board[freeRow][col] = player;
            if (checkWin(player, board))
            {
                board[freeRow][col] = emptySpace; // reset
                return col;
            }
            else
            {
                board[freeRow][col] = emptySpace; // reset
            }
        }
    }
    return rowNotFound;
}

int blockOpponent(char **board, char player)
{
    char opponent = getOpponent(player);
    for (int col = 0; col < COLUMNS; ++col)
    {
        int freeRow = findFreeRow(col, board, ROWS);
        if (freeRow != rowNotFound)
        {
            board[freeRow][col] = opponent;
            if (checkWin(opponent, board))
            {
                board[freeRow][col] = emptySpace; // block
                return col;
            }
            else
            {
                board[freeRow][col] = emptySpace; // reset
            }
        }
    }
    return rowNotFound;
}

int chooseRandomColumn(char **board, char player, int COLUMNS)
{
    int col = rand() % COLUMNS;
    while (findFreeRow(col, board, ROWS) == rowNotFound)
    {
        col = rand() % COLUMNS;
    }
    return col;
}

void expandBoard(char ***board, int *ROWS, int *COLUMNS)
{
    char whereToAdd;
    const int numColumnsToAdd = 7;
    const int numRowsToAdd = 6;

    scanf(" %c", &whereToAdd);

    if (whereToAdd != 'h' && whereToAdd != 'l' && whereToAdd != 'r')
    {
        printf("Invalid option. Please enter 'h', 'l', or 'r'.\n");
        return;
    }

    if (whereToAdd == 'h') // expand height
    {
        char **newBoard = (char **)malloc((*ROWS + numRowsToAdd) * sizeof(char *));
        for (int i = 0; i < *ROWS + numRowsToAdd; ++i)
        {
            newBoard[i] = (char *)malloc((*COLUMNS) * sizeof(char));
            if (i < numRowsToAdd)
            {
                for (int j = 0; j < *COLUMNS; ++j)
                {
                    newBoard[i][j] = emptySpace;
                }
            }
            else
            {
                // copy the existing rows to new positions
                for (int j = 0; j < *COLUMNS; ++j)
                {
                    newBoard[i][j] = (*board)[i - numRowsToAdd][j];
                }
            }
        }
        for (int i = 0; i < *ROWS; ++i)
        {
            free((*board)[i]);
        }
        free(*board);

        *board = newBoard;
        *ROWS += numRowsToAdd;
    }
    else if (whereToAdd == 'l') // expand to the left
    {
        char **newBoard = (char **)malloc((*ROWS) * sizeof(char *));
        for (int i = 0; i < *ROWS; ++i)
        {
            newBoard[i] = (char *)malloc((*COLUMNS + numColumnsToAdd) * sizeof(char));
            for (int j = 0; j < *COLUMNS + numColumnsToAdd; ++j)
            {
                if (j < numColumnsToAdd)
                {
                    newBoard[i][j] = emptySpace;
                }
                else
                {
                    newBoard[i][j] = (*board)[i][j - numColumnsToAdd];
                }
            }
        }

        for (int i = 0; i < *ROWS; ++i)
        {
            free((*board)[i]);
        }
        free(*board);

        *board = newBoard;
        *COLUMNS += numColumnsToAdd;
    }
    else if (whereToAdd == 'r') // expand to the right
    {
        char **newBoard = (char **)malloc((*ROWS) * sizeof(char *));
        for (int i = 0; i < *ROWS; ++i)
        {
            newBoard[i] = (char *)malloc((*COLUMNS + numColumnsToAdd) * sizeof(char));
            for (int j = 0; j < *COLUMNS; ++j)
            {
                newBoard[i][j] = (*board)[i][j];
            }
            for (int j = *COLUMNS; j < *COLUMNS + numColumnsToAdd; ++j)
            {
                newBoard[i][j] = emptySpace;
            }
        }

        for (int i = 0; i < *ROWS; ++i)
        {
            free((*board)[i]);
        }
        free(*board);

        *board = newBoard;
        *COLUMNS += numColumnsToAdd;
    }

    printf("Here is your expanded board:\n");
}

int main()
{
    ROWS = 6;
    COLUMNS = 7;
    int gameMode;
    printf("Let's play Connect 4!\n"
           "Select an option: \n"
           "1 -> 2 players\n"
           "2 -> AI vs player\n"
           "I want to play in mode: ");
    scanf("%d", &gameMode);
    if (gameMode <= 0 || gameMode > 3)
    {
        printf("Invalid game mode. Exiting...\n");
        return 0;
    }
    game(gameMode);
    return 0;
}
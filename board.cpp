#include <iostream>
#include <cstdlib> // For  malloc and free
#include <stdio.h> // For printf

#define ROWS 6
#define COLUMNS 7
#define matrixSize sizeof(char) * ROWS *COLUMNS
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
#define AIvsPlayer 2

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
int countVerticalTokens(char player, int startRow, int column, char **board);
int countHorizontalTokens(char player, int row, int startColumn, char **board);
int countDiagonalTokens1(char player, int startRow, int column, char **board);
int countDiagonalTokens2(char player, int startRow, int column, char **board);
bool checkHorizontal(char player, char **board);
bool checkVertical(char player, char **board);
bool checkDiagonal(char player, char **board);
int chooseRandomColumn(char **board, char player);
int randomNum(int min, int max);
int calculateBestMove(char **board, char player);
void checkWinAI(char **board, char player);
void blockOpponent(char **board, char player);

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
    displayBoard(board);          
    clearBoard(board);            
    char currentPlayer = player1; 
    int moves = 0;

    while (moves < ROWS * COLUMNS)
    {
        int column = 0;
        if (gameMode == twoPlayers) // game mode 1
        {
            column = requestAPlayersColumn();
            int result = placeAToken(currentPlayer, column, board);

            if (result == invalidRow)
            {
                printf("Número de columna inválido. Por favor, elige un número entre 1 y 7.\n");
                continue; 
            }
            else if (result == fullColumn)
            {
                printf("¡La columna está llena! Por favor, elige otra columna.\n");
                continue; 
            }
            displayBoard(board); 

            // check if the current player has won
            bool win = checkWin(currentPlayer, board);
            if (win)
            {
                printf("Player %c wins!\n", currentPlayer);
                break;
            }
        }

        else if (gameMode == AIvsPlayer) // game mode 2
        {
            if (currentPlayer == player1) // player's turn
            {
                column = requestAPlayersColumn();
                int result = placeAToken(currentPlayer, column, board);

                if (result == invalidRow)
                {
                    printf("Número de columna inválido. Por favor, elige un número entre 1 y 7.\n");
                    continue; 
                }
                else if (result == fullColumn)
                {
                    printf("¡La columna está llena! Por favor, elige otra columna.\n");
                    continue; 
                }
                displayBoard(board);

                // check if the current player has won
                bool win = checkWin(currentPlayer, board);
                if (win)
                {
                    printf("Player %c wins!\n", currentPlayer);
                    break;
                }
            }
            else // AI's turn
            {
                printf("Turno de la IA:\n");
                column = calculateBestMove(board, currentPlayer);
                int result = placeAToken(currentPlayer, column, board);

                displayBoard(board);

                // check if the current player has won
                bool win = checkWin(currentPlayer, board);
                if (win)
                {
                    printf("IA wins!\n");
                    break;
                }
            }
        }
        currentPlayer = getOpponent(currentPlayer);
        moves++;
    }
    // free memory
    for (int i = 0; i < ROWS; ++i)
    {
        free(board[i]);
    }
    free(board);
}

// CHECK WIN - TOKEN COUNT
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

// CHECK CONNECT 4
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
{
    // Check diagonals (top-left to bottom-right)
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

    // Check diagonals (top-right to bottom-left)
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

// AI vs player
void duplicateBoard(char **board, char **duplicate)
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLUMNS; ++j)
        {
            duplicate[i][j] = board[i][j];
        }
    }
}

void checkWinAI(char **board, char player) // 1. Check if AI can win in the next move
{
    char **tempBoard;
    for (int col = 0; col < COLUMNS; ++col)
    {
        createBoard(&tempBoard);
        duplicateBoard(board, tempBoard);
        int result = placeAToken(player, col, tempBoard);
        if (result == noError && checkWin(player, tempBoard))
        {
            placeAToken(player, col, board);
        }
        free(tempBoard);
    }
}

void blockOpponent(char **board, char player)
{
    char **tempBoard;
    char opponent = getOpponent(player);
    for (int col = 0; col < COLUMNS; ++col)
    {
        createBoard(&tempBoard);
        duplicateBoard(board, tempBoard);
        int result = placeAToken(opponent, col, tempBoard); // iterate through every column placing the opponents token
        if (result == noError && checkWin(opponent, tempBoard))
        {
            placeAToken(player, col, board);
        }
        free(tempBoard);
    }
}

int calculateHighScoreRow(char **board, char player, int *score, int *index)
{
    int highestScore = 0;
    int columnWithHighestScore = -1;
    char **tempBoard;

    for (int column = 0; column < COLUMNS; ++column)
    {
        createBoard(&tempBoard);
        duplicateBoard(board, tempBoard);
        int result = placeAToken(player, column, tempBoard);
        if (result == noError)
        {
            int rowOfLastToken = findFreeRow(column, tempBoard);
            if (rowOfLastToken != rowNotFound)
            {
                int count = 0;
                // Count tokens in different directions (horizontal, vertical, diagonal) and update the highest score
                count += countHorizontalTokens(player, rowOfLastToken, column, tempBoard);
                count += countVerticalTokens(player, rowOfLastToken, column, tempBoard);
                count += countDiagonalTokens1(player, rowOfLastToken, column, tempBoard);
                count += countDiagonalTokens2(player, rowOfLastToken, column, tempBoard);

                // Update the highest score and column index if the current count is higher
                if (count > highestScore)
                {
                    highestScore = count;
                    columnWithHighestScore = column;
                }
            }
        }
        // free memory
        for (int i = 0; i < ROWS; ++i)
        {
            free(tempBoard[i]);
        }
        free(tempBoard);
    }
    *score = highestScore;
    *index = columnWithHighestScore;
    return columnWithHighestScore;
}

int calculateBestMove(char **board, char player)
{
    checkWinAI(board, player);    // Check if AI can win in the next move
    blockOpponent(board, player); // Block the opponent if they can win in the next move
    int bestMove = -1;
    int highestScore = -1;

    for (int col = 0; col < COLUMNS; ++col)
    {
        char **tempBoard;
        createBoard(&tempBoard);
        duplicateBoard(board, tempBoard);
        int result = placeAToken(player, col, tempBoard);
        if (result == noError)
        {
            int score;
            int index;
            calculateHighScoreRow(tempBoard, player, &score, &index);
            if (score > highestScore)
            {
                highestScore = score;
                bestMove = col;
            }
        }
        for (int i = 0; i < ROWS; ++i)
        {
            free(tempBoard[i]);
        }
        free(tempBoard);
    }

    // If no winning or blocking moves, choose center or random
    if (bestMove == -1)
    {
        int centerColumn = COLUMNS / 2;
        if (board[0][centerColumn] == emptySpace)
        {
            return centerColumn;
        }
        else
        {
            return chooseRandomColumn(board, player);
        }
    }

    return bestMove;
}

int chooseRandomColumn(char **board, char player)
{
    int col = randomNum(0, COLUMNS - 1);
    int resultado = placeAToken(player, col, board);
    if (resultado == noError)
    {
        return col;
    }
    return -1;
}

int randomNum(int min, int max)
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int main()
{
    int gameMode;
    printf("1 -> 2 players\n"
           "2 -> AI vs player\n"
           "Select an option: ");
    scanf("%d", &gameMode);
    if (gameMode <= 0 || gameMode > 3)
    {
        return 0;
    }
    game(gameMode);
    return 0;
}
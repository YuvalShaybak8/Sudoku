#ifndef __POINTERS_H
#define __POINTERS_H

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$       Project : Sudoku              $$
$$       By:                           $$
$$   Rannan Harpak  - id : 315816983   $$
$$   Yuval Saybak   - id : 313524837   $$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>


#define MAX_SIZE 9
#define FINISH_SUCCESS 1
#define FINISH_FAILURE -1
#define NOT_FINISH 0
#define FILLED 2
#define FAIL -1
#define EMPTY_CELL -1

//Q1
typedef struct _Array
{
    short* arr;
    unsigned short size;
} Array;

//--function declaration--//
void startGame();

/*Q1*/
int getMiddleSquare(int current);
void copyArr(short* dest, short* src);
void checkMemoryAllocation(void* arr);
void sudokoPrintBoard(short sudokuBoard[][9]);
Array*** PossibleDigits(short sudokuBoard[][9]);
void updatePosArr(short* possibledig, int* possibleSize, short valuetodelete);
void checkCol(short sudokuBoard[][9], int j, short* possibledig, int* possibleSize);
void checkRow(short sudokuBoard[][9], int i, short* possibledig, int* possibleSize);
void checkSquare(short sudokuBoard[][9], int i, int j, short* possibledig, int* possibleSize);

/*Q2*/
void freeArray(Array* posArray);
bool updateRelevantCells(Array*** pos, short value, int row, int col);
void updateCol(Array*** pos, short value, int row, int col, bool* res);
void updateRow(Array*** pos, short value, int row, int col, bool* res);
void removeValueFromArray(Array* pos, int value, bool* duplicateExist);
void updateSquare(Array*** pos, short value, int row, int col, bool* res);
int OneStage(short board[][9], Array*** possibilities, int* x, int* y, bool* keepLooping);

/*Q3*/
bool isBoardFull(Array*** possibilities);
void copyUpdateArr(short* dest, short* src, int arrSize);
void copyArrExcepttheNum(short* dest, short* src, int arrSize, int value);

#endif
#include "Pointers.h"
#include "Pointers.h"

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$       Project : Sudoku              $$
$$       By:                           $$
$$   Rannan Harpak  - id : 315816983   $$
$$   Yuval Saybak   - id : 313524837   $$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

/*Look for possible options to put in each 
cell and cells and create an array for each
of them and put the options of the same cell in them.*/
Array*** PossibleDigits(short sudokuBoard[][9])
{
	Array*** possibleDig = (Array***)malloc(sizeof(Array**) * MAX_SIZE);
	checkMemoryAllocation(possibleDig);

	for (int i = 0; i < MAX_SIZE; i++)
	{
		possibleDig[i] = (Array**)malloc(sizeof(Array*) * MAX_SIZE);
		checkMemoryAllocation(possibleDig[i]);

		for (int j = 0; j < MAX_SIZE; j++)
		{
			// if cell is already have a number.
			if (sudokuBoard[i][j] != -1)
			{
				possibleDig[i][j] = NULL;
				continue;
			}

			int possibleSize = 9;
			short relevantOptions[] = { 1,2,3,4,5,6,7,8,9 };
			checkSquare(sudokuBoard, i, j, relevantOptions, &possibleSize);/*check square*/
			checkRow(sudokuBoard, i, relevantOptions, &possibleSize);/*check row*/
			checkCol(sudokuBoard, j, relevantOptions, &possibleSize);/*check col*/

			if (possibleSize > 0)
			{

				possibleDig[i][j] = (Array*)malloc(sizeof(Array));
				checkMemoryAllocation(possibleDig[i][j]);

				possibleDig[i][j]->arr = (short*)malloc(sizeof(short) * (short)possibleSize);
				checkMemoryAllocation(possibleDig[i][j]->arr);

				possibleDig[i][j]->size = possibleSize;
				copyArr(possibleDig[i][j]->arr, relevantOptions);

			}
			else
				possibleDig[i][j] = NULL;
		}
	}
	return possibleDig;
}

/*copy arr from src to dest*/
void copyArr(short* dest, short* src)
{
	int dest_index = 0;

	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (src[i] != -1)
		{
			dest[dest_index] = src[i];
			dest_index++;
		}
	}
}

/*check each Square and update the tmp array with the possible numbers remaining*/
void checkSquare(short sudokuBoard[][9], int i, int j, short* relevantOptions, int* possibleSize)
{
	int currentCenterRow = getMiddleSquare(i);
	int currentCenterCol = getMiddleSquare(j);

	for (int r = currentCenterRow - 1; r <= currentCenterRow + 1; r++)
	{
		for (int c = currentCenterCol - 1; c <= currentCenterCol + 1; c++)
		{
			if (sudokuBoard[r][c] != -1)
			{
				updatePosArr(relevantOptions, possibleSize, sudokuBoard[r][c]);
			}
		}
	}
}

/*check each Row and update the tmp array with the possible numbers remaining*/
void checkRow(short sudokuBoard[][9], int i, short* relevantOptions, int* possibleSize)
{
	for (int index = 0; index < MAX_SIZE; index++)
	{
		if (sudokuBoard[i][index] != -1)
		{
			updatePosArr(relevantOptions, possibleSize, sudokuBoard[i][index]);
		}
	}
}

/*check each Col and update the tmp array with the possible numbers remaining*/
void checkCol(short sudokuBoard[][9], int j, short* relevantOptions, int* possibleSize)
{
	for (int index = 0; index < MAX_SIZE; index++)
	{
		if (sudokuBoard[index][j] != -1)
		{
			updatePosArr(relevantOptions, possibleSize, sudokuBoard[index][j]);
		}
	}
}

/*update the arr (mark the numbers we cant choose)*/
void updatePosArr(short* relevantOptions, int* possibleSize, short valuetodelete)
{
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (relevantOptions[i] == valuetodelete)
		{
			relevantOptions[i] = -1;/*"delete" the number that cannot be used from the possible array(-1 not valid)*/
			*possibleSize = (*possibleSize) - 1;
		}
	}
}

/*print the sudoko board*/
void sudokoPrintBoard(short sudokuBoard[][9])
{
	printf("  | 0 1 2| 3 4 5| 6 7 8\n");
	printf("-----------------------\n");
	for (int i = 0; i < 9; i++)
	{
		printf(" %d|", i);
		for (int j = 0; j < 9; j++)
		{
			if (sudokuBoard[i][j] == -1)
			{
				printf("  ");
			}
			else
			{
				printf(" %d", sudokuBoard[i][j]);
			}
			if (((j + 1) % 3 == 0) && ((j + 1) != 9))
			{
				printf("|");
			}
		}
		printf("\n");
		if (((i + 1) % 3) == 0)
		{
			printf("-----------------------\n");
		}
	}
}

/*return the middle of the current square*/
int getMiddleSquare(int current)
{
	if (current >= 0 && current <= 2)
		return 1;

	else if (current >= 3 && current <= 5)
		return 4;

	else
		return 7;/*(current >= 6 && current <= 8)*/
}

/*The function tries to fill the board , she check for a cells that have only one  digit legally.
When the function find such a cell, she will update the board.
Also, she's remove the cell from possibilities that fits the cell and will update the row, column and square 3X3 of
The same slot. and she continue to do this until there is no more those cells.*/
int OneStage(short board[][9], Array*** possibilities, int* x, int* y, bool* keepLooping)
{
	int minArrSize = MAX_SIZE + 1; // Set to 10 - heigher then maximun size. 
	int result = FINISH_SUCCESS;
	bool sizeOneUpdated = false;

	for (int curRow = 0; curRow < MAX_SIZE; curRow++)
	{
		for (int curCol = 0; curCol < MAX_SIZE; curCol++)
		{
			// If board cell is not empty.
			if (board[curRow][curCol] != EMPTY_CELL)
				continue;

			int arrSize = possibilities[curRow][curCol]->size;

			if (arrSize == 1)
			{
				board[curRow][curCol] = possibilities[curRow][curCol]->arr[0]; // Update board cell value.
				sizeOneUpdated = true;

				// update content of relevant cells.
				bool res = updateRelevantCells(possibilities, board[curRow][curCol], curRow, curCol);

				if (res)
				{
					*keepLooping = false;
					return FINISH_FAILURE;
				}
				continue;
			}

			if (minArrSize > arrSize)// we will update the min and the coordinates
			{
				minArrSize = arrSize;
				*x = curRow;
				*y = curCol;
				result = NOT_FINISH;
			}
		}
	}
	if (!sizeOneUpdated)
		*keepLooping = false;

	return result;
}

/*Q3*/

/*function receieve Array*** of cells, value to remove, current row and col.
function go through current row, col, square and remove 'value' as option from each cell.*/
bool updateRelevantCells(Array*** pos, short value, int row, int col)
{
	bool res = false;
	updateRow(pos, value, row, col, &res);
	updateCol(pos, value, row, col, &res);
	updateSquare(pos, value, row, col, &res);

	freeArray(pos[row][col]);
	pos[row][col] = NULL;

	return res;
}

/*update each Row and remove from array the relevant number*/
void updateRow(Array*** pos, short value, int row, int col, bool* res)
{
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (pos[row][i] == NULL || i == col)
			continue;

		removeValueFromArray(pos[row][i], value, res);
	}
}

/*update each Col and remove from array the relevant number*/
void updateCol(Array*** pos, short value, int row, int col, bool* res)
{
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (pos[i][col] == NULL || i == row)
			continue;

		removeValueFromArray(pos[i][col], value, res);
	}
}

/*update each Square and remove from array the relevant number*/
void updateSquare(Array*** pos, short value, int row, int col, bool* res)
{
	// returns center of 3x3 square indexes.
	int relRow = getMiddleSquare(row);
	int relCol = getMiddleSquare(col);

	for (int i = relRow - 1; i <= relRow + 1; i++)
	{
		for (int j = relCol - 1; j <= relCol + 1; j++)
		{
			if (pos[i][j] == NULL || (i == row && j == col))
				continue;

			removeValueFromArray(pos[i][j], value, res);
		}
	}
}

// remove if exist value from pos short array.
void removeValueFromArray(Array* pos, int value, bool* duplicateExist)
{
	if (pos == NULL)
		return;

	for (int i = 0; i < pos->size; i++)
	{
		if (pos->arr[i] == value)
		{
			short* tmpArr = (short*)malloc(sizeof(short) * (pos->size - 1));

			copyArrExcepttheNum(tmpArr, pos->arr, pos->size, value); // copy the array without the "value"
			copyUpdateArr(pos->arr, tmpArr, pos->size - 1);// return the update array

			//Resize array
			pos->size -= 1;

			if (pos->size > 0)
			{
				pos->arr = (short*)realloc(pos->arr, sizeof(short) * (pos->size));
				checkMemoryAllocation(pos->arr);
			}
		}
	}

	if (pos->size <= 0)// if pos != NULL and the size is under 1 so duplicateExist is true 
		*duplicateExist = true;
}

//copy all the array to a tmp array except the "value"
void copyArrExcepttheNum(short* dest, short* src, int arrSize, int value)
{
	int dest_index = 0;

	for (int i = 0; i < arrSize; i++)
	{
		if (src[i] != value)
		{
			dest[dest_index] = src[i];
			dest_index++;
		}
	}
}

//copy in return the array from tmp to array
void copyUpdateArr(short* dest, short* src, int arrSize)
{
	int dest_index = 0;

	for (int i = 0; i < arrSize; i++)
	{
		dest[dest_index] = src[i];
		dest_index++;
	}
}

//freeing the array
void freeArray(Array* posArray)
{
	if (posArray == NULL)
		return;

	free(posArray->arr);
	free(posArray);
}

//checks if the board is full and returns true, if not yet returns false
bool isBoardFull(Array*** possibilities)
{
	for (int i = 0; i < MAX_SIZE; i++)
	{
		for (int j = 0; j < MAX_SIZE; j++)
		{
			if (possibilities[i][j] != NULL)
			{
				return false;
			}
		}
	}
	return true;
}

//function for checking memory allocation
void checkMemoryAllocation(void* arr)
{
	if (arr == NULL)
	{
		puts("Memory allocation failed");
		exit(1);
	}
}



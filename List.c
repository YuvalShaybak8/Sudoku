#include "List.h"
#include "Tree.h"

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$       Project : Sudoku              $$
$$       By:                           $$
$$   Rannan Harpak  - id : 315816983   $$
$$   Yuval Saybak   - id : 313524837   $$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

/*function that star and play the game*/
void startGame()
{
	Tree playerTree;
	int x;//number of players
	playerListNode** mergArr;
	PlayerList activePlayers;
	makeEmptyPlayerList(&activePlayers);

	PlayerList winnerList;
	makeEmptyPlayerList(&winnerList);

	printf("Enter the number of players\n");
	scanf("%d", &x);

	mergArr = (playerListNode**)malloc(sizeof(playerListNode*) * x); // array of pointers
	checkMemoryAllocation(mergArr);

	getchar();

	for (int k = 0; k < x; k++)
	{
		PlayerInfo* player = (PlayerInfo*)malloc(sizeof(PlayerInfo));
		player->name = getPlayerName();

		for (int e = 0; e < 9; e++)
		{
			for (int z = 0; z < 9; z++)
			{
				player->board[e][z] = -1;
			}
		}

		player->possibleDig = PossibleDigits(player->board);

		makeRandomBoard(player);

		mergArr[k] = insertPlayerToEndList(&activePlayers, player);
	}

	mergeSort(mergArr, x);

	double n = log2(x + 1);//log 2 
	n = ceil(n);// round up
	n = pow((double)2, n);// power of 2
	n -= 1;// -1

	int updateSize = (int)n;
	mergArr = (playerListNode**)realloc(mergArr, sizeof(playerListNode*) * updateSize);
	checkMemoryAllocation(mergArr);

	for (int i = x; i < updateSize; i++)
	{
		mergArr[i] = NULL;
	}

	playerTree = BuildTreeFromArray(mergArr, updateSize);

	free(mergArr);

	gameTree(playerTree, &activePlayers, &winnerList);
}

/*random the numbers of the cells will be fill at thee start of the game
and random the cells*/
void makeRandomBoard(PlayerInfo* player)
{
	CellList randomBoard;
	makeEmptyList(&randomBoard);
	int N, K, U;
	int removeNumber;
	Cell currCell;
	Cell cellToDelete;

	for (int i = 0; i < MAX_SIZE; i++)
	{
		for (int j = 0; j < MAX_SIZE; j++)
		{
			currCell.row = i;
			currCell.col = j;
			insertDataToEndList(&randomBoard, currCell);
		}
	}

	ListNode* curr;
	N = rand() % 20 + 1;// random the fill board digits.
	player->fullCells = 81 - N;

	for (int t = 0; t < N; t++)
	{
		K = rand() % (82 - t); // random cell minus the number we already get.
		curr = randomBoard.head;

		for (int m = 0; m < K; m++)
		{
			curr = curr->next;
		}

		cellToDelete.row = curr->cell.row;
		cellToDelete.col = curr->cell.col;

		removeNodeFromList(&randomBoard, curr);//remove the random cell from the list

		U = rand() % (player->possibleDig[cellToDelete.row][cellToDelete.col]->size);
		removeNumber = player->possibleDig[cellToDelete.row][cellToDelete.col]->arr[U];
		player->board[cellToDelete.row][cellToDelete.col] = removeNumber;

		updateRelevantCells(player->possibleDig, removeNumber, cellToDelete.row, cellToDelete.col);//put the random cell of the board
	}
}

/*remove node from list*/
void removeNodeFromList(CellList* lst, ListNode* node)
{
	if (lst->head == lst->tail) // == node)
		lst->head = lst->tail = NULL;
	else if (lst->head == node) // or node->prev == NULL
	{
		lst->head = lst->head->next;
		lst->head->prev = NULL;
	}
	else if (lst->tail == node) // or node->next == NULL
	{
		lst->tail = lst->tail->prev;
		lst->tail->next = NULL;
	}
	else// middle
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	free(node);
}

/*insert data to end of list*/
void insertDataToEndList(CellList* lst, Cell currCell)
{
	ListNode* res = createNewListNode(currCell, NULL, NULL);

	insertNodeToEndList(lst, res);
}

/*insert node to end of list*/
void insertNodeToEndList(CellList* lst, ListNode* newTail)
{
	if (isEmptyList(*lst))
	{
		newTail->next = newTail->prev = NULL;
		lst->head = lst->tail = newTail;
	}
	else
	{
		newTail->prev = lst->tail;
		newTail->next = NULL;
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

/*create new list node*/
ListNode* createNewListNode(Cell currCell, ListNode* next, ListNode* prev)
{
	ListNode* res = (ListNode*)malloc(sizeof(ListNode));
	checkMemoryAllocation(res);

	res->cell.row = currCell.row;
	res->cell.col = currCell.col;
	res->next = next;
	res->prev = prev;

	return res;
}

/* make empty list*/
void makeEmptyList(CellList* lst)
{
	lst->head = lst->tail = NULL;
}

/*check if the list is already empty*/
int isEmptyList(CellList lst)
{
	return (lst.head == NULL);
}

/*freeing the list*/
void freeList(CellList lst)
{
	freeHelper(lst.head);
}

/*helper function to freeList*/
void freeHelper(ListNode* head)
{
	if (head != NULL)
	{
		freeHelper(head->next);
		free(head);
	}
}

/*get from the user the name of the player*/
char* getPlayerName()
{
	int phySize = 1, logSize = 0;
	char* playerName = (char*)malloc(sizeof(char) * phySize);
	checkMemoryAllocation(playerName);

	char buffer = -1;
	scanf("%c", &buffer);

	bool keep = true;

	while (buffer != '\n' && buffer != -1)
	{

		playerName[logSize] = buffer;
		logSize++;

		if (logSize == phySize)
		{
			phySize *= 2;
			playerName = (char*)realloc(playerName, sizeof(char) * phySize);
		}

		buffer = getchar();
	}

	playerName = (char*)realloc(playerName, sizeof(char) * logSize + 1);
	checkMemoryAllocation(playerName);

	playerName[logSize] = '\0';

	return playerName;
}

/*print all the winner List*/
void printWinnerList(PlayerList* winnerList)
{
	playerListNode* curr;
	curr = winnerList->head;

	while (curr != NULL)
	{
		printf("Player Name : %s \n", curr->player->name);
		sudokoPrintBoard(curr->player->board);
		curr = curr->next;
		printf("\n\n");
	}
}

/*function that delete node from activePlayers list*/
void deleteNodeFromList(PlayerList* activePlayers, playerListNode* currentPlayer)
{
	if (activePlayers->head == activePlayers->tail) // == node)
		activePlayers->head = activePlayers->tail = NULL;

	else if (activePlayers->head == currentPlayer) // remove the head
	{
		activePlayers->head = activePlayers->head->next;
		activePlayers->head->prev = NULL;
	}
	else if (activePlayers->tail == currentPlayer) // remove the tail
	{
		//activePlayers->tail->prev->next = NULL;
		activePlayers->tail = activePlayers->tail->prev;
	}
	else// middle
	{
		currentPlayer->prev->next = currentPlayer->next;
		currentPlayer->next->prev = currentPlayer->prev;
	}
	free(currentPlayer);
}

/*Fills the board according to the player's choices to fill the possible empty cells
as long as the player has not lost or won. */
int FillBoard(short board[][9], Array*** possibilities)
{
	int result;
	int x, y;

	while (true)// while we playing
	{
		int choice;
		bool keepLooping = true;
		while (keepLooping)// while there is cell to fill keeplooping
		{
			result = OneStage(board, possibilities, &x, &y, &keepLooping);// fill the board in the relevantplaces
		}

		if (result != -1 && result != 1)// print the board only if NOT_FINISH
			sudokoPrintBoard(board);

		if (result == FINISH_SUCCESS)
			break;

		bool duplicateExist = false;

		if (result == NOT_FINISH)// if NOT_FINISH keep show the minimum possible values to the player
		{
			printf("\n\nCell number [%d,%d] currently holds the minimum number of possible values,select one of the below:\n", x, y);
			for (int i = 0; i < possibilities[x][y]->size; i++)
			{ 
				printf("%d. %hi \n", i + 1, possibilities[x][y]->arr[i]); //print all the possibilities
			}

			scanf("%d", &choice);

			while (heIsntThere(choice,possibilities[x][y]) == true)
			{
				printf("The number cannot be selected\n\n");

				printf("\n\nCell number [%d,%d] currently holds the minimum number of possible values,select one of the below:\n", x, y);
				for (int i = 0; i < possibilities[x][y]->size; i++)
				{
					printf("%d. %hi \n", i + 1, possibilities[x][y]->arr[i]); //print all the possibilities
				}

				scanf("%d", &choice);
			}
			printf("Updating cell %d with value %d\n\n\n\n", ((x * 9) + y), choice);

			short relevantOptions[] = { choice };// make new arr with the relevant choice
			board[x][y] = choice;
			duplicateExist = updateRelevantCells(possibilities, choice, x, y);// remove the "choice" option from all the relevants places
		}

		// If duplicates exists, return fail.
		if (result == FINISH_FAILURE || duplicateExist == true)
		{
			result = FINISH_FAILURE;
			break;
		}

		//check the board in full so far
		if (isBoardFull(possibilities))
		{
			result = FINISH_SUCCESS;
			break;
		}
	}
	return result;
}

/*check fi the choice are valid or not*/
bool heIsntThere(int choice, Array* possibilities)
{
	for (int i = 0; i < possibilities->size; i++)
	{
		if (possibilities->arr[i] == choice)
		{
			return false;
		}
	}
	return true;
}

/*merge Sort all the activePlayers */
void mergeSort(playerListNode** arr, int size)
{
	playerListNode** tmpArr = NULL;

	if (size <= 1)
		return;

	mergeSort(arr, size / 2);
	mergeSort(arr + size / 2, size - size / 2);
	tmpArr = merge(arr, size / 2, arr + size / 2, size - size / 2);

	/*make another copyArr*/
	if (tmpArr)
	{
		for (int i = 0; i < size; i++) // copy values from tmpArr to arr
		{
			arr[i] = tmpArr[i];
		}
		free(tmpArr);
	}
	else
	{
		printf("Memory allocation failure!!!\n");
		exit(1);	// end program immediately with code 1 (indicating an error)
	}
}

/*merge function*/
playerListNode** merge(playerListNode** a1, int size1, playerListNode** a2, int size2)
{
	int ind1 = 0, ind2 = 0, resInd = 0;

	playerListNode** res = (playerListNode**)malloc((size1 + size2) * sizeof(playerListNode*));
	checkMemoryAllocation(res);

	int counterOfequal = 0;

	while ((ind1 < size1) && (ind2 < size2))
	{
		if (a1[ind1]->player->fullCells < a2[ind2]->player->fullCells)
		{
			res[resInd] = a1[ind1];
			ind1++;
		}
		else if (a1[ind1]->player->fullCells > a2[ind2]->player->fullCells)
		{
			res[resInd] = a2[ind2];
			ind2++;
		}
		else // they equals
		{
			if (counterOfequal < 2)
			{
				res[resInd] = a1[ind1];
				ind1++;
				counterOfequal++;
			}
			else
			{
				if (strcmp(a1[ind1]->player->name, a2[ind2]->player->name) < 0)
				{
					res[resInd] = a1[ind1];
					ind1++;
				}
				else
				{
					res[resInd] = a2[ind2];
					ind2++;
				}
			}
		}
		resInd++;
	}

	while (ind1 < size1)
	{
		res[resInd] = a1[ind1];
		ind1++;
		resInd++;
	}
	while (ind2 < size2)
	{
		res[resInd] = a2[ind2];
		ind2++;
		resInd++;
	}
	return res;
}

/*check if the player list is already empty*/
int isEmptyPlayerList(PlayerList* lst)
{
	return (lst->head == NULL);
}

/* make empty player list*/
void makeEmptyPlayerList(PlayerList* lst)
{
	lst->head = lst->tail = NULL;
}

/*insert player and his data to end of list*/
playerListNode* insertPlayerToEndList(PlayerList* activePlayers, PlayerInfo* player)
{
	playerListNode* res = createNewPlayerNode(player, NULL, NULL);

	insertPlayerNodeToEndList(activePlayers, res);

	return res;
}

/*create new player list node*/
playerListNode* createNewPlayerNode(PlayerInfo* player, playerListNode* next, playerListNode* prev)
{
	playerListNode* res = (playerListNode*)malloc(sizeof(playerListNode));
	checkMemoryAllocation(res);

	res->player = player;
	res->next = next;
	res->prev = prev;

	return res;
}

/*insert player node to end of list*/
void insertPlayerNodeToEndList(PlayerList* lstPlayer, playerListNode* newTail)
{
	if (isEmptyPlayerList(lstPlayer))
		lstPlayer->head = lstPlayer->tail = newTail;

	else
	{
		lstPlayer->tail->next = newTail;
		lstPlayer->tail = newTail;
	}
}




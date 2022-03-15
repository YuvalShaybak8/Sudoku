#ifndef __LIST_H
#define __LIST_H

#include "Pointers.h"

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$       Project : Sudoku              $$
$$       By:                           $$
$$   Rannan Harpak  - id : 315816983   $$
$$   Yuval Saybak   - id : 313524837   $$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

#define MAX_PLAYER_NAME 101

typedef struct cell
{
    int row;
    int col;
}Cell;

typedef struct listNode
{
    struct listNode* prev;
    Cell cell;
    struct listNode* next;
}ListNode;

typedef struct celllist
{
    ListNode* head;
    ListNode* tail;
}CellList;

typedef struct playerinfo
{
    int fullCells;
    char* name;
    short board[9][9];
    Array*** possibleDig;
}PlayerInfo;

typedef struct playerlistNode
{
    PlayerInfo* player;
    struct playerlistNode* prev;
    struct playerlistNode* next;
}playerListNode;

typedef struct playerlist
{
    playerListNode* head;
    playerListNode* tail;
}PlayerList;

//--function declaration--//

char* getPlayerName();
void freeList(CellList lst);
int isEmptyList(CellList lst);
void freeHelper(ListNode* head);
void makeEmptyList(CellList* lst);
int isEmptyPlayerList(PlayerList* lst);
void makeRandomBoard(PlayerInfo* player);
void makeEmptyPlayerList(PlayerList* lst);
void printWinnerList(PlayerList* winnerList);
void mergeSort(playerListNode** arr, int size);
bool heIsntThere(int choice, Array* possibilities);
void removeNodeFromList(CellList* lst, ListNode* node);
void insertDataToEndList(CellList* lst, Cell currCell);
int FillBoard(short board[][9], Array*** possibilities);
void insertNodeToEndList(CellList* lst, ListNode* newTail);
ListNode* createNewListNode(Cell currCell, ListNode* next, ListNode* prev);
void deleteNodeFromList(PlayerList* activePlayers, playerListNode* currActive);
void insertPlayerNodeToEndList(PlayerList* lstPlayer, playerListNode* newTail);
playerListNode* insertPlayerToEndList(PlayerList* activePlayers, PlayerInfo* player);
playerListNode** merge(playerListNode** a1, int size1, playerListNode** a2, int size2);
playerListNode** merge(playerListNode** a1, int size1, playerListNode** a2, int size2);
playerListNode* createNewPlayerNode(PlayerInfo* player, playerListNode* next, playerListNode* prev);

#endif

#include "Tree.h"

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$       Project : Sudoku              $$
$$       By:                           $$
$$   Rannan Harpak  - id : 315816983   $$
$$   Yuval Saybak   - id : 313524837   $$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

/*play the game from the tree*/
void gameTree(Tree playerTree,PlayerList* activePlayers, PlayerList* winnerList)
{
    gameTreeHelper(playerTree.root,activePlayers, winnerList);
    freeTree(playerTree);//after we finish with the tree we freeing him
}

/*play the game from the tree rec (helper)*/
void gameTreeHelper(TreeNode* root,PlayerList* activePlayers, PlayerList* winnerList)
{
    if (root == NULL)
        return;

    else if (root->data == NULL)
    {
        gameTreeHelper(root->left, activePlayers, winnerList);
        gameTreeHelper(root->right, activePlayers, winnerList);
        free(root->data);
    }
    else
    {
        gameTreeHelper(root->left, activePlayers, winnerList);
        playerListNode* currentPlayer = root->data;
        printf("\nThis is %s turn:\n", currentPlayer->player->name);
        int res = FillBoard(currentPlayer->player->board, currentPlayer->player->possibleDig);//fill the board

        if (res == FINISH_FAILURE)//the player lost the game
        {
            printf("\n\n%s Lost The Game !\n\n", currentPlayer->player->name);
            deleteNodeFromList(activePlayers, currentPlayer);//we delete the player from the list and put there a NULL
            root->data = NULL;
        }
        else if (res == FINISH_SUCCESS)// the player win the game
        {
            printf("\n\n%s Win The Game !\n\n", currentPlayer->player->name);
            insertPlayerToEndList(winnerList, currentPlayer->player);//we insert the player the won from the activeplayerList to WinnerList
            deleteNodeFromList(activePlayers, currentPlayer);//we delete the player from the list and put there a NULL
            root->data = NULL;
        }
        gameTreeHelper(root->right, activePlayers, winnerList);
    }
    printWinnerList(winnerList);//we print the winner List
}

/*create a tree from array Rec*/
Tree BuildTreeFromArray(playerListNode** mergArr, int  n)
{
    Tree tr;

    tr.root = createNewTreeRec(mergArr, n);

    return tr;
}

/*rec function to create tree root*/
TreeNode* createNewTreeRec(playerListNode** mergArr, int  n)
{
    TreeNode* node = NULL;
    TreeNode* left = NULL;
    TreeNode* right = NULL;
    int mid = n / 2;

    if (n == 0)
        return node;
    else
    {
        if (mergArr[mid / 2] != NULL)
        {
            left = createNewTreeRec(mergArr, mid);/*creating left root of tree*/
        }
        if (mergArr[mid + (mid / 2) + 1] != NULL)
        {
            right = createNewTreeRec(mergArr + mid + 1, n - mid - 1);/*creating right root of tree*/
        }
        node = createNewTreeNode(mergArr[mid], left, right);/*creating the root of entire tree*/

        return node;
    }
}

/*function to create a single tree node*/
TreeNode* createNewTreeNode(playerListNode* data, TreeNode* left, TreeNode* right)
{
    TreeNode* res = (TreeNode*)malloc(sizeof(TreeNode));/*allocating memory*/
    checkMemoryAllocation(res);/*checking memory allocation*/

    /*inputting into the node*/
    res->data = data;
    res->left = left;
    res->right = right;

    return res;
}

/*freeing function*/
void freeTree(Tree tr)
{
    freeTreeHelper(tr.root);
}

/*helper function*/
void freeTreeHelper(TreeNode* root)
{
    if (root != NULL)
    {
        freeTreeHelper(root->left);
        freeTreeHelper(root->right);
        free(root->data);
        free(root);
    }
}


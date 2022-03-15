#ifndef _TREE_H
#define _TREE_H

#include "List.h"

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$       Project : Sudoku              $$
$$       By:                           $$
$$   Rannan Harpak  - id : 315816983   $$
$$   Yuval Saybak   - id : 313524837   $$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

typedef struct treeNode
{
    playerListNode* data;
    struct treeNode* left;
    struct treeNode* right;
}TreeNode;

typedef struct tree
{
    TreeNode* root;
}Tree;

//--function declaration--//
void freeTree(Tree tr);
void freeTreeHelper(TreeNode* root);
Tree BuildTreeFromArray(playerListNode** mergArr, int  n);
TreeNode* createNewTreeRec(playerListNode** mergArr, int  n);
void gameTree(Tree playerTree,PlayerList* activePlayers, PlayerList* winnerList);
TreeNode* createNewTreeNode(playerListNode* data, TreeNode* left, TreeNode* right);
void gameTreeHelper(TreeNode* root,PlayerList* activePlayers, PlayerList* winnerList);

#endif


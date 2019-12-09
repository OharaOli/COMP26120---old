#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define __USE_BSD
#include <string.h>

#include "global.h"
#include "bstree.h"


// Can be redefined if Value_Type changes
int compare(Value_Type a, Value_Type b){
  return strcmp(a,b);
}


struct bstree* initialize_set (int size)
{
  // An empty tree is represented by NULL
  return NULL;
}

void tidy(struct bstree* tree)
{
  if(tree){
    free(tree->left);
    free(tree->right);
    free(tree);
  }
}

int size(struct bstree* tree){
  if(tree){
    return size(tree->left) + size(tree->right);
  }
  return 0;
}

struct bstree* insert (Value_Type value, struct bstree* tree)
{
  if(tree)
  {
    // TODO if tree is not NULL then insert into the correct sub-tree
    if (value == tree->value)
    {
      return tree;
    }
    else if(value > tree->value)
    {
      tree->right = insert(value, tree->right);
    }
    else
    {
      tree->left = insert(value, tree->left);
    }
  }
  else
  {
    // TODO otherwise create a new node containing the value
    tree->value = value;
    tree->left = NULL;
    tree->right = NULL;
    tree->numberOfNodes = tree->numberOfNodes + 1;
  }
  return tree;
}

bool find (Value_Type value, struct bstree* tree)
{
  if(tree)
  {
    //TODO complete the find function
    if(value > tree->value)
    {
      return find(value, tree->right);
    }
    else
    {
      return find(value, tree->left);
    }
  }
  // if tree is NULL then it contains no values
  return false;
}

// You can update this if you want
void print_set_recursive(struct bstree* tree, int depth)
{
  if(tree){
    for(unsigned i=0;i<depth;i++){ printf(" "); }
    printf("%s\n",tree->value);
    print_set_recursive(tree->left,depth+1);
    print_set_recursive(tree->right,depth+1);
  }
}
// You can update this if you want
void print_set (struct bstree* tree)
{
 printf("Tree:\n");
 print_set_recursive(tree,0);
}

//Find the depth/height of a given tree
int height (struct bstree* tree)
{
   //If the tree is empty return 0
   if (tree->value == NULL)
   {
      return 0;
   } //if
   else
   {
      //Find the height of the left and right branch
      int leftHeight = height(tree->left);
      int rightHeight = height(tree->right);

      //Add one to the current heighest branch to find the depth
      if(leftHeight > rightHeight)
      {
         return (leftHeight + 1);
      } //if
      else
      {
         return (rightHeight + 1);
      } //else
   } //else
} //height

void print_stats (struct bstree* tree)
{
   // TODO update code to record and print statistics
   printf("The height/depth of the tree is: %d", height(tree));
   printf("The number of nodes in this tree: %d", tree->numberOfNodes);
}

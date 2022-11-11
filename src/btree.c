#include "btree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//DISCLAMER!!!! 
//Har lavet denne opgave med samtaler + inspiration fra Andreas Mandøe 
//Koden er ikke super overskuelig og kan muligvis forbedres en del. Men dette var min "intuitive" måde at løse opgaven på
//Håber ikke det er for stort et problem :) 

void find_placement(int x, struct tree_node *t) 
{

//specialcase if "t" has no value
  if (t==NULL) 
  {
    t=malloc(sizeof(struct tree_node));
    t -> item = x;
    t -> left = NULL;
    t -> right = NULL;
  }

//leafnode case
  else if (t -> left == NULL && t -> right == NULL) 
  {

  //if less move to the left
    if (x <= t -> item) 
    {
      t -> left = malloc(sizeof(struct tree_node));
      t -> left -> item = x;
      t -> left -> left = NULL;
      t -> left -> right = NULL;

    }

  //If bigger then go to the right
  else if (x > t -> item)
    {
      t -> right = malloc(sizeof(struct tree_node));
      t -> right -> item = x;
      t -> right -> left = NULL;
      t ->right-> right = NULL;
    }

  }

//go left and recurssive back
  else if (t -> left != NULL && x <= t -> item) 
  {
    find_placement(x,t->left);
  }

  else if (t->left == NULL && x <= t->item) 
  {
    t->left = malloc(sizeof(struct tree_node));

    t->left->left = NULL;
    t->left->right = NULL;

    t->left->item=x;
  }



//go right and recurssive back
  else if (t->right != NULL && x > t->item) 
  {
    find_placement(x,t->right);
  }

  else if (t->right == NULL && x > t->item) 
  {
    t->right = malloc(sizeof(struct tree_node));

    t->right->left = NULL;
    t->right->right = NULL;

    t->right->item=x;
  }

}


struct tree_node *Insert(int x, struct tree_node *t) 
{
  // Insert item x into the tree t
  if( t == NULL)
  {
    t = malloc(sizeof(struct tree_node));
    t-> item = x;
    t -> left = NULL;
    t -> right = NULL;
    return t;
  }
  find_placement(x,t);
  
  return t;
}
bool removed = false;


struct tree_node* remove_recursively(int x, struct tree_node *t) 
{

  //If t is the root
  if ((x==t->item && t->left->item!=x) || (x==t->item && t->left==NULL)) 
  {
    return t;
  }

  if ((t->left != NULL && x==t->left->item && t->left->left==NULL) || (t->left != NULL && x==t->left->item && t->left->left->item!=x)) 
  {
    //return parent 
    return t;

  } 
    else if ((t->right != NULL && x==t->right->item && t->right->left==NULL) || (t->right != NULL && x==t->right->item && t->right->left->item!=x))
    {
      //return parent
      return t;
    } 
      else if (x<=t->item && t->left!=NULL) 
      {
        return (remove_recursively(x,t->left));
      } 
        else if (x>t->item && t->right!=NULL) 
        {
          return (remove_recursively(x,t->right));
        }

  

}

struct tree_node *find_replacement(struct tree_node *replacement) 
{

  if (replacement -> right == NULL) 
  {
    return replacement;
  }
    else
    {
      return find_replacement(replacement -> right);
    }

}

struct tree_node *parent_replacement(struct tree_node *replacement) 
{

  if (replacement -> right -> right == NULL) 
  {
    return replacement;
  }
    else 
    {
      return find_replacement(replacement -> right);
    }

}



void change_places(int x, struct tree_node *parent_node_to_remove) //changes place between 
{

struct tree_node *node_to_remove = malloc(sizeof(struct tree_node));

  if (x<=parent_node_to_remove -> item) 
  {
    node_to_remove = parent_node_to_remove -> left;
  } 
    else
    {
      node_to_remove = parent_node_to_remove -> right;
    }


//Specialcase where leafnode is to be removed
  if (node_to_remove -> left == NULL && node_to_remove -> right == NULL) 
  {
    node_to_remove -> left=NULL;
    node_to_remove -> right=NULL;

    if (parent_node_to_remove -> left == node_to_remove) 
    {
      parent_node_to_remove -> left=NULL;
      free(node_to_remove);
    } 
      else if (parent_node_to_remove -> right == node_to_remove) 
      {
      parent_node_to_remove -> right = NULL;
      free(node_to_remove);
      }

  } 
  else 
  {
    
      struct tree_node *replacement = malloc(sizeof(struct tree_node)); //Finding the node to replace 

      if (node_to_remove -> left != NULL) 
      {
        replacement = find_replacement(node_to_remove->left);
      } 
        else
        {
          replacement = find_replacement(node_to_remove->right);
        }

    if (replacement==node_to_remove->left) 
    {
      node_to_remove->item = node_to_remove->left->item;
      node_to_remove->left = NULL;
      free(replacement);
    }

    else if (replacement!=node_to_remove->left) 
    {
      struct tree_node *parent_of_replacement = malloc(sizeof(struct tree_node)); //If the replacement is a child of node to be removed
      if (node_to_remove->left != NULL) 
      {
        parent_of_replacement = parent_replacement(node_to_remove->left);
      } 
      else 
      {
        parent_of_replacement = parent_replacement(node_to_remove);
      }
      

      node_to_remove->item = replacement->item;
      parent_of_replacement -> right=NULL;

      free(replacement);
    } 
  }
}

struct tree_node *Remove(int x, struct tree_node *t) 
{
  // Remove one item from the tree t 
  if (Contains(x,t)==true) 
  {
    struct tree_node *ToBe_removed_parent = malloc(sizeof(struct tree_node));
    ToBe_removed_parent = remove_recursively(x,t);
    change_places(x,ToBe_removed_parent);
  }
  
  return t;
}

int Contains(int x, struct tree_node *t) 
{

  // Return true if the tree t contains item x. Return false otherwise.
  //Check the roots
  if(t -> item == x)
  {
    return true;
  }
    else if (x <= t -> item && t -> left != NULL)
    {
      return Contains(x, t -> left);
    }
      else if (x > t -> item && t -> right != NULL)
      { 
        return Contains(x, t -> right);
      }
  else 
  {
    return false;
  }
}

struct tree_node *Initialize(struct tree_node *t) 
{
  // Create an empty tree
  t == NULL; 
  return t;
}

int Empty(struct tree_node *t) 
{
  // Test if empty
  if(t == NULL)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int Full(struct tree_node *t) 
{
  // Test if full
  return 1;
}

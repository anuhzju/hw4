#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int findPathLength(Node* root, int length, int& leafLength);

bool equalPaths(Node * root)
{
  int leafLength = -1;
  return findPathLength(root, 0, leafLength);
}

int findPathLength(Node* root, int length, int& leafLength){
  if (root == nullptr){
    return true;
  }

  // if we reach a leaf node
  if(root -> left == nullptr && root -> right == nullptr){
    if (leafLength == -1){
      leafLength = length;
      return true;
    }
    return length == leafLength;
  }

  return findPathLength(root -> left, length + 1, leafLength)
  && findPathLength(root -> right, length + 1, leafLength);
}
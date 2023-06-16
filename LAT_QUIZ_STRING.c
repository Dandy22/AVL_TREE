#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  char gameTitle[50], gameGenre[50], gameStock[50];
  struct Node *left, *right;
  int height;
};

int max(int a, int b) { return (a > b) ? a : b; }

int height(struct Node *root) {
  if (root == NULL)
    return 0;
  return root->height;
}

int getBalance(struct Node *root) {
  if (root == NULL)
    return 0;
  return height(root->left) - height(root->right);
}

struct Node *newNode(char *gameTitle, char *gameGenre, char *gameStock) {
  struct Node *tnode = (struct Node *)malloc(sizeof(struct Node));
  strcpy(tnode->gameTitle, gameTitle);
  strcpy(tnode->gameGenre, gameGenre);
  strcpy(tnode->gameStock, gameStock);
  tnode->left = tnode->right = NULL;
  tnode->height = 1;
  return tnode;
}

struct Node *leftRotate(struct Node *root) {
  struct Node *x = root->right;
  struct Node *sub = x->left;

  x->left = root;
  root->right = sub;

  root->height = max(height(root->left), height(root->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;
  return x;
}

struct Node *rightRotate(struct Node *root) {
  struct Node *x = root->left;
  struct Node *sub = x->right;

  x->right = root;
  root->left = sub;

  root->height = max(height(root->left), height(root->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;
  return x;
}

struct Node *minVal(struct Node *root) {
  struct Node *curr = root;
  while (curr->left != NULL)
    curr = curr->left;
  return curr;
}

struct Node *insertNode(struct Node *root, char *gameTitle, char *gameGenre,
                        char *gameStock) {
  if (root == NULL) {
    return newNode(gameTitle, gameGenre, gameStock);
  }

  if (strcmp(gameStock, root->gameStock) < 0) {
    root->left = insertNode(root->left, gameTitle, gameGenre, gameStock);
  } else if (strcmp(gameStock, root->gameStock) > 0) {
    root->right = insertNode(root->right, gameTitle, gameGenre, gameStock);
  } else {
    return root;
  }

  root->height = 1 + max(height(root->left), height(root->right));

  int balance = getBalance(root);

  // Left Left Case
  if (balance > 1 && strcmp(gameStock, root->left->gameStock) < 0) {
    return rightRotate(root);
  }

  // Right Right Case
  if (balance < -1 && strcmp(gameStock, root->right->gameStock) > 0) {
    return leftRotate(root);
  }

  // Left Right Case
  if (balance > 1 && strcmp(gameStock, root->left->gameStock) > 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  // Right Left Case
  if (balance < -1 && strcmp(gameStock, root->right->gameStock) < 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

struct Node *deleteNode(struct Node *root, char *gameTitle) {
  if (root == NULL) {
    return NULL;
  }

  if (strcmp(gameTitle, root->gameTitle) < 0) {
    root->left = deleteNode(root->left, gameTitle);
  } else if (strcmp(gameTitle, root->gameTitle) > 0) {
    root->right = deleteNode(root->right, gameTitle);
  } else {
    if (root->left == NULL || root->right == NULL) {
      struct Node *temp = root->left ? root->left : root->right;
      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else {
        *root = *temp;
      }
      free(temp);
    } else {
      struct Node *pred = minVal(root->right);
      strcpy(root->gameTitle, pred->gameTitle);
      strcpy(root->gameGenre, pred->gameGenre);
      strcpy(root->gameStock, pred->gameStock);
      root->right = deleteNode(root->right, pred->gameTitle);
    }
  }

  if (root == NULL) {
    return root;
  }

  root->height = 1 + max(height(root->left), height(root->right));

  int balance = getBalance(root);

  // Left Left Case
  if (balance > 1 && getBalance(root->left) >= 0) {
    return rightRotate(root);
  }

  // Left Right Case
  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  // Right Right Case
  if (balance < -1 && getBalance(root->right) <= 0) {
    return leftRotate(root);
  }

  // Right Left Case
  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

struct Node *searchNode(struct Node *root, char *gameTitle) {
  if (root == NULL || strcmp(root->gameTitle, gameTitle) == 0) {
    return root;
  }

  if (strcmp(root->gameTitle, gameTitle) < 0) {
    return searchNode(root->left, gameTitle);
  } else {
    return searchNode(root->right, gameTitle);
  }
}

void inOrder(struct Node *root) {
  if (root != NULL) {
    inOrder(root->left);
    printf("%-15s|%-10s|%-10s\n", root->gameTitle, root->gameGenre,
           root->gameStock);
    inOrder(root->right);
  }
}

int main() {
  char gameTitle[50], gameGenre[50], gameStock[50];
  int choice;
  struct Node *root = NULL;

  do {
    printf("Bluejack GShop\n");
    printf("==============\n");
    printf("1. Insert Game\n");
    printf("2. View Games\n");
    printf("3. Update Stock\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      printf("Enter game title [5-25 characters, unique]: ");
      scanf(" %[^\n]", gameTitle);
      printf("Enter game genre [Action|RPG|Adventure|Card Game]: ");
      scanf(" %[^\n]", gameGenre);
      printf("Enter game stock [>=1]: ");
      scanf(" %[^\n]", gameStock);
      root = insertNode(root, gameTitle, gameGenre, gameStock);
      break;

    case 2:
      printf("Game Title    |Genre     |Stock    \n");
      printf("------------------------------------\n");
      if (root != NULL) {
        inOrder(root);
      } else {
        printf("Warehouse is Empty!\n");
      }
      break;

    case 3:
      printf("Enter game title: ");
      scanf(" %[^\n]", gameTitle);
      struct Node *findNode = searchNode(root, gameTitle);
      if (findNode != NULL) {
        printf("Current Stock: %s\n", findNode->gameStock);
        printf("Enter update type [add|remove] (case sensitive): ");
        scanf(" %[^\n]", gameGenre);

        if (strcmp(gameGenre, "add") == 0) {
          printf("Enter stock to add [1-50]: ");
          scanf(" %[^\n]", gameStock);
          int stock = atoi(findNode->gameStock) + atoi(gameStock);
          sprintf(findNode->gameStock, "%d", stock);
          printf("%s stock is updated in the warehouse!\n", gameTitle);
        } else if (strcmp(gameGenre, "remove") == 0) {
          printf("Enter stock to remove [1-50]: ");
          scanf(" %[^\n]", gameStock);
          int stock = atoi(findNode->gameStock) - atoi(gameStock);
          if (stock >= 0) {
            sprintf(findNode->gameStock, "%d", stock);
            printf("%s stock is removed from the warehouse!\n", gameTitle);
          } else {
            printf("Invalid stock value!\n");
          }
        } else {
          printf("Invalid update type!\n");
        }
      } else {
        printf("Game not found in the warehouse!\n");
      }
      break;

    case 4:
      printf("Exiting program...\n");
      break;

    default:
      printf("Invalid choice!\n");
      break;
    }
  } while (choice != 4);

  return 0;
}

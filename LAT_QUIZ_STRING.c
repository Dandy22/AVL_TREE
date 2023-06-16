#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Node {
  char gameTitle[50], gameGenre[50], gameStock[50];
  int height;
  struct Node *left, *right;
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

struct Node *minVal(struct Node *root) {
  struct Node *curr = root->left;
  while (root->left != NULL)
    curr = curr->left;
  return curr;
}

struct Node *leftRotate(struct Node *root) {
  struct Node *x = root->right;
  struct Node *sub = x->left;

  x->left = root;
  root->right = sub;

  root->height = max(height(root->left), height(root->right));
  x->height = max(height(root->left), height(root->right));
  return x;
}

struct Node *rightRotate(struct Node *root) {
  struct Node *x = root->left;
  struct Node *sub = x->right;

  x->right = root;
  root->left = sub;

  root->height = max(height(root->left), height(root->right));
  x->height = max(height(root->left), height(root->right));
  return x;
}

struct Node *insertNode(struct Node *root, char *gameTitle, char *gameGenre,
                        char *gameStock) {
  if (root == NULL) {
    return (newNode(gameTitle, gameGenre, gameStock));
  }

  if (strcmp(root->gameTitle, gameTitle) < 0) {
    root->left = insertNode(root->left, gameTitle, gameGenre, gameStock);
  } else if (strcmp(root->gameTitle, gameTitle) > 0) {
    root->right = insertNode(root->right, gameTitle, gameGenre, gameStock);
  } else {
    return root;
  }

  root->height = 1 + max(height(root->left), height(root->right));
  int balance = getBalance(root);

  // LL CASE
  if (balance > 1 && strcmp(root->left->gameTitle, gameTitle) < 0) {
    return rightRotate(root);
  }

  // LR CASE
  if (balance > 1 && strcmp(root->left->gameTitle, gameTitle) > 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  // RR CASE
  if (balance < -1 && strcmp(root->right->gameTitle, gameTitle) > 0) {
    return leftRotate(root);
  }

  // RL CASE
  if (balance < -1 && strcmp(root->right->gameTitle, gameTitle) < 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }
  return root;
}

struct Node *deleteNode(struct Node *root, char *gameTitle) {
  if (root == NULL) {
    return NULL;
  }
  if (strcmp(root->gameTitle, gameTitle) < 0) {
    root->left = deleteNode(root->left, gameTitle);
  } else if (strcmp(root->gameTitle, gameTitle) > 0) {
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
      struct Node *pred = minVal(root);
      strcpy(root->gameTitle, pred->gameTitle);
      strcpy(root->gameGenre, pred->gameGenre);
      strcpy(root->gameStock, pred->gameStock);

      root->right = deleteNode(root->right, gameTitle);
    }
  }
  if (root == NULL) {
    return NULL;
  }
  root->height = 1 + max(height(root->left), height(root->right));
  int balance = 1 + max(height(root->left), height(root->right));

  if (balance > 1 && strcmp(root->left->gameTitle, gameTitle) < 0) {
    return rightRotate(root);
  }
  if (balance > 1 && strcmp(root->left->gameTitle, gameTitle) > 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }
  if (balance < -1 && strcmp(root->right->gameTitle, gameTitle) > 0) {
    return leftRotate(root);
  }
  if (balance < -1 && strcmp(root->right->gameTitle, gameTitle) < 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }
  return root;
}

void inOrder(struct Node *root) {
  if (root != NULL) {
    inOrder(root->left);
    printf("%-15s | %-15s | %-15s\n", root->gameTitle, root->gameGenre,
           root->gameStock);
    inOrder(root->right);
  }
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

int main() {

  struct Node *root = NULL;
  char gameTitle[50], gameGenre[50], gameStock[50];
  int pilih;
  do {

    printf("Bluejack GShop\n");
    printf("==============\n");
    printf("1. Insert Game\n");
    printf("2. View Game\n");
    printf("3. Update Stock\n");
    printf("4. Exit\n");
    scanf("%d", &pilih);

    switch (pilih) {
    case 1: {
      do {
        printf("Input game title[5-25][unique]: ");
        scanf(" %[^\n]", gameTitle);
      } while (strlen(gameTitle) < 5 || strlen(gameTitle) > 25);

      printf("Input game type[Action|RPG|Adventure|Card Game]: ");
      scanf(" %[^\n]", gameGenre);
      do {
        printf("Input game stock[ >=1]: ");
        scanf(" %[^\n]", gameStock);
      } while (atoi(gameStock) < 1);

      root = insertNode(root, gameTitle, gameGenre, gameStock);
      break;
    }

    case 2: {
      if (root != NULL) {
        inOrder(root);
      } else {
        printf("Warehouse is Empty !\n");
      }
      break;
    }

    case 3: {
      printf("Input game title: ");
      scanf(" %[^\n]", gameTitle);
      struct Node *findRoot = searchNode(root, gameTitle);
      if (findRoot != NULL) {
        printf("Current stock: %s\n", gameStock);
        printf("Input update type[add|remove][case insensitive]: ");
        scanf(" %[^\n]", gameGenre);
        if (strcmp(gameGenre, "add") == 0) {
          do {
            printf("Input stock to update[1-50]: ");
            scanf(" %[^\n]", gameStock);
          } while (strlen(gameStock) < 1 || strlen(gameStock) > 50);
          int stock = atoi(findRoot->gameStock) + atoi(gameStock);
          sprintf(findRoot->gameStock, "%d", stock);
          printf("Data updated successfully !\n");
          printf("%s is added from the warehouse !\n", gameTitle);
        } else if (strcmp(gameGenre, "remove") == 0) {
          do {
            printf("Input stock to remove[1-50]: ");
            scanf(" %[^\n]", gameStock);
          } while (strlen(gameStock) < 1 || strlen(gameStock) > 50);
          int stock = atoi(findRoot->gameStock) - atoi(gameStock);
          sprintf(findRoot->gameStock, "%d", stock);
          printf("Data updated Sucessfully !\n");
          printf("%s is removed from the warehouse !\n", gameTitle);
        }
      } else {
        printf("NO DATA\n");
      }
      break;
    }

    case 4: {
      printf("Exiting...\n");
      exit(0);
    }
    default: {
      printf("Wrong Input\n");
      exit(0);
    }
    }
  } while (pilih != 5);
  return 0;
}

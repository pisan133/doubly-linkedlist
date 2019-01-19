/**
 * Linked list implementation
 *
 * For simplicity, the Linked List starts with 0 as first element
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// self-referential Node structure
struct Node {
  // each Node contains an int
  int data;
  // pointer to next node
  struct Node *nextPtr;
  // pointer to previous node
  struct Node *prevPtr;
};

// synonym for struct Node
typedef struct Node Node;

// get an int from keyboard
int getInt(const char *prompt) {
  char *cstr;
  char buffer[256];
  printf("%s", prompt);
  cstr = fgets(buffer, 255, stdin);
  return atoi(cstr);
}

// print all nodes except special dummy headtail node
void print(Node *headtail) {
  if (headtail->nextPtr == NULL) {
    printf("EMPTY\n");
    return;
  } else {
    printf("First: %d, Last: %d\n",
           headtail->nextPtr->data, headtail->prevPtr->data);
  }
  // print the first one
  Node *curr = headtail->nextPtr;
  printf("%d", curr->data);
  // print rest with arrows before them
  curr = curr->nextPtr;
  while (curr != NULL) {
    printf(" <--> %d", curr->data);
    curr = curr->nextPtr;
  }
  printf("\n");
}

// create a new node on the HEAP
Node *makeNode(int value) {
  Node *n = (Node *) malloc(sizeof(Node));
  n->data = value;
  n->nextPtr = NULL;
  n->prevPtr = NULL;
  return n;
}

// add a new item to the end of the list
void addToEnd(Node *headtail, int value) {
  Node *newNode = makeNode(value);
  if (headtail->nextPtr == NULL) {
    // if empty
    headtail->nextPtr = newNode;
    headtail->prevPtr = newNode;
  } else {
    // add to end
    Node *lastNode = headtail->prevPtr;
    lastNode->nextPtr = newNode;
    newNode->prevPtr = lastNode;
    headtail->prevPtr = newNode;
  }
}

// add the value given before other
// if other is not in the list add value at the end
void addBefore(Node *headtail, int value, int other) {
  Node *curr = headtail->nextPtr;
  while (curr != NULL && curr->data != other) {
    curr = curr->nextPtr;
  }
  if (curr == NULL) {
    // could not find, add to end
    addToEnd(headtail, value);
  } else {
    // call current node c, the new node b, order nodes as a-b-c
    Node *c = curr;
    Node *b = makeNode(value);
    Node *a = c->prevPtr;
    c->prevPtr = b;
    b->nextPtr = c;
    b->prevPtr = a;
    if (a == NULL) {
      // c must have been the first node
      headtail->nextPtr = b;
    } else {
      a->nextPtr = b;
    }
  }
}

// add the value given after other
// if other is not in the list add value at the end
void addAfter(Node *headtail, int value, int other) {
  Node *curr = headtail->nextPtr;
  while (curr != NULL && curr->data != other) {
    curr = curr->nextPtr;
  }
  if (curr == NULL) {
    // could not find, add to end
    addToEnd(headtail, value);
  } else {
    // call current node a, the new node b, order nodes as a-b-c
    Node *a = curr;
    Node *b = makeNode(value);
    Node *c = a->nextPtr;
    a->nextPtr = b;
    b->nextPtr = c;
    b->prevPtr = a;
    if (c == NULL) {
      // a must have been the last node
      headtail->prevPtr = b;
    } else {
      c->prevPtr = b;
    }
  }
}

// add the value given in sorted order
void addSorted(Node *headtail, int value) {
  Node *curr = headtail->nextPtr;
  if (curr == NULL) {
    // list is empty
    addToEnd(headtail, value);
  } else {
    // find curr that has a bigger value
    while (curr != NULL && curr->data < value) {
      curr = curr->nextPtr;
    }
    // if larger than all nodes
    if (curr == NULL) {
      addToEnd(headtail, value);
    } else {
      addBefore(headtail, value, curr->data);
    }
  }
}

// return true if value is in the linked list
bool contains(Node *headtail, int value) {
  Node *curr = headtail->nextPtr;
  while (curr != NULL) {
    if (curr->data == value)
      return true;
    curr = curr->nextPtr;
  }
  return false;
}

// remove the node from the list
// nodes initially ordered a-b-c, removing b
void removeSingleNode(Node *b) {
  Node *a = b->prevPtr;
  Node *c = b->nextPtr;
  if (a != NULL)
    a->nextPtr = c;
  if (c != NULL)
    c->prevPtr = a;
  free(b);
}

// remove the node with the given value
// if there are multiple nodes, remove the first one
// if successfully removed, return true
// if value not found in the linked list, return false
// Once node is removed, it must be free'd
bool removeNode(Node *headtail, int value) {
  Node *curr = headtail->nextPtr;
  while (curr != NULL && curr->data != value) {
    curr = curr->nextPtr;
  }
  if (curr == NULL)
    return false;
  // if first or last node, set headtail
  if (headtail->nextPtr == curr)
    headtail->nextPtr = curr->nextPtr;
  if (headtail->prevPtr == curr)
    headtail->prevPtr = curr->prevPtr;
  // call remove single
  removeSingleNode(curr);
  return true;
}

// free all nodes
void freeAll(Node *curr) {
  while (curr != NULL) {
    Node *todelete = curr;
    curr = curr->nextPtr;
    printf("Deleting %d\n", todelete->data);
    free(todelete);
  }
}

/**
 * Run several tests, expected output is:
 * Starting list
 * First: 20, Last: 60
 * 20 <--> 30 <--> 40 <--> 50 <--> 60
 * Adding 25 to be before 30
 * First: 20, Last: 60
 * 20 <--> 25 <--> 30 <--> 40 <--> 50 <--> 60
 * Adding 45 to be after 40
 * First: 20, Last: 60
 * 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60
 * Adding 80 to be before 1000
 * First: 20, Last: 80
 * 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80
 * Adding 100 to be after 1000
 * First: 20, Last: 100
 * 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Adding 10 to be before 20
 * First: 10, Last: 100
 * 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Adding sorted 5
 * First: 5, Last: 100
 * 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Adding sorted 31
 * First: 5, Last: 100
 * 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 31 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Adding sorted 120
 * First: 5, Last: 120
 * 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 31 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> 120
 * Remove node 120
 * First: 5, Last: 100
 * 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 31 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 31
 * First: 5, Last: 100
 * 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 5
 * First: 10, Last: 100
 * 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 10
 * First: 20, Last: 100
 * 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 20
 * First: 25, Last: 100
 * 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 25
 * First: 30, Last: 100
 * 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 30
 * First: 40, Last: 100
 * 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 40
 * First: 45, Last: 100
 * 45 <--> 50 <--> 60 <--> 80 <--> 100
 * Remove node 45
 * First: 50, Last: 100
 * 50 <--> 60 <--> 80 <--> 100
 * Remove node 50
 * First: 60, Last: 100
 * 60 <--> 80 <--> 100
 * Remove node 60
 * First: 80, Last: 100
 * 80 <--> 100
 * Remove node 80
 * First: 100, Last: 100
 * 100
 * Remove node 100
 * EMPTY
 * Deleting 0
 */
void testAddBeforeAfterSorted() {
  Node *headtail = makeNode(0);

  for (int i = 20; i < 70; i += 10) {
    addToEnd(headtail, i);
  }
  printf("Starting list\n");
  print(headtail);

  printf("Adding 25 to be before 30\n");
  addBefore(headtail, 25, 30);
  print(headtail);

  printf("Adding 45 to be after 40\n");
  addAfter(headtail, 45, 40);
  print(headtail);

  printf("Adding 80 to be before 1000\n");
  addBefore(headtail, 80, 1000);
  print(headtail);

  printf("Adding 100 to be after 1000\n");
  addAfter(headtail, 100, 1000);
  print(headtail);

  printf("Adding 10 to be before 20\n");
  addBefore(headtail, 10, 20);
  print(headtail);

  printf("Adding sorted 5\n");
  addSorted(headtail, 5);
  print(headtail);

  printf("Adding sorted 31\n");
  addSorted(headtail, 31);
  print(headtail);

  printf("Adding sorted 120\n");
  addSorted(headtail, 120);
  print(headtail);

  printf("Remove node 120\n");
  removeNode(headtail, 120);
  print(headtail);

  printf("Remove node 31\n");
  removeNode(headtail, 31);
  print(headtail);

  printf("Remove node 5\n");
  removeNode(headtail, 5);
  print(headtail);

  while (headtail->nextPtr != NULL) {
    printf("Remove node %d\n", headtail->nextPtr->data);
    removeNode(headtail, headtail->nextPtr->data);
    print(headtail);
  }

  freeAll(headtail);
}

// get new values from user and add them to the list
void testInteractive() {
  // head is set to a node instead of NULL for simplicity
  Node *headtail = makeNode(0);
  int input = getInt("Enter a number (-1 to end): ");
  while (input != -1) {
    addToEnd(headtail, input);
    print(headtail);
    input = getInt("Enter a number (-1 to end): ");
  }
  freeAll(headtail);
}


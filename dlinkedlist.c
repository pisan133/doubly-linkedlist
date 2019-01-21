/**
 * Linked list implementation
 *
 * For simplicity, the Linked List starts with 0 as first element
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

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

// print all nodes (including dummy head)
void print(Node *start) {
  // guaranteed to have at least dummy header node
  assert(start);
  Node *curr = start;
  while (curr != NULL) {
    printf("%d <--> ", curr->data);
    curr = curr->nextPtr;
  }
  printf("NULL\n");
}

// print all nodes from back (including dummy head)
void printBackward(Node *start) {
  // TODO
}

// create a new node on the HEAP and return it
Node *makeNode(int value) {
  Node *n = (Node *) malloc(sizeof(Node));
  n->data = value;
  n->nextPtr = NULL;
  n->prevPtr = NULL;
  return n;
}

// add a new item to the end of the list
void addToEnd(Node *start, int value) {
  Node *newNode = makeNode(value);
  Node *curr = start;
  while (curr->nextPtr != NULL) {
    curr = curr->nextPtr;
  }
  curr->nextPtr = newNode;
  newNode->prevPtr = curr;
}

// add the value given before other
// if other is not in the list add value at the end
void addBefore(Node *start, int value, int other) {
  Node *curr = start;
  while (curr->nextPtr != NULL && curr->nextPtr->data != other) {
    curr = curr->nextPtr;
  }
  if (curr->nextPtr == NULL) {
    // could not find, add to end
    Node *newNode = makeNode(value);
    curr->nextPtr = newNode;
    newNode->prevPtr = curr;
  } else {
    assert(curr->nextPtr->data == other);
    // call current node a, the new node b, the one after current c
    // turn a-c into a-b-c
    Node *a = curr;
    Node *b = makeNode(value);
    Node *c = a->nextPtr;
    a->nextPtr = b;
    b->prevPtr = a;
    b->nextPtr = c;
    c->prevPtr = b;
  }
}

// add the value given after other
// if other is not in the list add value at the end
void addAfter(Node *start, int value, int other) {
  Node *curr = start;
  while (curr->nextPtr != NULL && curr->nextPtr->data != other) {
    curr = curr->nextPtr;
  }
  if (curr->nextPtr == NULL) {
    // could not find, add to end
    Node *newNode = makeNode(value);
    curr->nextPtr = newNode;
    newNode->prevPtr = curr;
  } else {
    assert(curr->nextPtr->data == other);
    // call the node after curr a, the one after that c
    // call the new node to be created b. Make a-b-c
    Node *a = curr->nextPtr;
    Node *b = makeNode(value);
    Node *c = a->nextPtr;  // could be NULL
    a->nextPtr = b;
    b->prevPtr = a;
    b->nextPtr = c;
    if (c != NULL) {
      b->nextPtr = c;
      c->prevPtr = b;
    }
  }
}

// add the value given in sorted order
void addSorted(Node *start, int value) {
  Node *curr = start;
  while (curr->nextPtr != NULL && curr->nextPtr->data < value) {
    curr = curr->nextPtr;
  }
  if (curr->nextPtr == NULL) {
    // adding to end
    Node *newNode = makeNode(value);
    curr->nextPtr = newNode;
    newNode->prevPtr = curr;
  } else {
    // adding between curr and curr->nextptr
    Node *a = curr;
    Node *b = makeNode(value);
    Node *c = a->nextPtr;
    a->nextPtr = b;
    b->prevPtr = a;
    b->nextPtr = c;
    c->prevPtr = b;
  }
}

// return true if value is in the linked list
bool contains(Node *start, int value) {
  Node *curr = start->nextPtr;
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
bool removeNode(Node *start, int value) {
  Node *curr = start->nextPtr;
  while (curr != NULL) {
    if (curr->data == value) {
      removeSingleNode(curr);
      return true;
    }
    curr = curr->nextPtr;
  }
  return false;
}

// free all nodes
void freeAll(Node *start) {
  while (start != NULL) {
    Node *todelete = start;
    start = start->nextPtr;
    printf("Deleting %d\n", todelete->data);
    free(todelete);
  }
}

/**
 * Run several tests, expected output is:
 * Contains 40? Yes
 * Starting list
 * 0 <--> 20 <--> 30 <--> 40 <--> 50 <--> 60 <--> NULL
 * Adding 25 to be before 30
 * 0 <--> 20 <--> 25 <--> 30 <--> 40 <--> 50 <--> 60 <--> NULL
 * Adding 45 to be after 40
 * 0 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> NULL
 * Adding 80 to be before 1000
 * 0 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> NULL
 * Adding 100 to be after 1000
 * 0 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Adding 10 to be before 20
 * 0 <--> 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Adding sorted 5
 * 0 <--> 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Adding sorted 31
 * 0 <--> 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 31 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Adding sorted 120
 * 0 <--> 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 31 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> 120 <--> NULL
 * Remove node 120
 * 0 <--> 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 31 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 31
 * 0 <--> 5 <--> 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 5
 * 0 <--> 10 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 10
 * 0 <--> 20 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 20
 * 0 <--> 25 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 25
 * 0 <--> 30 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 30
 * 0 <--> 40 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 40
 * 0 <--> 45 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 45
 * 0 <--> 50 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 50
 * 0 <--> 60 <--> 80 <--> 100 <--> NULL
 * Remove node 60
 * 0 <--> 80 <--> 100 <--> NULL
 * Remove node 80
 * 0 <--> 100 <--> NULL
 * Remove node 100
 * 0 <--> NULL
 * Deleting 0
 */
void testAddBeforeAfterSorted() {
  Node *head = makeNode(0);

  for (int i = 20; i < 70; i += 10) {
    addToEnd(head, i);
  }
  printf("Contains 40? %s\n", contains(head, 40) ? "Yes" : "No");
  printf("Starting list\n");
  print(head);

  printf("Adding 25 to be before 30\n");
  addBefore(head, 25, 30);
  print(head);

  printf("Adding 45 to be after 40\n");
  addAfter(head, 45, 40);
  print(head);

  printf("Adding 80 to be before 1000\n");
  addBefore(head, 80, 1000);
  print(head);

  printf("Adding 100 to be after 1000\n");
  addAfter(head, 100, 1000);
  print(head);

  printf("Adding 10 to be before 20\n");
  addBefore(head, 10, 20);
  print(head);

  printf("Adding sorted 5\n");
  addSorted(head, 5);
  print(head);

  printf("Adding sorted 31\n");
  addSorted(head, 31);
  print(head);

  printf("Adding sorted 120\n");
  addSorted(head, 120);
  print(head);

  printf("Remove node 120\n");
  removeNode(head, 120);
  print(head);

  printf("Remove node 31\n");
  removeNode(head, 31);
  print(head);

  printf("Remove node 5\n");
  removeNode(head, 5);
  print(head);

  while (head->nextPtr != NULL) {
    printf("Remove node %d\n", head->nextPtr->data);
    removeNode(head, head->nextPtr->data);
    print(head);
  }

  freeAll(head);
}

// return largest node (skip dummy head)
// return NULL if empty
Node *findLargest(Node *start) {
  Node *largest = NULL;
  // TODO
  return largest;
}

// return smallest node (skip dummy head)
// return NULL if empty
Node *findSmallest(Node *start) {
  Node *smallest = NULL;
  // TODO
  return smallest;
}

// swapping place of nodes
// Name the nodes as follows: a-big-c and k-little-m
// Want a-little-c and k-big-m
// a, c, k and m can be NULL so have to check
void swapNodes(Node *big, Node *little) {
  // TODO
}

/**
 * Run several tests, expected output is something like below
 * Uses random numbers, so output will be different for each
 * Forward:
 * 0 <--> 7 <--> 49 <--> 73 <--> 58 <--> 30 <--> 72 <--> 44 <--> NULL
 * Backward:
 * NULL <--> 44 <--> 72 <--> 30 <--> 58 <--> 73 <--> 49 <--> 7 <--> 0
 * Swapping largest: 73 with smallest: 7
 * 0 <--> 73 <--> 49 <--> 7 <--> 58 <--> 30 <--> 72 <--> 44 <--> NULL
 * Removing largest: 73
 * 0 <--> 49 <--> 7 <--> 58 <--> 30 <--> 72 <--> 44 <--> NULL
 * Removing largest: 72
 * 0 <--> 49 <--> 7 <--> 58 <--> 30 <--> 44 <--> NULL
 * Removing largest: 58
 * 0 <--> 49 <--> 7 <--> 30 <--> 44 <--> NULL
 * Removing largest: 49
 * 0 <--> 7 <--> 30 <--> 44 <--> NULL
 * Removing largest: 44
 * 0 <--> 7 <--> 30 <--> NULL
 * Removing largest: 30
 * 0 <--> 7 <--> NULL
 * Removing largest: 7
 * 0 <--> NULL
 */
void testLargestSmallestSwap() {
  // Intializes random number generator
  time_t t;
  srand((unsigned) time(&t));
  Node *head = makeNode(0);

  for (int i = 0; i < 7; ++i) {
    int rnd = rand() % 100;
    addToEnd(head, rnd);
  }
  printf("Forward:\n");
  print(head);
  printf("Backward:\n");
  printBackward(head);
  Node *large = findLargest(head);
  Node *small = findSmallest(head);
  printf("Swapping largest: %d with ", large->data);
  printf("smallest: %d\n", small->data);
  swapNodes(large, small);
  print(head);
  // remove them one by one
  large = findLargest(head);
  while (large != NULL) {
    printf("Removing largest: %d\n", large->data);
    removeSingleNode(large);
    print(head);
    large = findLargest(head);
  }
  freeAll(head);
}

// get new values from user and add them to the list
void testInteractive() {
  // head is set to a node instead of NULL for simplicity
  Node *head = makeNode(0);
  int input = getInt("Enter a number (-1 to end): ");
  while (input != -1) {
    addToEnd(head, input);
    print(head);
    input = getInt("Enter a number (-1 to end): ");
  }
  freeAll(head);
}


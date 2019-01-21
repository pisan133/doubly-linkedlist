/**
 * Driver for testing Double Linked List
 *
 * Yusuf Pisan
 */

#include <stdio.h>
#include <stdlib.h>

// forward declaration, implemented in linkedlist.c
void testInteractive();
void testAddBeforeAfterSorted();
void testLargestSmallestSwap();

int main() {
  // testInteractive();
  // testAddBeforeAfterSorted();
  testLargestSmallestSwap();
  printf("Done!\n");
  return 0;
}
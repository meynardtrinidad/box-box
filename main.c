#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generate_random_number();

void generate_area(int x, int y);

int main() {
  srand(time(NULL));
  int x = generate_random_number(), y = generate_random_number();
  generate_area(x, y);

  while (1) {
    // TODO: REPL
    break;
  }
}

int generate_random_number() { return MIN + (rand() % (MAX - MIN + 1)); }

// Generate an x by y matrix where the outer
// edges are filled with '#' and that everything
// should be inside the box.
void generate_area(int x, int y) {
  x = 5;
  y = 5;
  int matrix[25] = {
      0}; // FIXME: Use `malloc` to allocate the calculated memory.
  int len = sizeof(matrix) / sizeof(int);

  int rowCount = 1;
  for (int i = 0; i < len; i++) {
    int row = i / y;
    int col = i % x;

    if (row == 0 || row == y - 1 || col == 0 || col == x - 1) {
      printf("%c", WALL);
    } else {
      printf("%c", SPACE);
    }

    if (col == x - 1) {
      rowCount++;
      printf("\n");
    }
  }
}

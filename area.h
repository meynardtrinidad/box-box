#ifndef AREA_H
#define AREA_H

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

// Generate an x by y matrix where the outer
// edges are filled with '#' and that everything
// should be inside the box.
void *generate_area(int x, int y) {
  int len = x * y;
  // int *area = malloc(len * sizeof(int));
  int *area;

  if ((area = (int *)malloc(len * sizeof(int))) == NULL) {
    return NULL;
  }

  for (int i = 0; i < len; i++) {
    int row = i / x;
    int col = i % x;

    if (row == 0 || row == y - 1 || col == 0 || col == x - 1) {
      area[i] = WALL;
    } else {
      area[i] = SPACE;
    }
  }

  return area;
}

void display_area(int *area, int len, int x, int y) {
  system("clear"); // FIX: This forces to redraw the whole screen instead of
                   // performing partial redraws.

  for (int i = 0; i < len; i++) {
    int col = i % x;

    switch (area[i]) {
    case WALL:
      printf("%c", WALL);
      break;
    case SPACE:
      printf("%c", SPACE);
      break;
    case PLAYER:
      printf("%c", PLAYER);
      break;
    case BOX:
      printf("%c", BOX);
      break;
    }

    if (col == x - 1) {
      printf("\n");
    }
  }
}

int is_inside(int point, int len, int x, int y) {
  int row = point / x;
  int col = point % x;

  if (row == 0 || row == y - 1 || col == 0 || col == x - 1) {
    return 0;
  }

  return 1;
}

#endif

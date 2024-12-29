#ifndef CONTROLS_H
#define CONTROLS_H

#include "area.h"
#include "constants.h"
#include <unistd.h>

int handle_move_direction(int dir, int pos, int step) {
  int new_pos = pos;

  switch (dir) {
  case 'A':
    new_pos = new_pos - step;
    break;
  case 'B':
    new_pos = new_pos + step;
    break;
  case 'C':
    new_pos = new_pos + 1;
    break;
  case 'D':
    new_pos = new_pos - 1;
    break;
  }

  return new_pos;
}

void handle_move_player(char c, int *pos, int *area, int len, int x, int y) {
  int start_pos = *pos;

  if (c == 27) {
    char seq[2];
    char *p = seq;

    read(STDIN_FILENO, &seq[0], 1);
    read(STDIN_FILENO, &seq[1], 1);

    int new_pos = start_pos;

    if (seq[0] == '[') {
      new_pos = handle_move_direction(seq[1], start_pos, x);

      if (area[new_pos] == WALL) {
        return;
      }

      if (area[new_pos] == BOX) {
        int new_box_pos = handle_move_direction(seq[1], new_pos, x);

        if (area[new_box_pos] == WALL) {
          return;
        }

        area[new_box_pos] = BOX;
      }

      area[start_pos] = SPACE;
      area[new_pos] = PLAYER;
      *pos = new_pos;
    }
  }
}

#endif

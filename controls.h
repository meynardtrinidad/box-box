#ifndef CONTROLS_H
#define CONTROLS_H

#include "area.h"
#include "constants.h"
#include <unistd.h>

void handle_move(char c, int *pos, int *area, int len, int x, int y) {
  int start_pos = *pos;

  // FIX: Prevent any object from going out-of-bounds
  if (c == 27) {
    char seq[2];
    char *p = seq;

    read(STDIN_FILENO, &seq[0], 1);
    read(STDIN_FILENO, &seq[1], 1);

    int new_pos = start_pos;

    if (seq[0] == '[') {
      switch (seq[1]) {
      case 'A':
        new_pos = start_pos - x;
        break;
      case 'B':
        new_pos = start_pos + x;
        break;
      case 'C':
        new_pos = start_pos + 1;
        break;
      case 'D':
        new_pos = start_pos - 1;
        break;
      }

      if (!is_inside(new_pos, len, x, y)) {
        return;
      }

      area[start_pos] = SPACE;
      area[new_pos] = PLAYER;
      *pos = new_pos;
    }
  }
}

#endif

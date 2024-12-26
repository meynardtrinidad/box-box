#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

struct termios termios_t;

void enable_raw_mode(struct termios *t);
void disable_raw_mode(struct termios *t);

int generate_random_number();

void *generate_area(int x, int y);
void display_area(int *area, int len, int x, int y);

int main() {
  srand(time(NULL));
  int x = generate_random_number(), y = generate_random_number();
  int len = x * y;

  int *area;
  if ((area = generate_area(x, y)) == NULL) {
    printf("Error generating area of x = %d and y = %d.", x, y);
    return 1;
  }

  display_area(area, len, x, y);
  int start_pos = len * .75; // FIX: Must be randomly generated inside the area.

  enable_raw_mode(&termios_t);

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    system("clear");

    if (c == 'q') // Exit on 'q' press
      break;

    if (c == 27) {
      char seq[2];
      char *p = seq;

      read(STDIN_FILENO, &seq[0], 1);
      read(STDIN_FILENO, &seq[1], 1);

      int new_pos = start_pos;

      if (seq[0] == '[') {
        switch (seq[1]) {
        case 'A':
          printf("Up arrow.\n");
          area[start_pos] = SPACE;
          new_pos = start_pos - x;
          area[new_pos] = PLAYER;
          start_pos = new_pos;
          break;
        case 'B':
          printf("Down arrow.\n");
          area[start_pos] = SPACE;
          new_pos = start_pos + x;
          area[new_pos] = PLAYER;
          start_pos = new_pos;
          break;
        case 'C':
          printf("Right arrow pressed.\n");
          area[start_pos] = SPACE;
          new_pos = start_pos + 1;
          area[new_pos] = PLAYER;
          start_pos = new_pos;
          break;
        case 'D':
          printf("Left arrow pressed.\n");
          area[start_pos] = SPACE;
          new_pos = start_pos - 1;
          area[new_pos] = PLAYER;
          start_pos = new_pos;
          break;
        }
      }
    }

    display_area(area, len, x, y);
  }

  disable_raw_mode(&termios_t);

  free(area);
  return 0;
}

int generate_random_number() { return MIN + (rand() % (MAX - MIN + 1)); }

// Generate an x by y matrix where the outer
// edges are filled with '#' and that everything
// should be inside the box.
void *generate_area(int x, int y) {
  int len = x * y;
  int *area = malloc(len * sizeof(int));
  int pos = len * .75; // FIX: Must be random generated inside the area.

  for (int i = 0; i < len; i++) {
    int row = i / x;
    int col = i % x;

    if (i == pos) {
      printf("i = %d, pos = %d, len = %d\n", i, pos, len);
      area[i] = PLAYER;
      continue;
    }

    if (row == 0 || row == y - 1 || col == 0 || col == x - 1) {
      area[i] = WALL;
    } else {
      area[i] = SPACE;
    }
  }

  return area;
}

void display_area(int *area, int len, int x, int y) {
  for (int i = 0; i < len; i++) {
    int col = i % x;

    switch (area[i]) {
    case 35:
      printf("%c", WALL);
      break;
    case 46:
      printf("%c", SPACE);
      break;
    case 64:
      printf("%c", PLAYER);
      break;
    case 79:
      printf("%c", BOX);
      break;
    }

    if (col == x - 1) {
      printf("\n");
    }
  }
}

void enable_raw_mode(struct termios *t) {
  tcgetattr(STDIN_FILENO, t);
  struct termios _termios; // Creates new termios to prevent modifying the
                           // original termios.
  _termios = *t;
  _termios.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &_termios);
}

void disable_raw_mode(struct termios *t) {
  tcsetattr(STDIN_FILENO, TCSANOW, t);
}

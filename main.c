#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

struct termios termios_t;

void enable_raw_mode(struct termios *t);
void disable_raw_mode(struct termios *t);

int generate_random_number(int min, int max);

void *generate_area(int x, int y);
void display_area(int *area, int len, int x, int y);
int is_inside(int point, int len, int x, int y);

int main() {
  srand(time(NULL));
  int x = generate_random_number(MIN, MAX),
      y = generate_random_number(MIN, MAX);
  int len = x * y;

  int *area;
  if ((area = generate_area(x, y)) == NULL) {
    printf("Error generating area of x = %d and y = %d.", x, y);
    return 1;
  }

  int start_pos = generate_random_number(MIN, len);
  while (is_inside(start_pos, len, x, y) == 0) {
    start_pos = generate_random_number(MIN, len);
  }
  area[start_pos] = PLAYER;
  display_area(area, len, x, y);

  enable_raw_mode(&termios_t);

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
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

        area[start_pos] = SPACE;
        area[new_pos] = PLAYER;
        start_pos = new_pos;
      }
    }

    display_area(area, len, x, y);
  }

  disable_raw_mode(&termios_t);

  free(area);
  return 0;
}

int generate_random_number(int min, int max) {
  return min + (rand() % (max - min + 1));
}

int is_inside(int point, int len, int x, int y) {
  int row = point / x;
  int col = point % x;

  if (row == 0 || row == y - 1 || col == 0 || col == x - 1) {
    return 0;
  }

  return 1;
}

// Generate an x by y matrix where the outer
// edges are filled with '#' and that everything
// should be inside the box.
void *generate_area(int x, int y) {
  int len = x * y;
  int *area = malloc(len * sizeof(int));

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

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

struct termios termios_t;

void enable_raw_mode(struct termios *t);
void disable_raw_mode(struct termios *t);
void start_repl();

int generate_random_number();

void *generate_area(int x, int y);

int main() {
  srand(time(NULL));
  int x = generate_random_number(), y = generate_random_number();

  int *area;
  if ((area = generate_area(x, y)) == NULL) {
    printf("Error generating area of x = %d and y = %d.", x, y);
    return 1;
  }

  enable_raw_mode(&termios_t);
  start_repl();
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

  for (int i = 0; i < len; i++) {
    int row = i / x;
    int col = i % x;

    if (row == 0 || row == y - 1 || col == 0 || col == x - 1) {
      printf("%c", WALL);
    } else {
      printf("%c", SPACE);
    }

    if (col == x - 1) {
      printf("\n");
    }
  }

  return area;
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

void start_repl() {
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == 'q') // Exit on 'q' press
      break;

    if (c == 27) {
      char seq[2];
      char *p = seq;

      read(STDIN_FILENO, &seq[0], 1);
      read(STDIN_FILENO, &seq[1], 1);

      if (seq[0] == '[') {
        switch (seq[1]) {
        case 'A':
          printf("Up arrow pressed.\n");
          break;
        case 'B':
          printf("Down arrow pressed.\n");
          break;
        case 'C':
          printf("Left arrow pressed.\n");
          break;
        case 'D':
          printf("Right arrow pressed.\n");
          break;
        }
      }
    }
  }
}

#include "area.h"
#include "constants.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

struct termios termios_t;

void enable_raw_mode(struct termios *t);
void disable_raw_mode(struct termios *t);

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

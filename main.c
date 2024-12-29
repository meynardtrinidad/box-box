#include "area.h"
#include "constants.h"
#include "controls.h"
#include "utils.h"
#include <stdint.h>
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

  int start_pos;
  while (start_pos = generate_random_number(MIN, len), area[start_pos] != SPACE)
    ;
  area[start_pos] = PLAYER;

  int random_pos;
  while (random_pos = generate_random_number(MIN, len),
         area[random_pos] != SPACE)
    ;
  area[random_pos] = BOX;

  display_area(area, len, x, y);

  enable_raw_mode(&termios_t);

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == 'q') // Exit on 'q' press
      break;

    handle_move_player(c, &start_pos, area, len, x, y);
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

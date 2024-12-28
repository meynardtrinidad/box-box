#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

int generate_random_number(int min, int max) {
  return min + (rand() % (max - min + 1));
}

#endif

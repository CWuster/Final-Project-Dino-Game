#include "./Generated_Code/modified_dino.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int m = ((int)sizeof(modified_dino_rom) / (int)sizeof(modified_dino_rom[0]));
  int n = ((int)sizeof(modified_dino_rom[0]) /
           (int)sizeof(modified_dino_rom[0][0]));
  printf("%d\n", m);
  printf("%d\n", n);
  for (int j = 0; j < m; ++j) {
    printf("{");
    for (int i = 0; i < n; ++i) {
      if (modified_dino_rom[j][i] == 0)
        printf(" ");
      else
        printf("*");
    }
    printf("},");
    printf("\n");
  }
}

#include "Modules/sys.h"

int robert_main(void) {
  ASSERT(gpio_ptr, "Failed to map GPIO memory\n");

  char *str;

  /* Initial memory allocation */
  str = (char *) malloc(6);
  strcpy(str, "hello,");
  print(str);
  print("\n");

  /* Reallocating memory */
  str = (char *) realloc(str, sizeof(str), 13);
  strcat(str, " world\n");
  print(str);

  /* free(str); <-- for some reason causes segmentation fault (SIGSEGV) */
  return 0;
}

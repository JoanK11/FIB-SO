#include <unistd.h>

int main(int argc, char *argv[]) {
  char c;

  while (read(10, &c, 1) > 0)
    write(20, &c, 1);

  return 0;
}

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void error_y_exit(char *msg, int exit_code) {
  write(2, msg, strlen(msg));
  exit(exit_code);
}

void Usage() {
  char buff[80];
  sprintf(buff, "Usage: prog1 File1 [... FileN]\n");
  write(2, buff, strlen(buff));
  exit(1);
}

int main(int argc, char *argv[]) {
  int n = argc;
  if (n < 2) Usage();

  // Creem una pipe sense nom
  int fd[2];
  if (pipe(fd) < 0) error_y_exit("Error en la pipe\n", 1);

  for (int i = 1; i < n; ++i) {
    int pid = fork();
    if (pid == 0) {
      dup2(fd[1], 1); close(fd[1]); close(fd[0]);
      execlp("cat", "cat", argv[i], NULL);
      error_y_exit("Error en el execlp\n", 1);
    }
    else if (pid == -1) error_y_exit("Error en el fork\n", 1);
  }
  // n + 1
  int pid = fork();
  if (pid == 0) {
    dup2(fd[0], 10);
    close(fd[0]); close(fd[1]);
    if (mknod("MIPIPE", S_IFIFO | 0640, 0) < 0) {
      if (errno != EEXIST) error_y_exit("Error en mknod\n", 1); // Si la pipe ja existia no dona error, si falla per un altre cas sí dona error
    }
    int npd;
    if ((npd = open("MIPIPE", O_WRONLY)) < 0) error_y_exit("Error en open\n", 1);
    dup2(npd, 20); close(npd);
    execlp("./micat", "./micat", NULL);
    error_y_exit("Error en el execlp\n", 1);
  }

  close(fd[0]); close(fd[1]);
  while (waitpid(-1, NULL, 0) > 0);

  char buff[80];
  sprintf(buff, "He acabat\n");
  write(1, buff, strlen(buff));
}

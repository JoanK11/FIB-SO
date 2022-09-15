
void error_y_exit(char *msg, int exit_status) {
  perror(msg);
  exit(exit_status);
}

void Usage() {
	char buf[120];
	sprintf(buf, "Usage:listaParametros arg1 arg2 [arg3..argn]\nEste programa escribe por su salida la lista de argumentos que recibe\n");
	write(1, buf, strlen(buf));
}
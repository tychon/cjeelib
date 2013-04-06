
#define _POSIX_SOURCE

#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s DEVICE\n", argv[0]);
    exit(1);
  }
  char *device = argv[1];
  
  int serial_fd = open(device, O_RDONLY | O_NOCTTY);
  struct termios config;
  if(tcgetattr(serial_fd, &config) < 0) exit(1);
  config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  config.c_cflag &= ~(CSIZE | PARENB);
  config.c_cflag |= CS8;
  config.c_cc[VMIN]  = 1;
  config.c_cc[VTIME] = 0;
  //TODO make baud rate configurable by command line
  if(cfsetispeed(&config, B57600) < 0 || cfsetospeed(&config, B57600) < 0) exit(1);
  if(tcsetattr(serial_fd, TCSAFLUSH, &config) < 0) exit(1);
  FILE *serial = fdopen(serial_fd, "r");
  
  setbuf(stdout, NULL);
  setbuf(serial, NULL);
  
  char l[255];
  while (1) {
    if (fgets(l, 255, serial) == NULL) return 0;
    char *c = l;
    printf("%s", c);
  }
  
  return 0;
}

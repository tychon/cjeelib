
#define _POSIX_SOURCE

#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_help(char *path) {
  fprintf(stderr, "usage: %s [-b BAUDRATE] DEVICE\n", path);
  fputs("supported baud rates:\n"
      "  B50\n"
      "  B75\n"
      "  B110\n"
      "  B134\n"
      "  B150\n"
      "  B200\n"
      "  B300\n"
      "  B600\n"
      "  B1200\n"
      "  B1800\n"
      "  B2400\n"
      "  B4800\n"
      "  B9600\n"
      "  B19200\n"
      "  B38400\n"
      "  B57600\n"
      "  B115200\n"
      "  B230400\n"
      "  B500000\n"
      "  B576000\n"
      "  B1000000\n\n", stderr);
  exit(1);
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  
  char *device = NULL;
  int baudrate = B57600;
  for (int i = 1; i < argc; i++) {
    if (! strcmp(argv[i], "-b")) {
      if (++i >= argc) print_help(argv[0]);
      #define TRY(rate) if (!strcmp(argv[i], #rate)) baudrate = rate;
           TRY(B50)
      else TRY(B75)
      else TRY(B110)
      else TRY(B134)
      else TRY(B150)
      else TRY(B200)
      else TRY(B300)
      else TRY(B600)
      else TRY(B1200)
      else TRY(B1800)
      else TRY(B2400)
      else TRY(B4800)
      else TRY(B9600)
      else TRY(B19200)
      else TRY(B38400)
      else TRY(B57600)
      else TRY(B115200)
      else TRY(B230400)
      else TRY(B500000)
      else TRY(B576000)
      else TRY(B1000000)
      else print_help(argv[0]);
    } else if (! device) {
      device = argv[i];
    } else print_help(argv[0]);
  }
  if (! device) {
    fputs("Error: missing path to device\n\n", stderr);
    print_help(argv[0]);
  }
  fprintf(stderr, "baud rate termios bits: 0x%x\n", baudrate);
  
  
  int serial_fd = open(device, O_RDONLY | O_NOCTTY);
  struct termios config;
  if(tcgetattr(serial_fd, &config) < 0) exit(1);
  config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  config.c_cflag &= ~(CSIZE | PARENB);
  config.c_cflag |= CS8;
  config.c_cc[VMIN]  = 1;
  config.c_cc[VTIME] = 0;
  if(cfsetispeed(&config, baudrate) < 0 || cfsetospeed(&config, baudrate) < 0) exit(1);
  if(tcsetattr(serial_fd, TCSANOW, &config) < 0) exit(1);
  FILE *serial = fdopen(serial_fd, "r");
  
  setbuf(serial, NULL);
  
  char l[255];
  while (1) {
    if (fgets(l, 255, serial) == NULL) return 0;
    char *c = l;
    printf("%s", c);
  }
  
  return 0;
}

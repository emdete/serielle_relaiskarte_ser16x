#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int fd;
	struct termios attr;
	char buf[4];

	if ((fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY)) < 0) {
		fprintf(stderr, "Error opening the device: %s.\n", strerror(errno));
		exit(1);
	}
	tcgetattr(fd, &attr);
	cfsetispeed(&attr, B19200);
	cfsetospeed(&attr, B19200);
	attr.c_cflag &= ~PARENB;
	attr.c_cflag &= ~CSTOPB;
	attr.c_cflag &= ~CSIZE;
	attr.c_cflag |= CS8;
	attr.c_cflag &= ~CRTSCTS;
	attr.c_cflag |= CREAD | CLOCAL;
	attr.c_iflag &= ~(IXON | IXOFF | IXANY);
	attr.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	attr.c_oflag &= ~OPOST;
	attr.c_cc[VMIN] = 4;
	attr.c_cc[VTIME] = 1;
	if ((tcsetattr(fd, TCSANOW, &attr)) < 0) {
		fprintf(stderr, "Error setting attributes: %s.\n", strerror(errno));
		exit(1);
	}
	if (tcflush(fd, TCIFLUSH) < 0) {
		fprintf(stderr, "Error flushing: %s.\n", strerror(errno));
		exit(1);
	}
	buf[0] = 0x01;
	buf[1] = 0x01;
	buf[2] = 0xf0;
	buf[3] = buf[0] ^ buf[1] ^ buf[2];
	printf("Sending %02x %02x %02x %02x.\n", buf[0], buf[1], buf[2], buf[3]);
	if (write(fd, &buf, sizeof(buf)) != sizeof(buf)) {
		fprintf(stderr, "Error reading: %s.\n", strerror(errno));
		exit(1);
	}
	if (read(fd, &buf, sizeof(buf)) != sizeof(buf)) {
		fprintf(stderr, "Error writing: %s.\n", strerror(errno));
		exit(1);
	}
	printf("Return %02x %02x %02x %02x.\n", buf[0], buf[1], buf[2], buf[3]);
}

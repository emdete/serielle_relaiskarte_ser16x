#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

unsigned char* send_command(int fd, unsigned char* buf, unsigned char a, unsigned char b, unsigned char c) {
	buf[0] = a;
	buf[1] = b;
	buf[2] = c;
	buf[3] = buf[0] ^ buf[1] ^ buf[2];
	printf("Sending %02x %02x %02x %02x.\n", buf[0], buf[1], buf[2], buf[3]);
	fflush(stdout);
	if (write(fd, buf, 4) != 4) {
		fprintf(stderr, "Error reading: %s.\n", strerror(errno));
		return NULL;
	}
	if (read(fd, buf, 4) != 4) {
		fprintf(stderr, "Error writing: %s.\n", strerror(errno));
		return NULL;
	}
	printf("Return %02x %02x %02x %02x.\n", buf[0], buf[1], buf[2], buf[3]);
	return buf;
}

unsigned char* send_set(int fd, unsigned char* buf, int port, int state) {
	return send_command(fd, buf, 0x01, port, state);
}

unsigned char* send_get(int fd, unsigned char* buf, int port, int state) {
	return send_command(fd, buf, 0x02, port, state);
}

void help() {
	printf(
		"Benutzung: sersw <Gerätename> ([+-=](bit ...) ...)\n"
		"Beispiele:\n"
		"Schalte Relais 1 ein, ohne die anderen zu ändern:\n"
		"          sersw /dev/ttyUSB0 +1\n"
		"Schalte Relais 2, 4 und 6 ein und 5 aus:\n"
		"          sersw /dev/ttyUSB0 +246 -5\n"
		"Schalte alle aus, aber Relais 4 und 6 an:\n"
		"          sersw /dev/ttyUSB0 =46\n"
		"Das Programm macht keine Validierung der Parameter\n"
		"Sollte das Programm hängen, weil das Board nicht reagiert, hilft ein Reset des Boards.\n"
		);
}

int main(int argc, char** argv) {
	int fd = -1;
	unsigned char state = 0;
	struct termios attr;
	unsigned char buf[4];

	if (argc < 2) {
		fprintf(stderr, "Not enough arguments.\n");
		help();
		exit(0);
	}
	if ((fd = open(argv[1], O_RDWR | O_NOCTTY)) < 0) {
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
	if (tcsetattr(fd, TCSANOW, &attr) < 0) {
		fprintf(stderr, "Error setting attributes: %s.\n", strerror(errno));
		exit(1);
	}
	if (tcflush(fd, TCIFLUSH) < 0) {
		fprintf(stderr, "Error flushing: %s.\n", strerror(errno));
		exit(1);
	}
	if (send_get(fd, buf, 0x01, state) == NULL) {
		exit(1);
	}
	state = buf[2];
	for (int argi = 2; argi < argc; argi++) {
		switch (argv[argi][0]) {
		case '-':
			for (int i=1; i<strlen(argv[argi]); i++) {
				int bit = argv[argi][i] - '1';
				printf("Bit %d off\n", bit);
				state &= ~(1<<bit);
			}
			break;
		case '=':
			state = 0;
			printf("All off\n");
			// fall through:
		case '+':
			for (int i=1; i<strlen(argv[argi]); i++) {
				int bit = argv[argi][i] - '1';
				printf("Bit %d on\n", bit);
				state |= 1<<bit;
			}
			break;
		default:
			fprintf(stderr, "Wrong argument.\n");
			exit(1);
			break;
		}
	}
	if (send_set(fd, buf, 0x01, state) == NULL) {
		exit(1);
	}
	exit(0);
}

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>
#include "labels.h"
#include <errno.h>
#include <linux/ioctl.h>

#define flush() fflush(stdout)

// Output
char outputDevice[32] = "/dev/input/";
int outputFd;
input_event outputEvent;
// Input
char inputDevice[32] = "/dev/input/";
pollfd inputPollfd;

bool isDebug = false;

const char* get_label(const struct label* labels, int value) {
	while (labels->name && value != labels->value)
		labels++;
	return labels->name;
}

void send_event(const unsigned short type, const unsigned short code, const signed int value) {
	memset(&outputEvent, 0, sizeof(input_event));
	outputEvent.type = type;
	outputEvent.code = code;
	outputEvent.value = value;

	const int ret = write(outputFd, &outputEvent, sizeof(input_event));
	if (ret < sizeof(input_event))
		printf("Write event failed, %s\n", strerror(errno));
}

void handle_event(input_event ev) {
	if (isDebug) {
		const char* type_label = get_label(ev_labels, ev.type);
		printf("Type:\t%s\tCode:\t%d\tValue:\t%d\n", type_label, ev.code, ev.value);
	}

	if (ev.type == EV_KEY && ev.code == BTN_RIGHT) {
		if (isDebug) {
			if (ev.value == 1)
				printf("Button down\n");
			else
				printf("Button up\n");
		}
		send_event(EV_KEY, KEY_O, ev.value);
		send_event(EV_SYN, SYN_REPORT, ev.value);
	}
}

int main(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		const char arg = argv[i][1];
		
		if (arg == 'd') {
			isDebug = true;
		}
		else if (arg == 'i') {
			if (++i == argc) {
				printf("Error in argument -i\n");
				return 1;
			}
			strcat(inputDevice, argv[i]);
		}
		else if (arg == 'o') {
			if (++i == argc) {
				printf("Error in argument -o\n");
				return 1;
			}
			strcat(outputDevice, argv[i]);
		}
	}

	int bytesRead, bufferPosition, version;
	input_event eventsBuffer[256];

	printf("Debug %s\n", isDebug ? "True" : "False");
	printf("Input %s\n", inputDevice);
	printf("Output %s\n", outputDevice);
	flush();

	inputPollfd.fd = open(inputDevice, O_RDONLY | O_NONBLOCK);
	inputPollfd.events = POLLIN;

	if (inputPollfd.fd == -1) {
		printf("Could not open input stream\n");
		return 1;
	}

	outputFd = open(outputDevice, O_RDWR | O_NONBLOCK);

	if (outputFd == -1 || ioctl(outputFd, EVIOCGVERSION, &version)) {
		printf("Could not open output stream\n");
		return 1;
	}

	while (true) {
		poll(&inputPollfd, 1, -1);

		if (inputPollfd.revents == POLLIN) {
			bytesRead = read(inputPollfd.fd, eventsBuffer, sizeof(eventsBuffer));

			if (isDebug) {
				printf("Read Bytes: %d\n", bytesRead);
				flush();
			}

			for (bufferPosition = 0; bufferPosition < bytesRead; bufferPosition += sizeof(input_event)) {
				if (bytesRead - bufferPosition < sizeof(input_event))
					printf("Could not get event\n");
				const input_event ev = eventsBuffer[bufferPosition / sizeof(input_event)];
				handle_event(ev);
			}
			if (isDebug)
				flush();
		}
	}

	return 0;
}
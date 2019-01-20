#include <stdlib.h>
#include <stdio.h>
#include <poll.h>
#include <ctype.h>
#include <unist.ih>
#include "mbconsole.h"

// TODO? uint32_t* argv[])

int extra_debug = 0;
void exe_test(uint32_t argc, char* argv[]){
	printf("TEST OK\n");
}

void exe_parse(uint32_t argc, char* argv[]){
	printf("Parsed %d args\n", argc);
	for (uint32_t i = 0; i < argc; i++){
		printf("%d | %s\n", argc, argv[i]);
	} 
}

void exe_echo(uint32_t argc, char* argv[]){
	for (uint32_t i = 0; i < argc; i++)
	{
		printf("%s ", argv[i]);
	}
	printf("\n");
}

void exe_exit(uint32_t argc, char* argv[]){
	printf("exit");
	printf("\n");
	exit(0);
}

console_executable exes[] = {
	{"test", &exe_test},
	{"parse", &exe_parse},
	{"echo", &exe_echo},
	{"exit", &exe_exit},
};
console_executable_table exe_table = { exes, sizeof(exes)/sizeof(exes[0])};

int main(void){

	console_set_exe_table(&exe_table);
  system ("/bin/stty raw");
  while (true) {
		struct pollfd fds;

		/* watch stdin for input */
		fds.fd = STDIN_FILENO;
		fds.events = POLLIN;

		// /* watch stdout for ability to write */
		// fds.fd = STDOUT_FILENO;
		// fds.events = POLLOUT;

		int ret;
		if (ret = poll(&fds, 1, 1000) == -1)
		{
			perror("polling stdin returned -1 - err");
			return(1);
		}
		/* // 0 is also our nice FDS descriptor
		else if (ret == 0)
		{
			perror("polling stdin returned 0 - timeout no character to read");
		}
		*/
		else if (fds.revents & POLLIN)
		{
			int fd = STDIN_FILENO;
			size_t n;
			char b[100];
			n = read(fd, b, sizeof(b));
			for (uint32_t i = 0; i < n; i++)
			{
				const char c = b[i];
				printf("getchar() = %c\n", isprint(c) ? c : 'x');
				console_interrupt_handler_with_char(c);
			}
		}
		else if (fds.revents & POLLHUP)
		{
			printf("POLLHUP\n");
		}
		else if (fds.revents & POLLERR)
		{
			printf("POLLERR\n");
		}
		else if (fds.revents & POLLNVAL)
		{
		}
		if (extra_debug)
		{
			printf("revents: %08x\n", fds.revents);
			printf("POLLIN: %08x\n", POLLIN);
		}
		fds.revents = 0;
		console_exec();
		}
	}



#if EXAMPLE
#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>

#define TIMEOUT 5

int main (void)
{
	struct pollfd fds[2];
	int ret;

	/* watch stdin for input */
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	/* watch stdout for ability to write */
	fds[1].fd = STDOUT_FILENO;
	fds[1].events = POLLOUT;

	ret = poll(fds, 2, TIMEOUT * 1000);

	if (ret == -1) {
		perror ("poll");
		return 1;
	}

	if (!ret) {
		printf ("%d seconds elapsed.\n", TIMEOUT);
		return 0;
	}

	if (fds[0].revents & POLLIN)
		printf ("stdin is readable\n");

	if (fds[1].revents & POLLOUT)
		printf ("stdout is writable\n");

	return 0;
#endif

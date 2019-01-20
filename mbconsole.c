#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mbconsole.h"

static const uint8_t nbuf = 2;
static const uint8_t sbuf = 100;
static const uint8_t MAX_ARGS = 10;
char console_buf [nbuf][sbuf];
char* input_buf = NULL;
char* exec_buf = NULL;
uint8_t input_pos = 0;


console_executable* executables;
uint32_t nexecutables;

uint32_t min(uint32_t s1, uint32_t s2) {
	return ((s1 > s2) ? s1 : s2);
}

// multi-buffer console

void console_init()
{
	memset(&console_buf, 0, sizeof(console_buf));
	input_buf = &console_buf[0][0];
	input_pos = 0;
	exec_buf = NULL;
}

#if 0
void console_interrupt_handler() {
	console_interrupt_handler_with_char(console_read_input());
}
#endif

void console_interrupt_handler_with_char(char c) {

	if (input_buf == NULL && exec_buf == NULL)
	{
		console_init();
	}

        /* BUFFER POINTER CHECK */
        if (!input_buf) // no extra buffer, throw out character
        {
                printf("[Console] ERR: no buffer, drop ch:%d\n", c);
                return;
        }

        /* BUFFER BOUNDS CHECK */
        // Allow last char to be eol
        if (input_pos + 1 == sbuf && (c != '\r'))
        {
                printf("[Console] ERR: input full\n");
                return;
        }

        /* SAVE CHAR */
        /* CHECK FOR ENTER */
        if ((input_buf[input_pos++] = c) == '\r') {
	printf("nl\n");

                // replace newline with eol
                input_buf[input_pos - 1]  = '\0';

                // g
                for (uint32_t i = 0; i < nbuf; i++) {
                        // TODO re-write for multi-buffer
                        if ((console_buf[i] != input_buf) && (console_buf[i] != exec_buf)) {
                                exec_buf = input_buf;
                                input_buf = console_buf[i];
                                input_pos = 0;
                                return;
                        }
                }
        }
}


bool console_exec()
{
        if (!exec_buf || !executables || !nexecutables)
                return(false);

        /* Buffer bounds check */
        if (!memchr(exec_buf, '\0', sbuf))
        {
                exec_buf = NULL;
		return(false);
        }
	
	uint32_t argc = 0;
	char* argv[MAX_ARGS];
	memset(argv, 0, sizeof(argv));
	argv[argc++] = exec_buf;

	// -1: if the last char is a space we can't split it into a second word
	for (uint32_t i = 0; i < (min(strlen(exec_buf), sbuf) - 1); i++){
		if (exec_buf[i] == ' '){
			exec_buf[i]  = '\0';
			argv[argc++] = &exec_buf[i + 1];
		}
	}

	for (uint32_t i = 0; i < nexecutables; i++)
	{
		console_executable* exe = &executables[i];
		if (strncmp(exec_buf, exe->name, min(sbuf, strlen(exe->name))) == 0)
		{
			exe->exe(argc, argv);
			exec_buf = NULL;
			return(true);
		}
	}
	exec_buf = NULL;
	return(false);
}

bool console_set_exe_table(console_executable_table* t)
{
	executables = t->exes;
	nexecutables = t->n;
	return 1;
}

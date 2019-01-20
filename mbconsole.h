#ifndef _MULTI_BUFFER_CONSOLE_H_
#define _MULTI_BUFFER_CONSOLE_H_

#include <stdint.h>


/*
 * MULTI BUFFER CONSOLE
 * USES MORE RAM
 * ALLOWS {nbuf} NUMBER OF {sbuf} LENGTH COMMANDS 
 * TO BE ENTERED BEFORE THROWING AWAY CHARACTERS
 * GREAT FOR PROGRAMATIC CONSOLE INPUT
 **/

typedef void (*executable)(uint32_t argc, char* argv[]);
struct console_executable
{
        const char* name;
        executable exe;
};

struct console_executable_table
{
	console_executable* exes;
	uint32_t n;
        
};



// Project 
bool console_set_executable_array();
void console_interrupt_handler();
void console_interrupt_handler_with_char(char c);
char console_read_input();
bool console_exec();
bool console_set_exe_table(console_executable_table* t);





#endif //_MULTI_BUFFER_CONSOLE_H_


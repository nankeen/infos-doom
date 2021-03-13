/* SPDX-License-Identifier: MIT */

#include <libcore.h>

static void run_command(const char *cmd)
{
	//printf("Running Command: %s\n", cmd);
	
	char prog[64];
	int n = 0;
	while (*cmd && *cmd != ' ' && n < 63) {
		prog[n++] = *cmd++;
	}
	
	prog[n] = 0;
	
	if (*cmd) cmd++;
	
	HPROC pcmd = exec(prog, cmd);
	if (is_error(pcmd)) {
		printf("error: unable to run command '%s'\n", cmd);
	} else {
		wait_proc(pcmd);
	}
}

int main(const char *)
{
	printf("This is the InfOS shell.  Path resolution is not-yet-implemented, so you\n"
			"must type the command exactly, e.g. try typing: /usr/ls.\n\n");
	
	printf("Use the cat program to view the README: /usr/cat /usr/docs/README\n\n");
	
	while (true) {
		printf("> ");

		char command_buffer[128];
		int n = 0;

		while (n < 127) {
			char c = getch();
			if (c == 0) continue;
			if (c == '\n') break;
			if (c == '\b') {
				if (n > 0) {
					command_buffer[--n] = 0;
					printf("\b");
				}
			} else {
				command_buffer[n++] = c;
				printf("%c", c);
			}
		}

		printf("\n");
		if (n == 0) continue;
		
		command_buffer[n] = 0;
		
		if (strcmp("exit", command_buffer) == 0) break;
		run_command(command_buffer);
	}
	
	return 0;
}

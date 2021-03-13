/* SPDX-License-Identifier: MIT */

#include <libcore.h>

int main(const char *)
{
	printf("Welcome to InfOS!\nStarting the system...\n");
	
	while (true) {
		HPROC shell_proc = exec("/usr/shell", NULL);
		if (is_error(shell_proc)) {
			printf("Error: Unable to launch shell");
			return 1;
		}

		wait_proc(shell_proc);

		printf("SHELL TERMINATED!  PRESS ENTER TO RESTART\n");
		while (getch() != '\n');
	}
	
	return 0;
}

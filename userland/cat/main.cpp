/* SPDX-License-Identifier: MIT */

#include <libcore.h>

int main(const char *cmdline)
{
	if (!cmdline || strlen(cmdline) == 0) {
		return 1;
	}
	
	HFILE file = open(cmdline, 0);
	if (is_error(file)) {
		printf("error: unable to open file '%s' for reading\n", cmdline);
		return 1;
	}
	
	char buffer[64];
	int bytes_read;
	
	do {
		bytes_read = read(file, buffer, sizeof(buffer)-1);
		buffer[bytes_read] = 0;
		
		printf("%s", buffer);
	} while (bytes_read > 0);
	
	close(file);

	return 0;
}

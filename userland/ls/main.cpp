/* SPDX-License-Identifier: MIT */

#include <libcore.h>

int main(const char *cmdline)
{
	const char *path;
	if (!cmdline || strlen(cmdline) == 0) {
		path = "/usr";
	} else {
		path = cmdline;
	}
	
	HDIR dir = opendir(path, 0);
	if (is_error(dir)) {
		printf("Unable to open directory '%s' for reading.\n", path);
		return 1;
	}
	
	printf("Directory Listing of '%s':\n", path);

	struct dirent de;
	while (readdir(dir, &de)) {
		printf("  %s (%u bytes)\n", de.name, de.size);
	}
	closedir(dir);

	return 0;
}

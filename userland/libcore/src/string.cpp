/* SPDX-License-Identifier: MIT */

#include <libcore.h>

int strcmp(const char *l, const char *r)
{
	while (*l && *r) {
		if (*l++ != *r++) return -1;
	}
	
	return 0;
}

int strlen(const char *s)
{
	int count = 0;
	while (*s++) {
		count++;
	}
	
	return count;
}
